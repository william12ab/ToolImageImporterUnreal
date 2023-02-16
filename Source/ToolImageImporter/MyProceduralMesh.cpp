// Fill out your copyright notice in the Description page of Project Settings.
#include "MyProceduralMesh.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include <Runtime/Engine/Public/ImageUtils.h>

// Sets default values
AMyProceduralMesh::AMyProceduralMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

float AMyProceduralMesh::Lerp(const int& p1, const int& p2, const float& t)
{
	//auto p3 = p1 + ((p2 - p1) * t);
	//return (1 - t) * v0 + t * v1;
	auto c = (1 - t) * p1 + t * p2;
	return c;
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
	material_interface = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/TerrainMaterial.TerrainMaterial'"));
	terrain_mat = UMaterialInstanceDynamic::Create(material_interface, this);
	terrain_mat->SetTextureParameterValue(FName("terr_text"), t_);		//this is reference also to the material obj in scene, have a look if you dont get it.
	procedural_mesh_comp->SetMaterial(0, terrain_mat);
}
void AMyProceduralMesh::ClearMeshData(){
	m_verts.Empty();
	m_tris.Empty();
	m_u_vs.Empty();
	m_norms.Empty();
	m_vert_colors.Empty();
	m_tangents.Empty();
}

void AMyProceduralMesh::CoordAdjuster(float& x, float& y, const int& index, float part_added)
{
	if (index == 0) {
		x += part_added;	//moving to br
	}
	if (index == 1) {			//moving to tl
		x -= part_added;
		y += part_added;
	}
	if (index == 2) {			//to tr
		x += part_added;
	}
	if (index == 3) {
		x -= part_added;
		y -= part_added;
	}
}

float AMyProceduralMesh::FindHeight(float x, float y, int og_x, int og_y, const TArray<int32>& c_)
{	
	float height;
	if (x==(float)og_x){				//VERTICAL LINE
		//p1=og_x,og_y. p2=og_x,og_y+1
		float t = ((float)og_y + 1) - y;
		height = Lerp(c_[og_y * 400 + og_x], c_[(og_y+1) * 400 + og_x],t);
	}
	else if (x == (float)(og_x+1)) {			//VERTICAL LINE
		//p1=og_x+1,og_y. p2=og_x+1,og_y+1
		float t = ((float)og_y + 1) - y;
		height = Lerp(c_[og_y * 400 + (og_x+1)], c_[(og_y + 1) * 400 + (og_x+1)], t);
	}
	else if (y == (float)og_y) {				//HORIZONTAL LINE
		//p1=og_x,og_y. p2=og_x+1,og_y
		float t = ((float)og_x + 1) - x;
		height = Lerp(c_[og_y * 400 + og_x], c_[(og_y) * 400 + (og_x+1)], t);
	}
	else if (y == (float)(og_y + 1)) {		//HORIZONTAL LINE
		//p1=og_x,og_y+1. p2=og_x+1,og_y+1
		float t = ((float)og_x + 1) - x;
		height = Lerp(c_[(og_y+1) * 400 + (og_x)], c_[(og_y + 1) * 400 + (og_x + 1)], t);
	}
	else{
		float p1_t = ((float)og_y)-y;//t on the coord p1. see diagram
		float p1_height = Lerp(c_[og_y * 400 + (og_x)], c_[(og_y + 1) * 400 + (og_x)], p1_t);
		float p2_t = ((float)og_y + 1) - y;//t on the coord p2.
		float p2_height = Lerp(c_[og_y * 400 + (og_x + 1)], c_[(og_y + 1) * 400 + (og_x + 1)], p2_t);
		float t = ((float)og_x + 1) - x;
		height = Lerp(p1_height, p2_height, t);
	}
	//other points
	//p1 = (og_x, p3.y)
	//p2 = (og_x+1, p3.y)
	//p3= (x, y)
	return height;
}

