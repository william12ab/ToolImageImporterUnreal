// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "../../../Unreal/UE_4.26/Engine/Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "MyProceduralMesh.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API AMyProceduralMesh : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, Category = "MyProceduralMesh")
		UProceduralMeshComponent* procedural_mesh_comp;
public:
	// Sets default values for this actor's properties
	AMyProceduralMesh();

	UPROPERTY()
		TArray<FVector> m_verts;
	UPROPERTY()
		TArray<FVector> m_norms;
	UPROPERTY()
		TArray<int32> m_tris;
	UPROPERTY()
		TArray<FVector2D> m_u_vs;
	UPROPERTY()
		TArray<FLinearColor> m_vert_colors;
	UPROPERTY()
		TArray<FProcMeshTangent> m_tangents;

	void GenerateVerts();
	void GenerateTris();
	void ClearMeshData();
	void CreateMesh(int& d_height_, int& d_width_, float& d_spacing_);

	void ModiVerts();		//changing the vertss

	void SetAr(TArray<int32> t) { c_ = t; }

	TArray<int32> ReadFileInfo(const FString& name_);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int32 height_;
	int32 width_;
	float spacing_;
	TArray<int32> c_;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
