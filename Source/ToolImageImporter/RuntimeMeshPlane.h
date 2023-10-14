// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "RuntimeMeshPlane.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API ARuntimeMeshPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuntimeMeshPlane();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		URuntimeMeshComponent* RuntimeMeshComponent;

	UPROPERTY(VisibleAnywhere)
		UMaterialInterface* material_interface;
	UPROPERTY(VisibleAnywhere)
		UMaterialInstanceDynamic* material_instance;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
