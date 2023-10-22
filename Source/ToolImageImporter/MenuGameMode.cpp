#include "MenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>



AMenuGameMode::AMenuGameMode() {
	ConstructorHelpers::FClassFinder<UOptionsWidget> OptionsWidgetClassFinder(TEXT("/Game/UI_BP/OptionsBP"));
	if (OptionsWidgetClassFinder.Succeeded()) {
		options_class = OptionsWidgetClassFinder.Class;
	}

	ConstructorHelpers::FClassFinder<UMainMenuUIWidget> MenuWidgetClassFinder(TEXT("/Game/UI_BP/BPMainMenu"));
	if (MenuWidgetClassFinder.Succeeded()) {
		menu_class = MenuWidgetClassFinder.Class;
	}
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}


void AMenuGameMode::BeginPlay() {
	Super::BeginPlay();
	if (IsValid(options_class)) {
		options_ui = CreateWidget<UOptionsWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), options_class);
		if (options_ui) {
			options_ui->AddToViewport(9999);
		}
	}
	if (IsValid(menu_class)) {
		menu_ui = CreateWidget<UMainMenuUIWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), menu_class);
		if (menu_ui) {
			menu_ui->AddToViewport(9909);
		}
	}
}

void AMenuGameMode::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	if (menu_ui->GetIsActive()){
		options_ui->SetOpen();
		menu_ui->SetActive(false);
	}
}

