// Fill out your copyright notice in the Description page of Project Settings.
#include "VehicleFrontWheel.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
UVehicleFrontWheel::UVehicleFrontWheel(){
	ShapeRadius = 39.f;//32.f;
	ShapeWidth = 15.5f;//12.50f;
	bAffectedByHandbrake = false;
	SteerAngle = 70.f;
	LongStiffValue = 4000.0f;//fast	er acc, higher top speed, default =1000
	LatStiffMaxLoad = 15.f;//smaller harder turning, default = 2
	LatStiffValue = 1000.f;//lower more sliding, default =17
	SuspensionMaxDrop = 15;//higher for maxdrop and raise, wheels can go above and below natural resting point much more, default =10
	SuspensionMaxRaise = 15;
	SuspensionDampingRatio = .98f;//default = 1.0, car body moves about less
	SuspensionNaturalFrequency = 10.0f;//default 7, higher less movement 
	MaxBrakeTorque = 4000.f;
	MaxHandBrakeTorque = 6000.f;
	static ConstructorHelpers::FObjectFinder<UTireConfig> TireConfigObj(TEXT("TireConfig'/Game/DA_tire_config.DA_tire_config'"));
	if (TireConfigObj.Succeeded()) {
		TireConfig = TireConfigObj.Object;
	}
	Mass = 9.f;
	
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS
