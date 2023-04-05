// Fill out your copyright notice in the Description page of Project Settings.


#include "CarGameMode.h"
#include "VehicleController.h"


ACarGameMode::ACarGameMode()
{
	DefaultPawnClass = AVehicleController::StaticClass();
}
