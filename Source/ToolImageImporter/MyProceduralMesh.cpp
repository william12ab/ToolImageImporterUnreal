// Fill out your copyright notice in the Description page of Project Settings.
#include "MyProceduralMesh.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"

#include <Runtime/Engine/Public/ImageUtils.h>

// Sets default values
AMyProceduralMesh::AMyProceduralMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ScnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Main"));
	SetRootComponent(ScnComponent);
	procedural_mesh_comp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	procedural_mesh_comp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	width_ = 4;
	height_ = 4;
	spacing_ = 20.0f;
	count = 0;
}

// Called when the game starts or when spawned
void AMyProceduralMesh::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyProceduralMesh::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


void AMyProceduralMesh::SmoothTerrain(TArray<float>& c_)
{
	for (int j = 0; j < (width_); j++){
		for (int i = 0; i < (width_); i++){
			int count_loc = 0;
			float tHeight = 0.0f;

			if (i - 1 >= 0)											//left
			{
				count_loc++;
				tHeight += c_[(j * width_) + (i - 1)];
			}

			if (i + 1 < width_)									//right
			{
				count_loc++;
				tHeight += c_[(j * width_) + (i + 1)];
			}

			if (j - 1 >= 0)											//down	
			{
				count_loc++;
				tHeight += c_[((j - 1) * width_) + i];
			}

			if (j + 1 < width_)									//up
			{
				count_loc++;
				tHeight += c_[((j + 1) * width_) + i];
			}

			if ((i - 1 >= 0) && (j - 1 >= 0))								//down left 
			{
				count_loc++;
				tHeight += c_[((j - 1) * width_) + (i - 1)];
			}

			if ((i + 1 < width_) && (j - 1 >= 0))								//down right
			{
				count_loc++;
				tHeight += c_[((j - 1) * width_) + (i + 1)];
			}

			if ((i - 1 >= 0) && (j + 1 < width_))								//up left 
			{
				count_loc++;
				tHeight += c_[((j + 1) * width_) + (i - 1)];
			}

			if ((i + 1 < width_) && (j + 1 < width_))								//up right
			{
				count_loc++;
				tHeight += c_[((j + 1) * width_) + (i + 1)];
			}

			tHeight /= (float)count_loc;

			c_[(j * width_) + i] = tHeight;
		}
	}
}


float AMyProceduralMesh::FindT(const FVector2D& p1, const FVector2D& p2, const FVector2D& p3)
{
	auto a = p1 - p3;
	auto b = p1 - p2;

	float t = 0.0f;
	if (a.X == 0 || b.X == 0){
		float c2 = ((float)a.Y / (float)b.Y);
		t = c2;
		if (t < 0.0f){
			t *= -1;
		}
		if (a.Y == 0 && b.Y == 0){
			t = 0;
		}
	}
	else if (a.Y == 0 || b.Y == 0){
		float c1 = ((float)a.X / (float)b.X);
		t = c1;
		if (a.X == 0 && b.X == 0){
			t = 0;
		}
	}
	else{
		float c1 = ((float)a.X / (float)b.X);
		float c2 = ((float)a.Y / (float)b.Y);
		t = (c1 + c2) / 2.0f;
	}
	return t;
}

void AMyProceduralMesh::SetMaterial(UTexture* t_)
{
	material_interface = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/Materials/TerrainMaterial.TerrainMaterial'"));
	material_instance = UMaterialInstanceDynamic::Create(material_interface, this);
	material_instance->SetTextureParameterValue(FName("terr_text"), t_);		//this is reference also to the material obj in scene, have a look if you dont get it.
	procedural_mesh_comp->SetMaterial(0, material_instance);
}
void AMyProceduralMesh::ClearMeshData(){
	m_verts.Empty();
	m_tris.Empty();
	m_u_vs.Empty();
	m_norms.Empty();
	m_vert_colors.Empty();
	m_tangents.Empty();
}



void AMyProceduralMesh::GenerateVerts(){
	float uv_spacing = 1.0f / FMath::Max(height_, width_);

	for (int32 y = 0; y < height_; y++){
		for (int32 x = 0; x < width_; x++){
			m_verts.Add(FVector(x * spacing_, y * spacing_, 0.0f));
			m_norms.Add(FVector(0.0f, 0.0f, .0f));
			m_u_vs.Add(FVector2D(x * uv_spacing, y * uv_spacing));
			m_vert_colors.Add(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
			m_tangents.Add(FProcMeshTangent(0.0f, 1.0f, 0.0f));
		}
	}
}

void AMyProceduralMesh::CalculateNormals(){
	
	for (int j = 0; j < (height_ - 1); j++) {
		for (int i = 0; i < (width_- 1); i++) {
			//Calculate the plane normals
			FVector a, b, c;	//Three corner vertices
			a = m_verts[j * height_ + i];
			b = m_verts[j * height_+ i + 1];
			c = m_verts[(j + 1) * height_+ i];

			//Two edges
			FVector ab(c.X - a.X, c.Y - a.Y, c.Z - a.Z);
			FVector ac(b.X - a.X, b.Y - a.Y, b.Z - a.Z);

			//Calculate the cross product
			FVector cross;
			cross.X = ab.Y * ac.Z - ab.Z * ac.Y;
			cross.Y = ab.Z * ac.X - ab.X * ac.Z;
			cross.Z = ab.X * ac.Y - ab.Y * ac.X;
			float mag = (cross.X * cross.X) + (cross.Y * cross.Y) + (cross.Z * cross.Z);
			mag = sqrtf(mag);
			cross.X /= mag;
			cross.Y /= mag;
			cross.Z /= mag;
			m_norms[j * height_+ i]= -cross;
		}
	}

	FVector smoothedNormal(0, 0, 0);
	for (int j = 0; j < width_; j++) {
		for (int i = 0; i < width_; i++) {
			smoothedNormal.X = 0;
			smoothedNormal.Y = 0;
			smoothedNormal.Z = 0;
			float count_n = 0;
			//Left planes
			if ((i - 1) >= 0) {
				//Top planes
				if ((j) < (width_- 1)) {
					smoothedNormal.X += m_norms[j * width_ + (i - 1)].X;
					smoothedNormal.Y += m_norms[j * width_ + (i - 1)].Y;
					smoothedNormal.Z += m_norms[j * width_ + (i - 1)].Z;
					count_n++;
				}
				//Bottom planes
				if ((j - 1) >= 0) {
					smoothedNormal.X += m_norms[(j - 1) * width_ + (i - 1)].X;
					smoothedNormal.Y += m_norms[(j - 1) * width_ + (i - 1)].Y;
					smoothedNormal.Z += m_norms[(j - 1) * width_ + (i - 1)].Z;
					count_n++;
				}
			}
			//right planes
			if ((i) < (width_ - 1)) {

				//Top planes
				if ((j) < (width_ - 1)) {
					smoothedNormal.X += m_norms[j * width_ + i].X;
					smoothedNormal.Y += m_norms[j * width_ + i].Y;
					smoothedNormal.Z += m_norms[j * width_ + i].Z;
					count_n++;
				}
				//Bottom planes
				if ((j - 1) >= 0) {
					smoothedNormal.X += m_norms[(j - 1) * width_ + i].X;
					smoothedNormal.Y += m_norms[(j - 1) * width_ + i].Y;
					smoothedNormal.Z += m_norms[(j - 1) * width_ + i].Z;
					count_n++;
				}
			}
			smoothedNormal.X /= count_n;
			smoothedNormal.Y /= count_n;
			smoothedNormal.Z /= count_n;

			float mag = sqrt((smoothedNormal.X * smoothedNormal.X) + (smoothedNormal.Y * smoothedNormal.Y) + (smoothedNormal.Z * smoothedNormal.Z));
			smoothedNormal.X /= mag;
			smoothedNormal.Y /= mag;
			smoothedNormal.Z /= mag;

			m_norms[j * width_ + i]= smoothedNormal;
		}
	}
}

void AMyProceduralMesh::GenerateTris(){
	for (int32 y = 0; y < (height_-1); y++){
		for (int32 x = 0; x < (width_ - 1); x++){
			m_tris.Add(x + (y * width_));					//current vertex
			m_tris.Add(x + (y * width_) + width_);			//current vertex + row
			m_tris.Add(x + (y * width_) + width_ + 1);		//current vertex + row + one right

			m_tris.Add(x + (y * width_));					//current vertex
			m_tris.Add(x + (y * width_) + width_ + 1);		//current vertex + row + one right
			m_tris.Add(x + (y * width_) + 1);				//current vertex + one right
		}
	}
}

void AMyProceduralMesh::UpdateVerts(const float& d_spacing_)
{
	for (int32 y = 0; y < height_; y++) {
		for (int32 x = 0; x < width_; x++) {
			m_verts[y * height_ + x].X*= d_spacing_;
			m_verts[y * height_ + x].Y*= d_spacing_;
		}
	}
	procedural_mesh_comp->UpdateMeshSection_LinearColor(0, m_verts, m_norms, m_u_vs, m_vert_colors, m_tangents);
}

void AMyProceduralMesh::CreateMesh(int& d_height_, int& d_width_, float& d_spacing_)
{
	height_ = d_height_;
	width_ = d_width_;
	spacing_ = 20.0f;
	ClearMeshData();
	GenerateVerts();
	GenerateTris();
	//Function that creates mesh section
	procedural_mesh_comp->CreateMeshSection_LinearColor(0, m_verts, m_tris, m_norms, m_u_vs, m_vert_colors, m_tangents, true);
}


void AMyProceduralMesh::ChangeTest(const TArray< FVector2D>& track_points)
{
	for (int i = 0; i < track_points.Num(); i++){
		int x = track_points[i].X-1;
		int y = track_points[i].Y;
		m_vert_colors[y * height_ + x] = FLinearColor(0.114435, 0.076185, 0.046665, 0.1);
		x = track_points[i].X + 2;
		m_vert_colors[y * height_ + x] = FLinearColor(0.114435, 0.076185, 0.046665, 0.1);
	}
}

void AMyProceduralMesh::ModiVerts(TArray<float>& c_, const int& m_)
{
	SmoothTerrain(c_);
	SmoothTerrain(c_);
	for (int32 y = 0; y < height_; y++) {
		for (int32 x = 0; x < width_; x++) {
			m_verts[y * height_ + x].Z = (c_[y * height_ + x]* spacing_)/ m_;
		}
	}
	
	CalculateNormals();
	procedural_mesh_comp->bCastDynamicShadow=false;
	procedural_mesh_comp->UpdateMeshSection_LinearColor(0, m_verts, m_norms, m_u_vs, m_vert_colors, m_tangents);
	
	material_interface = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/Materials/TerrainMaterial.TerrainMaterial'"));
	material_instance = UMaterialInstanceDynamic::Create(material_interface, this);
	procedural_mesh_comp->SetMaterial(0, material_instance);
}

int count_digit(int number) {
	int count = 0;
	while (number != 0) {
		number = number / 10;
		count++;
	}
	return count;
}

//takes in int (the coord in question) rounds to the nearest 10 integer. so 406 becomes 410
int round_up(const int& d)
{
	int divider = 0;
	int digits = count_digit(d);
	switch (digits)
	{
	case 1:
		divider = 10;
		break;
	case 2:
		divider = 10;
		break;
	case 3:
		divider = 10;
		break;
	case 4:
		divider = 100;
		break;
	}
	float division_amount = round(d) / divider;
	int rounded_to_nearest_ten = round(division_amount);
	int final_digit = rounded_to_nearest_ten * divider;
	return final_digit;
}

void AMyProceduralMesh::ChangeForSpline(const TArray<FVector>& verts_)
{
	int non_rounded = (int)verts_[0].X;		//rounded down so 406.6 = 406
	int rounded = round(non_rounded);		//up so 406.6 = 407

	int new_num = round_up(non_rounded);
	int new_num_rounded = round_up(rounded);

	for (int i = 0; i < m_verts.Num(); i++)
	{
		if (new_num == (int)m_verts[i].X || new_num_rounded == (int)m_verts[i].X)
		{
			non_rounded = (int)verts_[0].Y;
			rounded = round(non_rounded);
			new_num = round_up(non_rounded);
			new_num_rounded = round_up(non_rounded);
			for (int j = 0; j < m_verts.Num(); j++)
			{
				if (new_num == (int)m_verts[j].Y || new_num_rounded == (int)m_verts[j].Y)
				{
					UE_LOG(LogTemp, Warning, TEXT("equal to y"));
					UE_LOG(LogTemp, Warning, TEXT("x %d"), (int)m_verts[i].X);
					UE_LOG(LogTemp, Warning, TEXT("y %d"), (int)m_verts[i].Y);
				}
			}
		}
		if ((new_num+10) == (int)m_verts[i].X || (new_num_rounded+10) == (int)m_verts[i].X)
		{
			non_rounded = (int)verts_[0].Y;
			rounded = round(non_rounded);
			new_num = round_up(non_rounded);
			new_num_rounded = round_up(non_rounded);
			for (int j = 0; j < m_verts.Num(); j++)
			{
				if ((new_num +10)== (int)m_verts[j].Y || (new_num_rounded+10) == (int)m_verts[j].Y)
				{
					UE_LOG(LogTemp, Warning, TEXT("equal to y, plus 10"));
				}
			}
		}
	}

}


FVector2D NormalisedOppositeDir(const FVector& c1, const FVector& c2)
{
	FVector2D dir_, dir_norm_, dir_opp_, dir_norm_opp_;

	FVector2D p1= FVector2D(c1.X,c1.Y);					//control points
	FVector2D p2 = FVector2D(c2.X,c2.Y);

	auto p3 = (p1.X, p2.Y);				//opposite of control points kinda
	auto p4 = (p2.X, p1.Y);

	//directional vector
	dir_ = FVector2D(p2 - p1);
	dir_opp_ = FVector2D(p4 - p3);

	//normalising vector
	dir_norm_.X = dir_.X / sqrt(pow(dir_.X, 2) + pow(dir_.Y, 2));
	dir_norm_.Y = dir_.Y / sqrt(pow(dir_.X, 2) + pow(dir_.Y, 2));
	dir_norm_opp_.X = dir_opp_.X / sqrt(pow(dir_opp_.X, 2) + pow(dir_opp_.Y, 2));
	dir_norm_opp_.Y = dir_opp_.Y / sqrt(pow(dir_opp_.X, 2) + pow(dir_opp_.Y, 2));

	//doesnt work if the line is | or -, you know what i mean 
	if (dir_norm_ == FVector2D(0.0f, 1.0f) || dir_norm_ == FVector2D(0.0f, -1.0f))
	{
		dir_norm_opp_ = FVector2D(1.f, 0.f);
	}
	if (dir_norm_ == FVector2D(1.0, 0.0) || dir_norm_ == FVector2D(1.0f, 0.0f))
	{
		dir_norm_opp_ = FVector2D(0.f, 1.f);
	}
	return dir_norm_opp_;
}

void AMyProceduralMesh::SetTrackHeight(const TArray<FVector>& points_)
{
	/*find opposite direction between start and end point.
	set height for pmesh for 80 units in opposite direction. 
	and for each point moving forward to the end point.
	*/
	for (int i = 0; i < points_.Num(); i++){
		auto opp=NormalisedOppositeDir(points_[i], points_[i + 1]);

	}
}