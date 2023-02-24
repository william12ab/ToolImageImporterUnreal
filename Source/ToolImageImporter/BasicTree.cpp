// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTree.h"

// Sets default values
ABasicTree::ABasicTree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	instanced_basic_tree = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BasicInstancedTree"));
	SetRootComponent(instanced_basic_tree);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Stylized_PBR_Nature/Foliage/Assets/SM_Pine_Tree_1.SM_Pine_Tree_1'"));
	instanced_basic_tree->SetStaticMesh(MeshAsset.Object);
}

// Called when the game starts or when spawned
void ABasicTree::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABasicTree::AddBasicTree(const FTransform& transform_)
{
	/*UStaticMesh* meshToUse = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/Stylized_PBR_Nature/Foliage/Assets/SM_Pine_Tree_1.SM_Pine_Tree_1'")));
	if (meshToUse && instanced_basic_tree)
	{
		instanced_basic_tree->SetStaticMesh(meshToUse);
		UE_LOG(LogTemp, Warning, TEXT("mesh"));
	}*/
	instanced_basic_tree->AddInstance(transform_);
}
