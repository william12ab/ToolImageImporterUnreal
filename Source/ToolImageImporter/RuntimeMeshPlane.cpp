// Fill out your copyright notice in the Description page of Project Settings
#include "RuntimeMeshPlane.h"
#include "RuntimeMeshComponent.h"
#include "Providers/RuntimeMeshProviderStatic.h"
// Sets default values
ARuntimeMeshPlane::ARuntimeMeshPlane(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;
}

// Called when the game starts or when spawned
void ARuntimeMeshPlane::BeginPlay(){
	Super::BeginPlay();
	RuntimeMeshComponent = NewObject<URuntimeMeshComponent>(this, "TestRMC");
	RuntimeMeshComponent->SetupAttachment(RootComp);
	RuntimeMeshComponent->RegisterComponent();

	URuntimeMeshProviderStatic* StaticProvider = NewObject<URuntimeMeshProviderStatic>(this, TEXT("StaticProvider"));
	RuntimeMeshComponent->Initialize(StaticProvider);

	StaticProvider->SetupMaterialSlot(0, TEXT("Material"), nullptr);

	TArray<FVector> Positions{ FVector(0, -50, 0), FVector(0, 0, 100), FVector(0, 50, 0) };

	// This creates 3 vertex colors
	TArray<FColor> Colors{ FColor::Blue, FColor::Red, FColor::Green };

	// This indexes our simple triangle
	TArray<int32> Triangles = { 0, 1, 2 };

	TArray<FVector> EmptyNormals;
	TArray<FVector2D> EmptyTexCoords;
	TArray<FRuntimeMeshTangent> EmptyTangents;
	StaticProvider->CreateSectionFromComponents(0, 0, 0, Positions, Triangles, EmptyNormals, EmptyTexCoords, Colors, EmptyTangents, ERuntimeMeshUpdateFrequency::Infrequent, true);

}

// Called every frame
void ARuntimeMeshPlane::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

}

