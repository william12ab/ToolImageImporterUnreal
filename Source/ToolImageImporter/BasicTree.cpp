// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTree.h"

// Sets default values
ABasicTree::ABasicTree(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	instanced_basic_tree = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BasicInstancedTree"));
	SetRootComponent(instanced_basic_tree);
}
// Called when the game starts or when spawned
void ABasicTree::BeginPlay(){
	Super::BeginPlay();
	
}

//change random range and the string part of addbasictree to change tree type
void ABasicTree::AddClusterTrees(const TArray<FVector>& m_verts, const TArray<FVector2D> &track_point, const int&max_, const int&min_){

	FString mesh_name = "SM_Pine_Tree_";
	int range_start = 1;
	int range_end = 4;

	float max_m = max_;
	float min_m = min_;

	for (int a = 0; a < 1; a++){
		int rand_name = FMath::RandRange(0, 1);
		if (rand_name == 1) {
			mesh_name = "SM_Pine_Tree_";
			range_start = 1;
			range_end = 4;
		}
		else{
			mesh_name = "SM_Common_Tree_";
			range_start = 1;
			range_end = 11;
		}
		int tree_select = FMath::RandRange(range_start, range_end);
		for (int i = 0; i < 25; i++) {
			int pos_y = FMath::RandRange(1, 300);
			int pos_x = FMath::RandRange(1, 300);
			float z_pos = m_verts[pos_y * 400 + pos_x].Z;


			if (m_verts[pos_y * 400 + pos_x].Z<(max_m-(max_m * 0.30f)) && m_verts[pos_y * 400 + pos_x].Z>(min_+ (max_m * 0.20f))) {
				//place point
				FTransform A{
					FRotator{0,0,0},
					FVector{(float)pos_x * 20.0f, (float)pos_y * 20.0f, z_pos },
					FVector{0.250f, 0.250f, 0.250f}};	//Scale
				AddBasicTree(A, tree_select, mesh_name);
			}
			else{
				i--;
			}
		}
	}
}

void ABasicTree::AddName(const FString& name_attachment_,const int&tree_, FString& name_){
	if (name_attachment_ == "SM_Common_Tree_") {
		name_.Append(name_attachment_);
		if (tree_ < 10) {
			name_.AppendInt(0);
			name_.AppendInt(tree_);
		}
		else {
			name_.AppendInt(tree_);
		}
	}
	else {
		name_.Append(name_attachment_);
		name_.AppendInt(tree_);
	}
}

void ABasicTree::AddBasicTree(const FTransform& transform_, const int& tree_, const FString& name_attachment_){
	FString name_;
	name_.Append("StaticMesh'/Game/Stylized_PBR_Nature/Foliage/Assets/");
	AddName(name_attachment_, tree_, name_);
	name_.Append(".");
	AddName(name_attachment_, tree_, name_);
	name_.Append("'");
	UStaticMesh* meshToUse = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *name_));
	if (meshToUse && instanced_basic_tree){
		instanced_basic_tree->SetStaticMesh(meshToUse);
	}
	instanced_basic_tree->AddInstance(transform_);
}
