// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/Actor.h"
#include "BasicTree.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API ABasicTree : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere)
		UInstancedStaticMeshComponent* instanced_basic_tree;


public:	
	// Sets default values for this actor's properties
	ABasicTree();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* ScnComponent;

	void AddBasicTree(const FTransform& transform_, const int& tree_, const FString& name_attachment_);

	void AddClusterTrees(const TArray<FVector> &m_verts, const int& max_, const int& min_, const TArray<FVector2D>& track_point, const bool& is_foliage);
	void AddName(const FString& name_attachment_, const int& tree_, FString& name_);
	bool CheckBounds(const TArray<FVector2D>& track_point, int& point_x, int&point_y);

	void AddRockClusters(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts);
	void AddGrass(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts, const float& max, const float& min);


	void NameChoiceTree(FString& mesh_name, int& tree_select);
	void NameChoicePlant(FString& mesh_name, float& z_alter);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

};
