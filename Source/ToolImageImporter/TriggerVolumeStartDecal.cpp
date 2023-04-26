// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerVolumeStartDecal.h"
#include "DrawDebugHelpers.h"

ATriggerVolumeStartDecal::ATriggerVolumeStartDecal() {
	SetActorScale3D(FVector(5,5, 5));
	OnActorBeginOverlap.AddDynamic(this, &ATriggerVolumeStartDecal::OnOverlapBegin);

}

void ATriggerVolumeStartDecal::BeginPlay() {
	Super::BeginPlay();
}

void ATriggerVolumeStartDecal::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor){
}