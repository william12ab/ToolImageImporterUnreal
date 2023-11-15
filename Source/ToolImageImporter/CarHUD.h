#pragma once
#include "GameFramework/HUD.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "PaceNotesActor.h"
#include <Runtime/Engine/Public/CanvasItem.h>
#include <iostream>
#include <chrono>
#include "VehicleController.h"
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
	void ShowNote();
	void DisplayNote(const int& index, const FVector2D& position_, FTexture* texture);
	void ClearNotes();
	void ChangeSpeedo(AVehicleController* Vehicle_);
	void ChangeTacho(AVehicleController* Vehicle_);
	// Begin AHUD interface
	virtual void DrawHUD() override;
	void SwitchStatement(const TArray<int>& array_);
	// End AHUD interface

protected:
	class UTexture2D* pause_background_tex;
	TArray<UTexture2D*>pacenote_images;
	FColor white_;
	FColor red_;
	FText r_unit;
	FText v_unit;
	TArray<FCanvasTileItem> pacenote_items;
	APaceNotesActor* pace_notes_actor;
	TArray<int> extra_array;
	bool is_extra;
	bool is_drawing;
	bool is_timer_started;
	std::chrono::steady_clock::time_point start;
	int notes_displayed;


	class UTexture2D* speedometor_img;
	class UTexture2D* tachometor_img;
	class UTexture2D* dial_img;

	float speed_dial_rot;
	float tacho_dial_rot;
};