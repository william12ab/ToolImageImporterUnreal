// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "BasicTree.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API ABasicTree : public AActor
{
	GENERATED_BODY()
	/*	UPROPERTY(VisibleAnywhere)
		UInstancedStaticMeshComponent* instanced_basic_tree;*/

	UPROPERTY(VisibleAnywhere)
		UHierarchicalInstancedStaticMeshComponent* h_instanced;
public:	
	// Sets default values for this actor's properties
	ABasicTree();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* ScnComponent;
	void AddBasicTree(const FTransform& transform_, const int& tree_, const FString& name_attachment_, const TArray<FVector2D>& track_point_arr, const int& x_pos, const int& y_pos);
	void AddClusterTrees(const TArray<FVector> &m_verts, const int& max_, const int& min_, const TArray<FVector2D>& track_point, const bool& is_foliage);
	void AddName(const FString& name_attachment_, const int& tree_, FString& name_);
	bool CheckBounds(const TArray<FVector2D>& track_point, const int& point_x, const int&point_y);
	void AddTreeNearTrack(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts, const int& max_, const int& min_);
	void AddRockClusters(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts);
	void AddGrass(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts, const float& max, const float& min);
	void AddGrassAtEdge(const TArray<FVector>& m_verts, const TArray<FLinearColor>& m_colors, const int& height_);
	void AddGrassInstance(const int& x, const int& y, const float& z_pos, const int& item);
	void NameChoiceTree(FString& mesh_name, int& tree_select);
	void NameChoicePlant(FString& mesh_name, float& z_alter);
	void SetSpacing(float s_) { spacing_ = s_; }
	void SetDivision(float d_) { division_ = d_; }

	void CheckDistance( UStaticMesh& mesh_);
	bool CheckTrackTree(int& point_x, int& point_y);
	void SetItem(int s) { item_ = s; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	float spacing_;
	float division_;

	TArray<FVector2D> track_tree_points;
	TArray<FVector2D> track_rock_points;
	int index_tracker;
	bool is_track_tree;
	int item_;
};
