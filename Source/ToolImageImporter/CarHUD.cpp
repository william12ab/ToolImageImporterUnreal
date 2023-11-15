// Fill out your copyright notice in the Description page of Project Settings.


#include "CarHUD.h"
#include "WheeledVehicle.h"
#include "RenderResource.h"
#include "Shader.h"
#include "Engine/Canvas.h"
#include "WheeledVehicleMovementComponent.h"
#include "Engine/Font.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ACarHUD::ACarHUD() {
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Game/ui_font"));
	HUDFont = Font.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> puase_back_obj(TEXT("Texture2D'/Game/Textures/pausedbg.pausedbg'"));
	pause_background_tex = puase_back_obj.Object;

	static ConstructorHelpers::FObjectFinder<UTexture2D> dial_obj(TEXT("Texture2D'/Game/Textures/Dial/SpeedometerPointer.SpeedometerPointer'"));
	dial_img = dial_obj.Object;
	static ConstructorHelpers::FObjectFinder<UTexture2D> speedo_obj(TEXT("Texture2D'/Game/Textures/Dial/SpeedometerBG.SpeedometerBG'"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> tacho_obj(TEXT("Texture2D'/Game/Textures/Dial/TachometerBG.TachometerBG'"));
	tachometor_img = tacho_obj.Object;
	speedometor_img = speedo_obj.Object;


	const FString BaseTexturePath = TEXT("Texture2D'/Game/Textures/pacenote_images/");
	const FString TextureSuffix = TEXT("."); // Adjust the size accordingly

	TArray<FString> TexturePaths;
	for (int TextureIndex = 1; TextureIndex <= 24; ++TextureIndex) {
		FString CurrentTexturePath = BaseTexturePath + FString::Printf(TEXT("%d"), TextureIndex) + TextureSuffix + FString::Printf(TEXT("%d"), TextureIndex);
		FString end_ = "'";
		CurrentTexturePath += end_;
		TexturePaths.Add(CurrentTexturePath);
	}

	int index = 0;
	for (const FString& TexturePath : TexturePaths) {
		ConstructorHelpers::FObjectFinder<UTexture2D> TextureFinder(*TexturePath);
		if (TextureFinder.Succeeded()) {
			pacenote_images.Add(TextureFinder.Object);
			pacenote_images[index] = TextureFinder.Object;
			index++;
		}
		else {
			// Handle the case where the texture could not be found or loaded.
			// You can log an error or take appropriate action here.
		}
	}
	white_.R = 255; white_.G = 250; white_.B = 239; white_.A = 255.f;
	red_.R = 208; red_.G = 34; red_.B = 65; red_.A = 255.f;
	r_unit = FText::FromString("RPM");
	v_unit = FText::FromString("KPH");
	pace_notes_actor = Cast<APaceNotesActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APaceNotesActor::StaticClass()));

	for (int i = 0; i < 6; i++) {
		const FVector2D position_(0, 0);
		FCanvasTileItem temp(position_, pacenote_images[0]->Resource, FLinearColor::Transparent);
		pacenote_items.Add(temp);
	}
	is_timer_started = false;
	is_drawing = false;
	is_extra = false;
	notes_displayed = 0;

	speed_dial_rot = -135.f;
	tacho_dial_rot = -135.f;
}
void ACarHUD::DrawHUD(){
	Super::DrawHUD();

	FVector2D size_;
	GEngine->GameViewport->GetViewportSize(size_);
	// Get our vehicle so we can check if we are in car. If we are we don't want onscreen HUD
	AVehicleController* Vehicle = Cast<AVehicleController>(GetOwningPawn());
	if ((Vehicle != nullptr) ){
		ChangeSpeedo(Vehicle);
		ChangeTacho(Vehicle);
		if (Vehicle->GetIsRenderSpedo()) {
			// Gear
			FCanvasTextItem GearTextItem(FVector2D(size_.X * .90f, size_.Y * .85f), Vehicle->GearDisplayString, HUDFont, Vehicle->is_in_reverse_gear == false ? Vehicle->GearDisplayColor : Vehicle->GearDisplayReverseColor);

			
			FCanvasTileItem speedo_tile(FVector2D(1700.f,900.f), speedometor_img->Resource,FVector2D(128.f,128.f), FLinearColor::White);
			FCanvasTileItem tacho_tile(FVector2D(1572.f, 900.f), tachometor_img->Resource, FVector2D(128.f, 128.f), FLinearColor::White);
			FCanvasTileItem dial_tile_two(FVector2D(1572.f, 900.f), dial_img->Resource, FVector2D(128.f, 128.f), FLinearColor::White);
			FCanvasTileItem dial_tile(FVector2D(1700.f, 900.f), dial_img->Resource, FVector2D(128.f, 128.f), FLinearColor::White);
			dial_tile.BlendMode = SE_BLEND_Translucent;
			dial_tile.Rotation = FRotator(0, speed_dial_rot,0);
			dial_tile.PivotPoint = FVector2D(0.5f,0.5f);
			dial_tile_two.BlendMode = SE_BLEND_Translucent;
			dial_tile_two.Rotation = FRotator(0, tacho_dial_rot, 0);
			dial_tile_two.PivotPoint = FVector2D(0.5f, 0.5f);

			Canvas->DrawItem(speedo_tile);
			Canvas->DrawItem(dial_tile);
			Canvas->DrawItem(tacho_tile);
			Canvas->DrawItem(dial_tile_two);
			Canvas->DrawItem(GearTextItem);
			if (pace_notes_actor != nullptr) {
				ShowNote();
				for (int i = 0; i < 6; i++) {
					if (is_drawing) {
						Canvas->DrawItem(pacenote_items[i]);
						if (!is_timer_started) {
							start = std::chrono::high_resolution_clock::now();
							is_timer_started = true;
						}
						auto end = std::chrono::high_resolution_clock::now();
						std::chrono::duration<double> elapsed = end - start;

						if (elapsed.count() > 2.0f) {
							is_drawing = false;
							is_timer_started = false;
							ClearNotes();
							if (is_extra) {
								pace_notes_actor->SetExtraNotes(extra_array);
								is_extra = false;
							}
						}
					}
				}
			}
		}
	}
}

