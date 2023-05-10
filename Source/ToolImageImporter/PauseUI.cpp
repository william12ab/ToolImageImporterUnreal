#include "PauseUI.h"
#include "GameFramework/PlayerStart.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
void UPauseUI::NativeConstruct() {
	Super::NativeConstruct();
	return_to_menu_button->OnClicked.AddUniqueDynamic(this, &UPauseUI::ReturnToMenu);
	return_to_game_button->OnClicked.AddUniqueDynamic(this, &UPauseUI::ReturnToGame);
	restart_game_button->OnClicked.AddUniqueDynamic(this, &UPauseUI::RestartFunction);
	vehicle_pawn = Cast<AVehicleController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));//casting to pawn
}
void UPauseUI::ReturnToMenu() {
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuLevel");
}
void UPauseUI::ReturnToGame() {
	vehicle_pawn->SetPause(false);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(1, 1);
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
}

void UPauseUI::RestartFunction() {
	FString temp_level_name = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	if (temp_level_name =="Main"){
		vehicle_pawn->RestartMainLevel();
		vehicle_pawn->SetPause(false);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(1, 1);
	}
	else if (temp_level_name=="TestArenaLevel"){
		TArray<AActor*> temp_player_start;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), temp_player_start);
		auto f= temp_player_start[0]->GetActorLocation();
		
		vehicle_pawn->TeleportTo(f, FRotator(0.0f, 0.f, 0.0f), false, false);
		vehicle_pawn->Restart();
		vehicle_pawn->SetPause(false);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(1, 1);
	}
}