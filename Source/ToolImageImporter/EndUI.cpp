#include "EndUI.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
void UEndUI::NativeConstruct() {
	Super::NativeConstruct();

	restart_button->OnClicked.AddUniqueDynamic(this, &UEndUI::RestartFunction);
	menu_button->OnClicked.AddUniqueDynamic(this, &UEndUI::GoToMenuFunction);
	vehicle_pawn = Cast<AVehicleController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));//casting to pawn

}

void UEndUI::RestartFunction() {
	//restart level
}
void UEndUI::GoToMenuFunction() {
	//go to menu
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuLevel");
}

void UEndUI::SetTimer() {
	panel->SetVisibility(ESlateVisibility::Visible);
	stage_time_text->SetText(vehicle_pawn->GetFinalText());
	penalties->SetText(vehicle_pawn->GetPenText());
}