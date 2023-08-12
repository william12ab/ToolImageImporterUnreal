// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleReerWheel.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
UVehicleReerWheel::UVehicleReerWheel(){
	ShapeRadius = 39.f;//32.f;
	ShapeWidth = 15.5f;//12.50f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.f;
	LongStiffValue=4000.0f;
	SuspensionMaxDrop = 15;
	SuspensionMaxRaise = 15;
	LatStiffMaxLoad = 15.f;
	LatStiffValue = 200.f;
	SuspensionDampingRatio = .98f;
	SuspensionNaturalFrequency = 10.f;
	MaxBrakeTorque = 4000.f;
	MaxHandBrakeTorque = 6000.f;
	static ConstructorHelpers::FObjectFinder<UTireConfig> TireConfigObj(TEXT("TireConfig'/Game/DA_tire_config.DA_tire_config'"));
	if (TireConfigObj.Succeeded()) {
		TireConfig = TireConfigObj.Object;
	}
	
	Mass = 9.f;
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS