// Fill out your copyright notice in the Description page of Project Settings.
#include "ControlPointTriggerBox.h"

AControlPointTriggerBox::AControlPointTriggerBox() {
    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &AControlPointTriggerBox::OnOverlapBegin);
    SetActorScale3D(FVector(10, 20, 10));//change this
}

void AControlPointTriggerBox::BeginPlay() {
    Super::BeginPlay();
}

void AControlPointTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor) {

}

