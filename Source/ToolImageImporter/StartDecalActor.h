#pragma once
#include "Engine/DecalActor.h"
#include "StartDecalActor.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API AStartDecalActor : public ADecalActor
{
	GENERATED_BODY()
	
public:
	AStartDecalActor();
	UMaterialInterface* material_interface;
	void SetPosition(const FVector& pos);
};
