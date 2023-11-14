// Fill out your copyright notice in the Description page of Project Settings.
#include "HeaedLight.h"

// Sets default values
AHeaedLight::AHeaedLight(){
	PrimaryActorTick.bCanEverTick = true;

	intensity_l = 1500.f;
	atten_r = 3000.f;
	inner_c_a = 28.f;
	outer_c_a = 44.f;
	spot_light_comp = CreateDefaultSubobject<USpotLightComponent>(TEXT("spotlight"));
	spot_light_comp->SetIntensityUnits(ELightUnits::Candelas);
	spot_light_comp->SetOuterConeAngle(outer_c_a);
	spot_light_comp->SetInnerConeAngle(inner_c_a);
	spot_light_comp->SetIntensity(intensity_l);
	spot_light_comp->SetAttenuationRadius(atten_r);
	
	spot_light_comp->SetVisibility(false);

	RootComponent = spot_light_comp;

	//then attach to vhicle controlelr
}

// Called when the game starts or when spawned
void AHeaedLight::BeginPlay(){
	Super::BeginPlay();
	
}

void AHeaedLight::TurnOn(const bool& b_) {
	spot_light_comp->SetVisibility(b_);
}
