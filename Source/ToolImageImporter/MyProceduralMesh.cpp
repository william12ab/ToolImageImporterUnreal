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

void AMyProceduralMesh::AddMultiVerts(float x,float y, const TArray<int32>& c_, const int& m_)
{
	float part_added = 0.5;
	for (int i = 0; i < 4; i++)
	{
		AddVert(x, y, c_, m_);			//bottom left
		AddVert(x + part_added, y, c_, m_);			//bottom right
		AddVert(x, y + part_added, c_, m_);			//top left
		AddVert(x + part_added, y + part_added, c_, m_);			//top right
		GenerateTrackTris();
		if (i==0){
			x += part_added;	//moving to br
		}
		if (i == 1) {			//moving to tl
			x -= part_added;
			y += part_added;
		}
		if (i == 2) {			//to tr
			x += part_added;
		}
	}
}

void AMyProceduralMesh::AddVert(float x, float y, const TArray<int32>& c_, const int& m_)
{
	//(c_[y * 400 + x] * spacing_) / m_
	m_verts.Add(FVector(x*spacing_ , y * spacing_, 0));									//youll need to do something about this
	m_norms.Add(FVector(0.0f, 0.0f, 1.0f));
	m_u_vs.Add(FVector2D(x , y ));
	m_vert_colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	m_tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
}

void AMyProceduralMesh::GenerateTrackVerts(const TArray<FVector2D>& track_points,const TArray<int32>& c_, const int& m_)
{
	for (int i = 0; i < track_points.Num(); i++){
		float x = track_points[i].X;
		float y = track_points[i].Y;

		AddMultiVerts(x, y, c_, m_);
		//AddVert(x, y, c_, m_);			//bottom left
		//AddVert(x + 1, y, c_, m_);			//bottom right
		//AddVert(x, y + 1, c_, m_);			//top left
		//AddVert(x + 1, y + 1, c_, m_);			//top right

	}
	//GenerateTrackTris();
}
void AMyProceduralMesh::GenerateTrackTris()
{
	//count=0;
	//for (size_t i = 0; i < track_points.Num()-1; i++){
		m_tris.Add(count);
		m_tris.Add(count+2);
		m_tris.Add(count+1);

		m_tris.Add(count+1);
		m_tris.Add(count+2);
		m_tris.Add(count+3);
		count += 4;
//	}
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


