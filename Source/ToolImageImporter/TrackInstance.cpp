// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackInstance.h"

// Sets default values
ATrackInstance::ATrackInstance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	instanced_mesh_comp = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMeshComp"));
	SetRootComponent(instanced_mesh_comp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	
	instanced_mesh_comp->SetStaticMesh(MeshAsset.Object);
}


// Called when the game starts or when spawned
void ATrackInstance::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrackInstance::AddTrackComp(const FTransform& transform_)
{
	instanced_mesh_comp->AddInstance(transform_);
}

// Called every frame
void ATrackInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

