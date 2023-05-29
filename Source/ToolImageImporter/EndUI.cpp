#include "EndUI.h"

void UEndUI::NativeConstruct() {
	Super::NativeConstruct();

	restart_button->OnClicked.AddUniqueDynamic(this, &UEndUI::RestartFunction);
	menu_button->OnClicked.AddUniqueDynamic(this, &UOptionsWidget::GoToMenuFunction);
	vehicle_pawn = Cast<AVehicleController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));//casting to pawn

}

void UEndUI::RestartFunction() {
	//restart level
}
void UEndUI::GoToMenuFunction() {
	//go to menu
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuLevel");
}