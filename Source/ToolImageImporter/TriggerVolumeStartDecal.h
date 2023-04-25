// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerVolume.h"
#include "TriggerVolumeStartDecal.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API ATriggerVolumeStartDecal : public ATriggerVolume
{
	GENERATED_BODY()
public:
	ATriggerVolumeStartDecal();

	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

protected:
	virtual void BeginPlay() override;
};
