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
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = Font.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> puase_back_obj(TEXT("Texture2D'/Game/Textures/pausedbg.pausedbg'"));
	pause_background_tex = puase_back_obj.Object;
	
}

void ACarHUD::DrawHUD(){
	Super::DrawHUD();

	// Calculate ratio from 720p
	const float HUDXRatio = Canvas->SizeX / 1280.f;
	const float HUDYRatio = Canvas->SizeY / 720.f;
	// Get our vehicle so we can check if we are in car. If we are we don't want onscreen HUD
	AVehicleController* Vehicle = Cast<AVehicleController>(GetOwningPawn());
	if ((Vehicle != nullptr) ){
		FVector2D ScaleVec(HUDYRatio * 1.4f, HUDYRatio * 1.4f);
		DrawPauseMenu(ScaleVec);
		// Speed
		FCanvasTextItem SpeedTextItem(FVector2D(HUDXRatio * 805.f, HUDYRatio * 455), Vehicle->SpeedDisplayString, HUDFont, FLinearColor::White);
		SpeedTextItem.Scale = ScaleVec;
		Canvas->DrawItem(SpeedTextItem);
		// Gear
		FCanvasTextItem GearTextItem(FVector2D(HUDXRatio * 805.f, HUDYRatio * 500.f), Vehicle->GearDisplayString, HUDFont, Vehicle->bInReverseGear == false ? Vehicle->GearDisplayColor : Vehicle->GearDisplayReverseColor);
		GearTextItem.Scale = ScaleVec;
		Canvas->DrawItem(GearTextItem);
	}
}

void ACarHUD::DrawPauseMenu(const FVector2D& scale_vec) {	
	const FVector2D pausebg_pos(0,0);

	// draw the crosshair
	FCanvasTileItem TileItem(pausebg_pos, pause_background_tex->Resource, FLinearColor::White);
	
	TileItem.Size = FVector2D(Canvas->SizeX, Canvas->SizeY);
	Canvas->DrawItem(TileItem);
}