void ACarHUD::DisplayNote(const int& index,const FVector2D& position_, FTexture* texture) {
	FCanvasTileItem temp(position_, texture, FLinearColor::White);
	pacenote_items[index]=temp;
	pacenote_items[index].BlendMode = SE_BLEND_Translucent;
}
void ACarHUD::ClearNotes() {
	for (size_t i = 0; i < pacenote_items.Num(); i++){
		const FVector2D position_(0, 0);
		FCanvasTileItem temp(position_, pacenote_images[0]->Resource, FLinearColor::Transparent);
		pacenote_items[i]=(temp);
	}
}

void ACarHUD::SwitchStatement(const TArray<int>&array_) {
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.15f);
	const float y_position = Center.Y - (pacenote_images[0]->GetSurfaceHeight() * 0.5f);
	const FVector2D position_four((Center.X - (pacenote_images[0]->GetSurfaceWidth() * 0.5)), (y_position));
	const FVector2D position_three((Center.X - (pacenote_images[6]->GetSurfaceWidth() * 1.6)), (y_position));
	const FVector2D position_two((Center.X - (pacenote_images[6]->GetSurfaceWidth() * 2.7)), (y_position));
	const FVector2D position_one((Center.X - (pacenote_images[6]->GetSurfaceWidth() * 3.8)), (y_position));
	const FVector2D position_zero((Center.X - (pacenote_images[6]->GetSurfaceWidth() * 4.9)), (y_position));
	const FVector2D position_five((Center.X + (pacenote_images[6]->GetSurfaceWidth() * 0.6)), (y_position));
	switch (array_.Num()) {
	case 1: {
		DisplayNote(0, position_four, pacenote_images[array_[0] - 1]->Resource);
		is_drawing = true;
		notes_displayed++;
		break;
	}
	case 2: {
		DisplayNote(0, position_three, pacenote_images[array_[0] - 1]->Resource);
		DisplayNote(1, position_four, pacenote_images[array_[1] - 1]->Resource);
		is_drawing = true;
		notes_displayed++;
		break;
	}
	case 3: {
		DisplayNote(0, position_two, pacenote_images[array_[0] - 1]->Resource);
		DisplayNote(1, position_three, pacenote_images[array_[1] - 1]->Resource);
		DisplayNote(2, position_four, pacenote_images[array_[2] - 1]->Resource);
		is_drawing = true;
		notes_displayed++;
		break;
	}
	case 4: {
		DisplayNote(0, position_one, pacenote_images[array_[0] - 1]->Resource);
		DisplayNote(1, position_two, pacenote_images[array_[1] - 1]->Resource);
		DisplayNote(2, position_three, pacenote_images[array_[2] - 1]->Resource);
		DisplayNote(3, position_four, pacenote_images[array_[3] - 1]->Resource);
		is_drawing = true;
		notes_displayed++;
		break;
	}
	case 5: {
		DisplayNote(0, position_one, pacenote_images[array_[0] - 1]->Resource);
		DisplayNote(1, position_two, pacenote_images[array_[1] - 1]->Resource);
		DisplayNote(2, position_three, pacenote_images[array_[2] - 1]->Resource);
		DisplayNote(3, position_four, pacenote_images[array_[3] - 1]->Resource);
		DisplayNote(4, position_five, pacenote_images[array_[4] - 1]->Resource);
		is_drawing = true;
		notes_displayed++;
		break;
	}
	case 6: {
		DisplayNote(0, position_zero, pacenote_images[array_[0] - 1]->Resource);
		DisplayNote(1, position_one, pacenote_images[array_[1] - 1]->Resource);
		DisplayNote(2, position_two, pacenote_images[array_[2] - 1]->Resource);
		DisplayNote(3, position_three, pacenote_images[array_[3] - 1]->Resource);
		DisplayNote(4, position_four, pacenote_images[array_[4] - 1]->Resource);
		DisplayNote(5, position_five, pacenote_images[array_[5] - 1]->Resource);
		is_drawing = true;
		notes_displayed++;
		break;
	}
	}
}

void ACarHUD::ShowNote() {
	//positions see note for order

	if (pace_notes_actor->GetNotesToDisplay().IsValidIndex(0)){
		if (notes_displayed ==0){
			notes_displayed++;
			pace_notes_actor->EmptyArray();
		}
		if (pace_notes_actor->GetNotesToDisplay().Num()>6){
			for (size_t i = 0; i < 6; i++){
				extra_array.Add(pace_notes_actor->GetNotesToDisplay()[i]);
			}
			SwitchStatement(extra_array);
			extra_array.Empty();
			pace_notes_actor->RemoveSelected();
		}
		else {
			SwitchStatement(pace_notes_actor->GetNotesToDisplay());
			pace_notes_actor->EmptyArray();
		}	
	}
}

void ACarHUD::DrawPauseMenu(const FVector2D& scale_vec) {	
	const FVector2D pausebg_pos(0, 0);

	// draw the crosshair
	FCanvasTileItem TileItem(pausebg_pos, pause_background_tex->Resource, FLinearColor::Blue);
	
	TileItem.Size = FVector2D(Canvas->SizeX, Canvas->SizeY);
	Canvas->DrawItem(TileItem);
}

void ACarHUD::ChangeSpeedo(AVehicleController* Vehicle_) {
	int32 KPH_int = FMath::FloorToInt(Vehicle_->GetCurrentKPH());
	speed_dial_rot = -135.f + (KPH_int * 1.15f);
}

void ACarHUD::ChangeTacho(AVehicleController* Vehicle_) {
	int32 RPM_int = FMath::FloorToInt(Vehicle_->GetCurrentRPM());
	tacho_dial_rot = -135.f + (RPM_int * .023f);
}


