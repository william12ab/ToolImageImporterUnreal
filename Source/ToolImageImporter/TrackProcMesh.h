// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../Unreal/UE_4.26/Engine/Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "TrackProcMesh.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API ATrackProcMesh : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere, Category = "MyProceduralMesh")
		UProceduralMeshComponent* track_mesh_comp;
public:	
	// Sets default values for this actor's properties
	ATrackProcMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
