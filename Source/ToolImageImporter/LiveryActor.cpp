// Fill out your copyright notice in the Description page of Project Settings.


#include "LiveryActor.h"
ALiveryActor::ALiveryActor() {

}

void ALiveryActor::Init(FVector loc_, FVector scale_, FRotator rot_, FString name_) {
	FString conststring = "Material'/Game/Materials/decal/";
	conststring = conststring + name_;
	conststring = conststring + "'";

	material_interface = LoadObject<UMaterialInterface>(NULL, *conststring);
	SetDecalMaterial(material_interface);
	SetActorScale3D(scale_);
	SetActorRelativeLocation(loc_);
	SetActorRelativeRotation(rot_);
	//SetActorRotation(rot_);
}


void ALiveryActor::BeginPlay() {
	Super::BeginPlay();

}