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
	//float XL = 1980.f;
	//float YL = 1080.f;
	//FString ScoreString = FString::FromInt(2);
	//Canvas->TextSize(HUDFont, ScoreString, XL, YL);
	
}

void ACarHUD::DrawHUD(){
	Super::DrawHUD();

	FVector2D size_;
	GEngine->GameViewport->GetViewportSize(size_);
	// Get our vehicle so we can check if we are in car. If we are we don't want onscreen HUD
	AVehicleController* Vehicle = Cast<AVehicleController>(GetOwningPawn());
	if ((Vehicle != nullptr) ){

		FVector2D ScaleVec(1, 1);
		// Gear
		FCanvasTextItem GearTextItem(FVector2D(size_.X * .95f, size_.Y * .8f), Vehicle->GearDisplayString, HUDFont, Vehicle->is_in_reverse_gear == false ? Vehicle->GearDisplayColor : Vehicle->GearDisplayReverseColor);
		Canvas->DrawItem(GearTextItem);
		// Speed
		FCanvasTextItem SpeedTextItem(FVector2D(size_.X * .95f, size_.Y * .85f), Vehicle->SpeedDisplayString, HUDFont, FLinearColor::White);
		SpeedTextItem.Scale = ScaleVec;
		Canvas->DrawItem(SpeedTextItem);
		//rpm
		FCanvasTextItem RPMTextItem(FVector2D(size_.X * .95f, size_.Y * .90f), Vehicle->RPMDisplayString, HUDFont, FLinearColor::Red);
		RPMTextItem.Scale = ScaleVec;
		Canvas->DrawItem(RPMTextItem);
	}
}

void ACarHUD::DrawPauseMenu(const FVector2D& scale_vec) {	
	const FVector2D pausebg_pos(0,0);

	// draw the crosshair
	FCanvasTileItem TileItem(pausebg_pos, pause_background_tex->Resource, FLinearColor::White);
	
	TileItem.Size = FVector2D(Canvas->SizeX, Canvas->SizeY);
	Canvas->DrawItem(TileItem);
}


