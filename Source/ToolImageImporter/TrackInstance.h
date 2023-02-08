// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TrackInstance.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API ATrackInstance : public AActor
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere)
		UInstancedStaticMeshComponent* instanced_mesh_comp;
		
public:	
	// Sets default values for this actor's properties
	ATrackInstance();
	void AddTrackComp(const FTransform& transform_);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
