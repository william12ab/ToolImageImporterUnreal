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

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ACarHUD::ACarHUD(){
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Game/ui_font"));
	HUDFont = Font.Object;
	
	static ConstructorHelpers::FObjectFinder<UTexture2D> puase_back_obj(TEXT("Texture2D'/Game/Textures/pausedbg.pausedbg'"));
	pause_background_tex = puase_back_obj.Object;

	const FString BaseTexturePath = TEXT("Texture2D'/Game/Textures/pacenote_images/");
	const FString TextureSuffix = TEXT("."); // Adjust the size accordingly

	TArray<FString> TexturePaths;
	for (int TextureIndex = 1; TextureIndex <= 23; ++TextureIndex) {
		FString CurrentTexturePath = BaseTexturePath + FString::Printf(TEXT("%d"), TextureIndex) + TextureSuffix+ FString::Printf(TEXT("%d"), TextureIndex);
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
			UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *TexturePath);
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

	for (int i = 0; i < 6; i++){
		const FVector2D position_(0, 0);
		FCanvasTileItem temp(position_, pacenote_images[0]->Resource, FLinearColor::Transparent);
		pacenote_items.Add(temp);
	}
	is_timer_started = false;
	is_drawing = false;
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

			ShowNote();
			
			for (int i = 0; i < 6; i++) {
				if (is_drawing){
					Canvas->DrawItem(pacenote_items[i]);
					if (!is_timer_started){
						start = std::chrono::high_resolution_clock::now();
						is_timer_started = true;
					}
					auto end = std::chrono::high_resolution_clock::now();
					std::chrono::duration<double> elapsed = end - start;

					if (elapsed.count()>2.0f){
						is_drawing = false;
						is_timer_started = false;
						ClearNotes();
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


void ACarHUD::ShowNote() {
	//positions see note for order
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.15f);
	const float y_position = Center.Y - (pacenote_images[0]->GetSurfaceHeight() * 0.5f);
	const FVector2D position_four((Center.X - (pacenote_images[0]->GetSurfaceWidth() * 0.5)), (y_position));
	const FVector2D position_three((Center.X - (pacenote_images[6]->GetSurfaceWidth() * 1.6)), (y_position));
	const FVector2D position_two((Center.X - (pacenote_images[6]->GetSurfaceWidth() * 2.7)), (y_position));
	const FVector2D position_one((Center.X - (pacenote_images[6]->GetSurfaceWidth() * 3.8)), (y_position));
	const FVector2D position_zero((Center.X - (pacenote_images[6]->GetSurfaceWidth() * 4.9)), (y_position));
	const FVector2D position_five((Center.X + (pacenote_images[6]->GetSurfaceWidth() * 0.6)), (y_position));
	if (pace_notes_actor->GetNotesToDisplay().IsValidIndex(0)){
		switch (pace_notes_actor->GetNotesToDisplay().Num()) {
		case 1: {
			DisplayNote(0, position_four, pacenote_images[pace_notes_actor->GetNotesToDisplay()[0]]->Resource);
			is_drawing = true;
			pace_notes_actor->EmptyArray();
			break; 
		}
		case 2: {
			DisplayNote(0, position_three, pacenote_images[pace_notes_actor->GetNotesToDisplay()[0]]->Resource);
			DisplayNote(1, position_four, pacenote_images[pace_notes_actor->GetNotesToDisplay()[1]]->Resource);
			is_drawing = true;
			pace_notes_actor->EmptyArray();
			break; 
		}
		case 3: {
			DisplayNote(0, position_two, pacenote_images[pace_notes_actor->GetNotesToDisplay()[0]]->Resource);
			DisplayNote(1, position_three, pacenote_images[pace_notes_actor->GetNotesToDisplay()[1]]->Resource);
			DisplayNote(2, position_four, pacenote_images[pace_notes_actor->GetNotesToDisplay()[2]]->Resource);
			is_drawing = true;
			pace_notes_actor->EmptyArray();
			break;
		}
		case 4: {
			DisplayNote(0, position_one, pacenote_images[pace_notes_actor->GetNotesToDisplay()[0]]->Resource);
			DisplayNote(1, position_two, pacenote_images[pace_notes_actor->GetNotesToDisplay()[1]]->Resource);
			DisplayNote(2, position_three, pacenote_images[pace_notes_actor->GetNotesToDisplay()[2]]->Resource);
			DisplayNote(3, position_four, pacenote_images[pace_notes_actor->GetNotesToDisplay()[3]]->Resource);
			is_drawing = true;
			pace_notes_actor->EmptyArray();
			break;
		}
		case 5: {
			DisplayNote(0, position_one, pacenote_images[pace_notes_actor->GetNotesToDisplay()[0]]->Resource);
			DisplayNote(1, position_two, pacenote_images[pace_notes_actor->GetNotesToDisplay()[1]]->Resource);
			DisplayNote(2, position_three, pacenote_images[pace_notes_actor->GetNotesToDisplay()[2]]->Resource);
			DisplayNote(3, position_four, pacenote_images[pace_notes_actor->GetNotesToDisplay()[3]]->Resource);
			DisplayNote(4, position_five, pacenote_images[pace_notes_actor->GetNotesToDisplay()[4]]->Resource);
			is_drawing = true;
			pace_notes_actor->EmptyArray();
			break;
		}
		case 6: {
			DisplayNote(0, position_zero, pacenote_images[pace_notes_actor->GetNotesToDisplay()[0]]->Resource);
			DisplayNote(1, position_one, pacenote_images[pace_notes_actor->GetNotesToDisplay()[1]]->Resource);
			DisplayNote(2, position_two, pacenote_images[pace_notes_actor->GetNotesToDisplay()[2]]->Resource);
			DisplayNote(3, position_three, pacenote_images[pace_notes_actor->GetNotesToDisplay()[3]]->Resource);
			DisplayNote(4, position_four, pacenote_images[pace_notes_actor->GetNotesToDisplay()[4]]->Resource);
			DisplayNote(5, position_five, pacenote_images[pace_notes_actor->GetNotesToDisplay()[5]]->Resource);
			is_drawing = true;
			pace_notes_actor->EmptyArray();
			break;
		}
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


