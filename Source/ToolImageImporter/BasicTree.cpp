// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTree.h"

// Sets default values
ABasicTree::ABasicTree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	instanced_basic_tree = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BasicInstancedTree"));
	SetRootComponent(instanced_basic_tree);
}

// Called when the game starts or when spawned
void ABasicTree::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasicTree::AddBasicTree(const FTransform& transform_)
{
	int tree_select = FMath::RandRange(0, 3);
	FString name_;
	switch (tree_select){
	case 0:{
		name_.Append("SM_Pine_Tree_1.SM_Pine_Tree_1'");
		break;
	}
	case 1:{
		name_.Append("SM_Pine_Tree_2.SM_Pine_Tree_2'");
		break;
	}
	case 2: {
		name_.Append("SM_Pine_Tree_3.SM_Pine_Tree_3'");
		break;
	}
	case 3: {
		name_.Append("SM_Pine_Tree_4.SM_Pine_Tree_4'");
		break;
	}
	}
	FString fullname_;
	fullname_.Append("StaticMesh'/Game/Stylized_PBR_Nature/Foliage/Assets/");
	fullname_.Append(name_);
	UStaticMesh* meshToUse = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *fullname_));
	if (meshToUse && instanced_basic_tree)
	{
		instanced_basic_tree->SetStaticMesh(meshToUse);
		UE_LOG(LogTemp, Warning, TEXT("n: %d"), tree_select);
	}
	instanced_basic_tree->AddInstance(transform_);
}
