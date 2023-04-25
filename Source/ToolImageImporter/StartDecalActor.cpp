// Fill out your copyright notice in the Description page of Project Settings.
#include "StartDecalActor.h"

AStartDecalActor::AStartDecalActor() {
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatInter(TEXT("Material'/Game/Materials/StartFinishMat.StartFinishMat'"));
	material_interface = MatInter.Object;
	SetDecalMaterial(material_interface);
	SetActorScale3D(FVector(1.f, 3.f, 1.f));

	
}


void AStartDecalActor::SetPosition(const FVector& pos) {
	SetActorLocation(pos);
}

