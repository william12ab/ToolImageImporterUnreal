#include "PauseUI.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
void UPauseUI::NativeConstruct() {
	Super::NativeConstruct();
	return_to_menu_button->OnClicked.AddUniqueDynamic(this, &UPauseUI::ReturnToMenu);
	vehicle_pawn = Cast<AVehicleController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));//casting to pawn
}
void UPauseUI::ReturnToMenu() {
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuLevel");
}

void UPauseUI::RenderPauseDisplay() {
	if (vehicle_pawn->GetIsPaused()) {
		wrapper_->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		wrapper_->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPauseUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);
	RenderPauseDisplay();
}