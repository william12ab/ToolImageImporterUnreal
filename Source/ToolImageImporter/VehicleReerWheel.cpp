// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleReerWheel.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS

UVehicleReerWheel::UVehicleReerWheel()
{
	ShapeRadius = 60.f;//30
	ShapeWidth = 36.0f;//18
	bAffectedByHandbrake = true;
	SteerAngle = 20.f;
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS