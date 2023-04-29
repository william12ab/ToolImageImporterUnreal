#include "PauseUI.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
void UPauseUI::NativeConstruct() {
	Super::NativeConstruct();
	return_to_menu_button->OnClicked.AddUniqueDynamic(this, &UPauseUI::ReturnToMenu);
}


void UPauseUI::ReturnToMenu() {
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuLevel");
}