void AMyProceduralMesh::AddMultiVerts(float x,float y, const TArray<int32>& c_, const int& m_, int og_x, int og_y)
{
	float part_added = 0.125;
	for (int i = 0; i < 4; i++){
		AddVert(x, y, c_, m_, og_x, og_y);			//bottom left
		AddVert(x + part_added, y, c_, m_, og_x, og_y);			//bottom right
		AddVert(x, y + part_added, c_, m_, og_x, og_y);			//top left
		AddVert(x + part_added, y + part_added, c_, m_, og_x, og_y);			//top right
		GenerateTrackTris();
		CoordAdjuster(x, y,i, part_added);
	}
}

void AMyProceduralMesh::AddVert(float x, float y, const TArray<int32>& c_, const int& m_, int og_x, int og_y)
{
	float cal_height = FindHeight(x, y, og_x, og_y, c_);
	float height = c_[og_y * 400 + og_x];

	m_verts.Add(FVector(x*spacing_ , y * spacing_, (cal_height * spacing_) / m_));
	m_norms.Add(FVector(0.0f, 0.0f, 1.0f));
	m_u_vs.Add(FVector2D(x , y));
	m_vert_colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	m_tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
}

void AMyProceduralMesh::GenerateTrackVerts(const TArray<FVector2D>& track_points,const TArray<int32>& c_, const int& m_)
{
	float outer_part_added = 0.5;
	float inner_part_added = 0.25;
	for (int i = 0; i < track_points.Num(); i++){
		float x = track_points[i].X;
		float y = track_points[i].Y;
		int og_x = track_points[i].X;
		int og_y = track_points[i].Y;
		for (int j = 0; j < 4; j++){
			for (int index_ = 0; index_ < 4; index_++) {
				AddMultiVerts(x, y, c_, m_,og_x,og_y);
				CoordAdjuster(x, y, index_, inner_part_added);
			}
			CoordAdjuster(x, y, j, outer_part_added);
		}
	}
}
void AMyProceduralMesh::GenerateTrackTris()
{
	m_tris.Add(count);
	m_tris.Add(count+2);
	m_tris.Add(count+1);
	m_tris.Add(count+1);
	m_tris.Add(count+2);
	m_tris.Add(count+3);
	count += 4;
}

void AMyProceduralMesh::CreateTrack(const TArray<FVector2D>& track_points, const TArray<int32>& c_, const int& m_)
{
	ClearMeshData();
	GenerateTrackVerts(track_points, c_, m_);
	//Function that creates mesh section
	procedural_mesh_comp->CreateMeshSection_LinearColor(0, m_verts, m_tris, m_norms, m_u_vs, m_vert_colors, m_tangents, false);
}

void AMyProceduralMesh::GenerateVerts(){
	float uv_spacing = 1.0f / FMath::Max(height_, width_);

	for (int32 y = 0; y < height_; y++){
		for (int32 x = 0; x < width_; x++){
			m_verts.Add(FVector(x * spacing_, y * spacing_, 0.0f));
			m_norms.Add(FVector(0.0f, 0.0f, 1.0f));
			m_u_vs.Add(FVector2D(x * uv_spacing, y * uv_spacing));
			m_vert_colors.Add(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
			m_tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
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
	spacing_ = d_spacing_;
	ClearMeshData();
	GenerateVerts();
	GenerateTris();
	//Function that creates mesh section
	procedural_mesh_comp->CreateMeshSection_LinearColor(0, m_verts, m_tris, m_norms, m_u_vs, m_vert_colors, m_tangents, false);
}

void AMyProceduralMesh::ModiVerts(const TArray<int32>& c_, const int& m_)
{

	for (int32 y = 0; y < height_; y++) {
		for (int32 x = 0; x < width_; x++) {
			m_verts[y * height_ + x].Z = (c_[y * height_ + x]* spacing_)/m_;
		}
	}
	procedural_mesh_comp->UpdateMeshSection_LinearColor(0, m_verts, m_norms, m_u_vs, m_vert_colors, m_tangents);
}


