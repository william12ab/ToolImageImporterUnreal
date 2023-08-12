// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VehicleAnimInstance.h"
#include "MyVehicleAnimInstance.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API UMyVehicleAnimInstance : public UVehicleAnimInstance
{
	
	GENERATED_BODY()
public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Angle)
		float steer_angle;


};
