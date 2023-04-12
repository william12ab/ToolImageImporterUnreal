// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleFrontWheel.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
UVehicleFrontWheel::UVehicleFrontWheel()
{
	ShapeRadius = 39.f;//30
	ShapeWidth = 19.0f;//18
	bAffectedByHandbrake = false;
	SteerAngle = 70.f;
	LongStiffValue = 2000.0f;//faster acc, higher top speed, default =1000
	LatStiffMaxLoad = 5.f;//smaller harder turning, default = 2
	LatStiffValue = 17.f;//lower more sliding, default =17
	SuspensionMaxDrop = 10;//higher for maxdrop and raise, wheels can go above and below natural resting point much more, default =10
	SuspensionMaxRaise = 10;
	SuspensionDampingRatio = 0.8f;//default = 1.0, car body moves about less
	SuspensionNaturalFrequency = 4.f;//default 7, higher less movement 

	static ConstructorHelpers::FObjectFinder<UTireConfig> TireConfigObj(TEXT("TireConfig'/Game/DA_tire_config.DA_tire_config'"));
	if (TireConfigObj.Succeeded()) {
		TireConfig = TireConfigObj.Object;
		UE_LOG(LogTemp, Warning, TEXT("succ"));
	}
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS
