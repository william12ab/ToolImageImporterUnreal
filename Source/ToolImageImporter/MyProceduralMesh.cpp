// Fill out your copyright notice in the Description page of Project Settings.
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "MyProceduralMesh.h"
#include <Runtime/Engine/Public/ImageUtils.h>

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
}

TArray<int32> AMyProceduralMesh::ReadFileInfo(const FString& name_)
{
	TArray<int32> m_colors;

	FString FilePath = "F:/this one looks good/track_image.png";
	UE_LOG(LogTemp, Warning, TEXT("The path's name is %s"), *FilePath);
	UE_LOG(LogTemp, Warning, TEXT("The path's name is %s"), *name_);
	UTexture2D* texture_ = FImageUtils::ImportFileAsTexture2D(FilePath);

	texture_->AddToRoot();
	//height_ = 100;
	//width_ = 100;
	//spacing_ = 1;

	//ClearMeshData();
	//GenerateVerts();
	//GenerateTris();


	//since grey scale already. each rgb component should be greyscale value. therefore no need to add up and divide by 3.
	//rgb comp is uint8 value. so, use this as the height and modifiy the height of terrain.
	const FColor* formated_image_data = static_cast<const FColor*>(texture_->PlatformData->Mips[0].BulkData.LockReadOnly());
	UE_LOG(LogTemp, Warning, TEXT("The Size is %d"), texture_->PlatformData->Mips[0].SizeY);

	for (int32 y_ = 0; y_ < texture_->PlatformData->Mips[0].SizeY; y_++) {
		for (int32 x_ = 0; x_ < texture_->PlatformData->Mips[0].SizeX; x_++) {
			FColor pixel_color = formated_image_data[y_ * texture_->GetSizeX() + x_]; // Do the job with the pixel
			//m_verts[y_ * height_ + x_].Z = pixel_color.R;
			UE_LOG(LogTemp, Warning, TEXT("The path's name is %d"), pixel_color.R);
			m_colors.Add(pixel_color.R);
		}
	}
	texture_->PlatformData->Mips[0].BulkData.Unlock();
	//procedural_mesh_comp->UpdateMeshSection_LinearColor(0, m_verts, m_norms, m_u_vs, m_vert_colors, m_tangents);

	return m_colors;
}

