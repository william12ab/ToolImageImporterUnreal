// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleFrontWheel.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
UVehicleFrontWheel::UVehicleFrontWheel()
{
	ShapeRadius = 38.5f;//30
	ShapeWidth = 18.0f;//18
	bAffectedByHandbrake = false;
	SteerAngle = 65.f;
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS
