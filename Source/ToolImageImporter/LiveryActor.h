// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "Engine/DecalActor.h"
#include "LiveryActor.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API ALiveryActor : public ADecalActor
{
	GENERATED_BODY()
public:
	ALiveryActor();
	virtual void BeginPlay() override;
	void Init(FVector loc_, FVector scale_, FRotator rot_, FString name_);
	UMaterialInterface* material_interface;
};
