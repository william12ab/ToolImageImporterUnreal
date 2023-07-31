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
		TArray<FVector> m_verts1;
	UPROPERTY()
		TArray<FVector> m_verts2;
	UPROPERTY()
		TArray<FVector> m_verts3;
	UPROPERTY()
		TArray<FVector> m_verts_total;
	UPROPERTY()
		TArray<FVector> m_norms;
	UPROPERTY()
		TArray<FVector> m_norms1;
	UPROPERTY()
		TArray<FVector> m_norms2;
	UPROPERTY()
		TArray<FVector> m_norms3;
	UPROPERTY()
		TArray<FVector> m_norms_total;
	UPROPERTY()
		TArray<int32> m_tris;
	UPROPERTY()
		TArray<int32> m_tris1;
	UPROPERTY()
		TArray<int32> m_tris2;
	UPROPERTY()
		TArray<int32> m_tris3;
	UPROPERTY()
		TArray<int32> m_tris_total;
	UPROPERTY()
		TArray<FVector2D> m_u_vs;
	UPROPERTY()
		TArray<FVector2D> m_u_vs_total;
	UPROPERTY()
		TArray<FLinearColor> m_vert_colors;
	UPROPERTY()
		TArray<FLinearColor> m_vert_colors1;
	UPROPERTY()
		TArray<FLinearColor> m_vert_colors2;
	UPROPERTY()
		TArray<FLinearColor> m_vert_colors3;
	UPROPERTY()
		TArray<FLinearColor> m_vert_colors_total;
	UPROPERTY()
		TArray<FProcMeshTangent> m_tangents;
	UPROPERTY()
		TArray<FProcMeshTangent> m_tangents_total;

	void GenerateVerts(const int&index_);
	void AddVert(const int& index_, const int& x, const int& y);
	void GenerateTris(const int & index_);
	void ClearMeshData();
	void CreateMesh(int& d_height_, int& d_width_, float& d_spacing_, const int&index_);
	void SetMaterial(UTexture *t_);
	void ModiVerts(TArray<float>& c_,const int& m_, const int& index_);		//changing the vertss
	void SmoothTerrain(TArray<float>& c_);
	void CalculateNormals(const int& index_);
	void SetHeightProper(const TArray<FVector>& points_, const TArray<FVector>& verts_, const int& index_);
	void ChangeVert(const float& x_pos, const float& y_pos, const float& z_pos, const int& index_);
	void ReplaceC(TArray<float>& c_, const int& index_);
	virtual void PostInitializeComponents() override;

	void Resize(const TArray<FVector>& m_verts_, const int& scale_, const TArray<FLinearColor>& temp_colour, const int& index_);
	void NearestNeighbourSample(const int& grid_size, const int& new_size, const TArray<FVector>& m_verts_, TArray<FVector>& temp_vec, const int& scale, const TArray<FLinearColor>& temp_colour, TArray<FLinearColor>& new_c);
	void Save(TArray<FVector> &temp_, TArray<FLinearColor> &temp_colours);

	void ChangePos(int&pos,const int&max,bool & local_is_changed);
	
	void TestFinal();

	void FullSize();
	void SetIsChunking(bool f) { is_chunking = f; }
	void SetIsTemp(bool f) { is_temp = f; }
	TArray<TArray<FVector>> vec_m_verts;
	TArray<TArray<FVector>> vec_m_norms;
	TArray<TArray<int32>> vec_m_tris;
	TArray<TArray<FLinearColor>> vec_m_vert_colors;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float FindT(const FVector2D& p1, const FVector2D& p2, const FVector2D& p3);
	int32 height_;
	int32 width_;
	float spacing_;
	int count;
	bool is_created;
	bool is_chunking;
	TArray<float> z_axis;
	TArray<FLinearColor> temp_colors;
	bool is_temp;
};
