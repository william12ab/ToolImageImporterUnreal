// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackSpline.h"

// Sets default values
ATrackSpline::ATrackSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrackSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrackSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

