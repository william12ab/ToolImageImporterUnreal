// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleFrontWheel.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
UVehicleFrontWheel::UVehicleFrontWheel()
{
	ShapeRadius = 39.f;
	ShapeWidth = 19.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 70.f;
	LongStiffValue = 3000.0f;//faster acc, higher top speed, default =1000
	LatStiffMaxLoad = 15.f;//smaller harder turning, default = 2
	LatStiffValue = 1000.f;//lower more sliding, default =17
	SuspensionMaxDrop = 20;//higher for maxdrop and raise, wheels can go above and below natural resting point much more, default =10
	SuspensionMaxRaise = 20;
	SuspensionDampingRatio = 1.1f;//default = 1.0, car body moves about less
	SuspensionNaturalFrequency = 10.f;//default 7, higher less movement 
	MaxBrakeTorque = 4000.f;
	MaxHandBrakeTorque = 6000.f;
	static ConstructorHelpers::FObjectFinder<UTireConfig> TireConfigObj(TEXT("TireConfig'/Game/DA_tire_config.DA_tire_config'"));
	if (TireConfigObj.Succeeded()) {
		TireConfig = TireConfigObj.Object;
		UE_LOG(LogTemp, Warning, TEXT("succ"));
	}
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS
