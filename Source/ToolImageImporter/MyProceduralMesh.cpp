// Fill out your copyright notice in the Description page of Project Settings.
#include "MyProceduralMesh.h"

// Sets default values
AMyProceduralMesh::AMyProceduralMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	procedural_mesh_comp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	procedural_mesh_comp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	width_ = 4;
	height_ = 4;
	spacing_ = 50.0f;


}

// Called when the game starts or when spawned
void AMyProceduralMesh::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyProceduralMesh::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
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


void AMyProceduralMesh::ModiVerts()
{
	for (int32 y = 0; y < height_; y++) {
		for (int32 x = 0; x < width_; x++) {
			m_verts[y * height_ + x].Z = rand() % 5 * spacing_;
			//this works when adding but not here?
		}
	}
	procedural_mesh_comp->UpdateMeshSection_LinearColor(0, m_verts, m_norms, m_u_vs, m_vert_colors, m_tangents);
	//procedural_mesh_comp->UpdateMeshSection_LinearColor(0, m_verts, m_tris, m_norms, m_u_vs, m_vert_colors, m_tangents, false);
}
