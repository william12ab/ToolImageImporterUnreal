#pragma once
#include "Engine/TriggerBox.h"
#include "ControlPointTriggerBox.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API AControlPointTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	AControlPointTriggerBox();
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
};
