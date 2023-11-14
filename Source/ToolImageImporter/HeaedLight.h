#pragma once
#include "GameFramework/Actor.h"
#include "Components/SpotLightComponent.h"
#include "HeaedLight.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API AHeaedLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeaedLight();
	void TurnOn(const bool& b_);


	UPROPERTY(VisibleAnywhere, Category = "Head Light")
		class USpotLightComponent* spot_light_comp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float intensity_l;
	float atten_r;
	float inner_c_a;
	float outer_c_a;
};
