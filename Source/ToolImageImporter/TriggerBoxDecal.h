// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/TriggerBox.h"
#include "TriggerBoxDecal.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API ATriggerBoxDecal : public ATriggerBox
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ATriggerBoxDecal();
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
};
