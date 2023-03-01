// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterMesh.h"

// Sets default values
AWaterMesh::AWaterMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ScnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene"));
	SetRootComponent(ScnComponent);
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	StaticMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AWaterMesh::BeginPlay()
{
	Super::BeginPlay();

	material_interface = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/WaterMaterials/Materials/M_Water_Opaque.M_Water_Opaque'"));
	material_instance = UMaterialInstanceDynamic::Create(material_interface, this);
	StaticMeshComponent->SetMaterial(0, material_instance);
}


