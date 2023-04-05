// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleFrontWheel.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
UVehicleFrontWheel::UVehicleFrontWheel()
{
	ShapeRadius = 30.f;
	ShapeWidth = 18.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 45.f;
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS
