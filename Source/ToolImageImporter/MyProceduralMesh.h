// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "../../../Unreal/UE_4.26/Engine/Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "MyProceduralMesh.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API AMyProceduralMesh : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	AMyProceduralMesh();


	UPROPERTY(VisibleAnywhere, Category = "MyProceduralMesh")
		UProceduralMeshComponent* procedural_mesh_comp;

	UPROPERTY(VisibleAnywhere)
		UMaterialInterface* material_interface;
	UPROPERTY(VisibleAnywhere)
		UMaterialInstanceDynamic* material_instance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* ScnComponent;
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
	void SetMaterial(UTexture *t_);
	void ModiVerts(TArray<float>& c_,const int& m_);		//changing the vertss
	void SmoothTerrain(TArray<float>& c_);
	void CalculateNormals();
	void SetHeightProper(const TArray<FVector>& points_, const TArray<FVector>& verts_);
	void ChangeVert(const float& x_pos, const float& y_pos, const float& z_pos);
	void ReplaceC(TArray<float>& c_);
	virtual void PostInitializeComponents() override;

	void Resize(const TArray<FVector>& m_verts_, const int& scale_, const TArray<FLinearColor>& temp_colour);
	void NearestNeighbourSample(const int& grid_size, const int& new_size, const TArray<FVector>& m_verts_, TArray<FVector>& temp_vec, const int& scale, const TArray<FLinearColor>& temp_colour, TArray<FLinearColor>& new_c);
	void Save(TArray<FVector> &temp_, TArray<FLinearColor> &temp_colours);


	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float FindT(const FVector2D& p1, const FVector2D& p2, const FVector2D& p3);
	int32 height_;
	int32 width_;
	float spacing_;
	int count;
};
