// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/Actor.h"
#include "BasicTree.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API ABasicTree : public AActor
{
	GENERATED_BODY()
		UPROPERTY(VisibleAnywhere)
		UInstancedStaticMeshComponent* instanced_basic_tree;
public:	
	// Sets default values for this actor's properties
	ABasicTree();
	void AddBasicTree(const FTransform& transform_);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

};
