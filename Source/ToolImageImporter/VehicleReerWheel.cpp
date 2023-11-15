// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleReerWheel.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
UVehicleReerWheel::UVehicleReerWheel(){
	ShapeRadius = 39.f;//39.f;
	ShapeWidth = 16.5f;//15.50f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.f;
	LongStiffValue= 4000.0f;
	SuspensionMaxDrop = 19;
	SuspensionMaxRaise = 19;
	LatStiffMaxLoad = 2.f;
	LatStiffValue = 200.f;
	SuspensionDampingRatio = 0.7f;
	SuspensionNaturalFrequency = 7;
	MaxBrakeTorque = 2000.f;
	MaxHandBrakeTorque = 4000.f;
	static ConstructorHelpers::FObjectFinder<UTireConfig> TireConfigObj(TEXT("TireConfig'/Game/DA_tire_config.DA_tire_config'"));
	if (TireConfigObj.Succeeded()) {
		TireConfig = TireConfigObj.Object;
	}
	
	Mass = 35.f;
	DampingRate = .15;
	bAutoAdjustCollisionSize = true;
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS