// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleReerWheel.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
UVehicleReerWheel::UVehicleReerWheel()
{
	ShapeRadius = 39.f;
	ShapeWidth = 19.0f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.f;
	LongStiffValue=1000.0f;
	SuspensionMaxDrop = 20;
	SuspensionMaxRaise = 20;
	LatStiffMaxLoad = 15.f;
	LatStiffValue = 200.f;
	SuspensionDampingRatio =1.1f;
	SuspensionNaturalFrequency = 10.f;
	
	MaxBrakeTorque = 4000.f;
	MaxHandBrakeTorque = 6000.f;

	static ConstructorHelpers::FObjectFinder<UTireConfig> TireConfigObj(TEXT("TireConfig'/Game/DA_tire_config.DA_tire_config'"));
	if (TireConfigObj.Succeeded()) {
		TireConfig = TireConfigObj.Object;
		UE_LOG(LogTemp, Warning, TEXT("succ"));
	}
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS