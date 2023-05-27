// Fill out your copyright notice in the Description page of Project Settings.


#include "CarHUD.h"
#include "VehicleController.h"
#include "WheeledVehicle.h"
#include "RenderResource.h"
#include "Shader.h"
#include "Engine/Canvas.h"
#include "WheeledVehicleMovementComponent.h"
#include "Engine/Font.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

ACarHUD::ACarHUD(){
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Game/ui_font"));
	HUDFont = Font.Object;
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> puase_back_obj(TEXT("Texture2D'/Game/Textures/pausedbg.pausedbg'"));
	pause_background_tex = puase_back_obj.Object;

	white_.R = 255; white_.G = 250; white_.B = 239; white_.A = 255.f;
	red_.R = 208; red_.G = 34; red_.B = 65; red_.A = 255.f;
	r_unit = FText::FromString("RPM");
	v_unit = FText::FromString("KPH");
}

void ACarHUD::DrawHUD(){
	Super::DrawHUD();

	FVector2D size_;
	GEngine->GameViewport->GetViewportSize(size_);
	// Get our vehicle so we can check if we are in car. If we are we don't want onscreen HUD
	AVehicleController* Vehicle = Cast<AVehicleController>(GetOwningPawn());
	if ((Vehicle != nullptr) ){
		if (Vehicle->GetIsRenderSpedo()) {
			// Gear
			FCanvasTextItem GearTextItem(FVector2D(size_.X * .90f, size_.Y * .85f), Vehicle->GearDisplayString, HUDFont, Vehicle->is_in_reverse_gear == false ? Vehicle->GearDisplayColor : Vehicle->GearDisplayReverseColor);
			Canvas->DrawItem(GearTextItem);
			// Speed
			FCanvasTextItem SpeedTextItem(FVector2D(size_.X * .90f, size_.Y * .88f), Vehicle->SpeedDisplayString, HUDFont, FLinearColor(white_));
			Canvas->DrawItem(SpeedTextItem);
			FCanvasTextItem units_v_text_item(FVector2D(size_.X * .93f, size_.Y * .88f), v_unit, HUDFont, FLinearColor(red_));
			Canvas->DrawItem(units_v_text_item);
			//rpm
			FCanvasTextItem RPMTextItem(FVector2D(size_.X * .90f, size_.Y * .91f), Vehicle->RPMDisplayString, HUDFont, FLinearColor(white_));
			Canvas->DrawItem(RPMTextItem);
			FCanvasTextItem units_r_text_item(FVector2D(size_.X * .95f, size_.Y * .91f), r_unit, HUDFont, FLinearColor(red_));
			Canvas->DrawItem(units_r_text_item);
		}
	}
}

void ACarHUD::DrawPauseMenu(const FVector2D& scale_vec) {	
	const FVector2D pausebg_pos(0,0);

	// draw the crosshair
	FCanvasTileItem TileItem(pausebg_pos, pause_background_tex->Resource, FLinearColor::White);
	
	TileItem.Size = FVector2D(Canvas->SizeX, Canvas->SizeY);
	Canvas->DrawItem(TileItem);
}


