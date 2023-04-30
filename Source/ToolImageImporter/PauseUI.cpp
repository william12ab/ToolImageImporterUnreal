#include "PauseUI.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
void UPauseUI::NativeConstruct() {
	Super::NativeConstruct();
	return_to_menu_button->OnClicked.AddUniqueDynamic(this, &UPauseUI::ReturnToMenu);
	return_to_game_button->OnClicked.AddUniqueDynamic(this, &UPauseUI::ReturnToGame);
	vehicle_pawn = Cast<AVehicleController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));//casting to pawn
}
void UPauseUI::ReturnToMenu() {
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuLevel");
}
void UPauseUI::ReturnToGame() {
	vehicle_pawn->SetPause(false);
}
void UPauseUI::RenderPauseDisplay() {
	if (vehicle_pawn->GetIsPaused()) {
		wrapper_->SetVisibility(ESlateVisibility::Visible);
		UGameplayStatics::SetGamePaused(UGameplayStatics::GetPlayerPawn(GetWorld(), 0), true);
	}
	else {
		wrapper_->SetVisibility(ESlateVisibility::Collapsed);
		UGameplayStatics::SetGamePaused(UGameplayStatics::GetPlayerPawn(GetWorld(), 0), false);
	}
}

void UPauseUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);
	RenderPauseDisplay();
	ListenForInputAction("Pause", IE_Pressed, true, FOnInputAction::BindUFunction(this, &UPauseUI::ReturnToGame));
	
}