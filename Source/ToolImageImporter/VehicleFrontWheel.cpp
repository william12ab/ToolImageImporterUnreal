// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleFrontWheel.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
UVehicleFrontWheel::UVehicleFrontWheel()
{
	ShapeRadius = 60.f;//30
	ShapeWidth = 36.0f;//18
	bAffectedByHandbrake = false;
	SteerAngle = 45.f;
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS
