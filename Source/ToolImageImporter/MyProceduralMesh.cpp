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


void AMyProceduralMesh::SmoothTerrain(TArray<float>& c_){
	for (int j = 0; j < (height_); j++){
		for (int i = 0; i < (height_); i++){
			int count_loc = 0;
			float tHeight = 0.0f;

			if (i - 1 >= 0)											//left
			{
				count_loc++;
				tHeight += c_[(j * height_) + (i - 1)];
			}

			if (i + 1 < height_)									//right
			{
				count_loc++;
				tHeight += c_[(j * height_) + (i + 1)];
			}

			if (j - 1 >= 0)											//down	
			{
				count_loc++;
				tHeight += c_[((j - 1) * height_) + i];
			}

			if (j + 1 < height_)									//up
			{
				count_loc++;
				tHeight += c_[((j + 1) * height_) + i];
			}

			if ((i - 1 >= 0) && (j - 1 >= 0))								//down left 
			{
				count_loc++;
				tHeight += c_[((j - 1) * height_) + (i - 1)];
			}

			if ((i + 1 < height_) && (j - 1 >= 0))								//down right
			{
				count_loc++;
				tHeight += c_[((j - 1) * height_) + (i + 1)];
			}

			if ((i - 1 >= 0) && (j + 1 < height_))								//up left 
			{
				count_loc++;
				tHeight += c_[((j + 1) * height_) + (i - 1)];
			}

			if ((i + 1 < height_) && (j + 1 < height_))								//up right
			{
				count_loc++;
				tHeight += c_[((j + 1) * height_) + (i + 1)];
			}

			tHeight /= (float)count_loc;

			c_[(j * height_) + i] = tHeight;
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
			m_vert_colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
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


void AMyProceduralMesh::ModiVerts(TArray<float>& c_, const int& m_){
	SmoothTerrain(c_);
	SmoothTerrain(c_);
	for (int32 y = 0; y < height_; y++) {
		for (int32 x = 0; x < width_; x++) {
			if (height_==400){
				m_verts[y * height_ + x].Z = (c_[y * height_ + x] * spacing_) / m_;
			}
			else{
				m_verts[y * height_ + x].Z = (c_[y * height_ + x] );
			}
		}
	}
	CalculateNormals();

	procedural_mesh_comp->bCastDynamicShadow=false;
	procedural_mesh_comp->UpdateMeshSection_LinearColor(0, m_verts, m_norms, m_u_vs, m_vert_colors, m_tangents);
	material_interface = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/Materials/TerrainMaterial.TerrainMaterial'"));
	material_instance = UMaterialInstanceDynamic::Create(material_interface, this);
	procedural_mesh_comp->SetMaterial(0, material_instance);
}


float Lerp(const float& p1, const float& p2, const float& t)
{
	auto c = (1.0f - t) * p1 + t * p2;
	return c;
}

FVector LerpV(const FVector& p1, const FVector& p2, const float& t)
{
	auto c =(1.0f - t)* p1 + t * p2;
	return c;
}


float Distance(const FVector& p1, const FVector& p2)
{
	float xd = p2.X - p1.X;
	float yd = p2.Y - p1.Y;
	auto c = sqrt((xd * xd) + (yd * yd));
	return c;
}

void AMyProceduralMesh::ChangeVert(const float &x_pos, const float &y_pos, const float &z_pos){
	//float rand_z = FMath::RandRange(-0.5f, 0.5f);
	m_verts[(static_cast<int>(y_pos / spacing_)) * height_ + (static_cast<int>(x_pos / spacing_))].Z = z_pos;
	m_vert_colors[(static_cast<int>(y_pos / spacing_)) * height_ + (static_cast<int>(x_pos / spacing_))] = FLinearColor::Black;
}

void AMyProceduralMesh::ReplaceC(TArray<float>& c_)
{
	TArray<float> temp;
	for (int i = 0; i < m_verts.Num(); i++)
	{
		temp.Add(m_verts[i].Z);
	}
	SmoothTerrain(temp);
	for (int32 y = 0; y < height_; y++) {
		for (int32 x = 0; x < width_; x++) {
			m_verts[y * height_ + x].Z = temp[y * height_ + x];
		}
	}
	CalculateNormals();

	procedural_mesh_comp->UpdateMeshSection_LinearColor(0, m_verts, m_norms, m_u_vs, m_vert_colors, m_tangents);
}


void AMyProceduralMesh::SetHeightProper(const TArray<FVector>& points_, const TArray<FVector>& verts_)
{
	int index_tracker_verts=0;
	int right, left;
	left = 3;
	right = 2;
	float inner_count_size = 100.0f;
	for (int i = 0; i < points_.Num(); i+=2){
		float dist = Distance(points_[i], points_[i + 1]);
		int int_dist = round(dist);
		int_dist *= 20;
		for (int j = 0; j < (int)int_dist; j++){
			float t = (float)(j / (float)int_dist);
			auto left_pos = LerpV(verts_[index_tracker_verts + left], verts_[index_tracker_verts + (left-2)], t);	//gives pos on left
			auto right_pos = LerpV(verts_[index_tracker_verts + right], verts_[index_tracker_verts + (right - 2)], t);	//gives pos on right
			auto centre_pos = LerpV(points_[i], points_[i + 1], t);
			ChangeVert(centre_pos.X, centre_pos.Y, left_pos.Z);
			ChangeVert(left_pos.X, left_pos.Y, left_pos.Z);
			ChangeVert(right_pos.X, right_pos.Y, left_pos.Z);
			for (int k = 0; k < (int)inner_count_size; k++){
				float t_inner = (float)(k / inner_count_size);
				auto a = LerpV(left_pos, right_pos, t_inner);
				ChangeVert(a.X, a.Y, left_pos.Z);
			}
		}
		index_tracker_verts += 4;
	}
}
void AMyProceduralMesh::NearestNeighbourSample(const int& grid_size, const int& new_size, const TArray<FVector>& m_verts_, TArray<FVector> & temp_vec, const int&scale, const TArray<FLinearColor>& temp_colour, TArray<FLinearColor> &new_c){
	for (int i = 0; i < (grid_size); i++) {
		for (int j = 0; j < (grid_size); j++) {
			int x_dash = j * new_size / grid_size;
			int y_dash = i * new_size / grid_size;
			temp_vec[i * new_size + j].Z = m_verts_[i * grid_size + j].Z;
			new_c[y_dash * new_size + x_dash] = temp_colour[i * grid_size + j];
		}
	}
	//coloums
	for (int i = 0; i < (new_size); i++) {
		for (int j = 0; j < (new_size - 1); j += scale) {
			auto c = m_verts_[(i / scale) * grid_size + (j / scale)].Z;
			auto c_ = temp_colour[(i / scale) * grid_size + (j / scale)];
			for (int g = 0; g < scale; g++) {
				temp_vec[(i * new_size) + (j + g)].Z = ( c);
				new_c[(i * new_size) + (j + g)] = c_;
			}
		}
	}
	//////rows - same for rows
	for (int i = 0; i < (new_size - 1); i += scale) {
		for (int j = 0; j < (new_size); j++) {
			auto c = m_verts_[(i / scale) * grid_size + (j / scale)].Z;
			auto c_ = temp_colour[(i / scale) * grid_size + (j / scale)];
			for (int g = 0; g < scale; g++) {
				temp_vec[(i + g) * new_size + j].Z = c;
				new_c[(i + g) * new_size + j] = c_;
			}
		}
	}
}

void AMyProceduralMesh::Resize(const TArray<FVector>& m_verts_, const int& scale_, const TArray<FLinearColor>& temp_colour){
	int grid_size = 400;
	float spacing = 0.f;//just for passing in
	int new_size = grid_size * scale_;
	TArray<FVector> new_z;
	TArray<FLinearColor> new_c;
	new_z.SetNum(new_size*new_size);
	new_c.SetNum(new_size * new_size);
	height_ = new_size;
	width_ = new_size;
	//inital vals
	
	NearestNeighbourSample(grid_size, new_size, m_verts_, new_z, scale_, temp_colour, new_c);
	CreateMesh(height_, width_, spacing);

	//gaining temp verts
	TArray<float> temp_c;
	temp_c.SetNum(new_size * new_size);
	for (int32 y = 0; y < height_; y++) {
		for (int32 x = 0; x < width_; x++) {
			m_verts[y * height_ + x].Z = new_z[y * height_ + x].Z;
			m_vert_colors[y * height_ + x] = new_c[y * height_ + x];
			temp_c[y * height_ + x] = new_z[y * height_ + x].Z;
		}
	}
	ModiVerts(temp_c, 0);//smoothing and setting verts plus regen.
}

void AMyProceduralMesh::Save(TArray<FVector>& temp_, TArray<FLinearColor>& temp_colours){
	for (int i = 0; i < m_verts.Num(); i++){
		temp_.Add(m_verts[i]);
		temp_colours.Add(m_vert_colors[i]);
	}
}