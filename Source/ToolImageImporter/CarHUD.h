#pragma once
#include "GameFramework/HUD.h"
#include "Components/Button.h"
#include "Components/Image.h"
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


	void DrawPauseMenu(const FVector2D& scale_vec);

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface

protected:
	class UTexture2D* pause_background_tex;

	FColor white_;
	FColor red_;
	FText r_unit;
	FText v_unit;
};