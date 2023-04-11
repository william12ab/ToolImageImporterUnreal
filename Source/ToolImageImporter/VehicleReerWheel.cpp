// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleReerWheel.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS

UVehicleReerWheel::UVehicleReerWheel()
{
	ShapeRadius = 39.f;//30
	ShapeWidth = 19.0f;//18
	bAffectedByHandbrake = true;
	SteerAngle = 0.f;
	LongStiffValue=1500.0f;
	SuspensionMaxDrop = 10;
	SuspensionMaxRaise = 10;
	LatStiffMaxLoad = 5.f;
	LatStiffValue = 10.f;
	SuspensionDampingRatio = .5f;
	SuspensionNaturalFrequency = 4.f;
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS