#include "CarGameMode.h"
#include "VehicleController.h"
#include "Blueprint/UserWidget.h"
#include "CarHUD.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
ACarGameMode::ACarGameMode(){
	DefaultPawnClass = AVehicleController::StaticClass();
	HUDClass = ACarHUD::StaticClass();
	ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/PauseBP"));
	if (WidgetClassFinder.Succeeded()){
		SomeWidgetClass = WidgetClassFinder.Class;
	}
	ConstructorHelpers::FClassFinder<UOptionsWidget> OptionsWidgetClassFinder(TEXT("/Game/UI_BP/OptionsBP"));
	if (OptionsWidgetClassFinder.Succeeded()) {
		options_class = OptionsWidgetClassFinder.Class;
	}
	SetTickableWhenPaused(true);
}
void ACarGameMode::BeginPlay() {
	Super::BeginPlay();
	if (IsValid(options_class)) {
		options_ui = CreateWidget<UOptionsWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), options_class);
		if (options_ui) {
			options_ui->AddToViewport(9999);
		}
	}
	if (IsValid(SomeWidgetClass)) {
		pause_ui = CreateWidget<UPauseUI>(UGameplayStatics::GetPlayerController(GetWorld(), 0), SomeWidgetClass);
		if (pause_ui) {
			pause_ui->AddToViewport(9909);
		}
	}
}


void ACarGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (pause_ui->GetIsActive()) {
		UE_LOG(LogTemp, Warning, TEXT("true"));

		options_ui->SetOpen();
		pause_ui->SetActive(false);
	}
	else if (!options_ui->GetActive()) {
	}
	UE_LOG(LogTemp, Warning, TEXT("true"));
}
