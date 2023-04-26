#include "TriggerBoxDecal.h"

ATriggerBoxDecal::ATriggerBoxDecal(){
    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &ATriggerBoxDecal::OnOverlapBegin);
    SetActorScale3D(FVector(1, 10, 1));
}

void ATriggerBoxDecal::BeginPlay(){
    Super::BeginPlay();
}

void ATriggerBoxDecal::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor){
}

