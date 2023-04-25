// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerVolumeStartDecal.h"
#include "DrawDebugHelpers.h"

ATriggerVolumeStartDecal::ATriggerVolumeStartDecal() {
	SetActorScale3D(FVector(5,5, 5));
	OnActorBeginOverlap.AddDynamic(this, &ATriggerVolumeStartDecal::OnOverlapBegin);
	
}

void ATriggerVolumeStartDecal::BeginPlay() {
	Super::BeginPlay();
	DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale() * 10, FColor::Cyan, true, -1, 0, 5);

}

void ATriggerVolumeStartDecal::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor){
    if (OtherActor && (OtherActor != this)) {
        // print to screen using above defined method when actor enters trigger volume
		auto name_ = OtherActor->GetName();
		auto this_name = OverlappedActor->GetName();
		UE_LOG(LogTemp, Warning, TEXT("size: %s"), *name_);
		UE_LOG(LogTemp, Warning, TEXT("size: %s"), *this_name);
    }
}