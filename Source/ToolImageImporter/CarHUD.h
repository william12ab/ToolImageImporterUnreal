#pragma once
#include "GameFramework/HUD.h"
#include "CarHUD.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API ACarHUD : public AHUD
{
	GENERATED_BODY()
public:
	ACarHUD();

	/** Font used to render the vehicle info */
	UPROPERTY()
		UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};