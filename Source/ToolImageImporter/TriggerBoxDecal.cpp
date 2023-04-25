// Fill out your copyright notice in the Description page of Project Settings.

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))
#include "TriggerBoxDecal.h"
#include "DrawDebugHelpers.h"

ATriggerBoxDecal::ATriggerBoxDecal(){
    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &ATriggerBoxDecal::OnOverlapBegin);
    SetActorScale3D(FVector(1, 10, 1));
}

void ATriggerBoxDecal::BeginPlay(){
    Super::BeginPlay();
}

void ATriggerBoxDecal::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor){
    // check if Actors do not equal nullptr and that 
    if (OtherActor && (OtherActor != this)) {
        // print to screen using above defined method when actor enters trigger box
        print("Overlap Begin");
        printFString("Overlapped Actor = %s", *OverlappedActor->GetName());
    }
}

