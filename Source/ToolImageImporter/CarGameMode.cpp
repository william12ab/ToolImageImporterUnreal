#include "CarGameMode.h"
#include "VehicleController.h"
#include "Blueprint/UserWidget.h"
#include "PauseUI.h"
#include "CarHUD.h"
ACarGameMode::ACarGameMode(){
	DefaultPawnClass = AVehicleController::StaticClass();
	HUDClass = ACarHUD::StaticClass();
}


void ACarGameMode::BeginPlay() {
	if (IsValid(WidgetClass)){
		pause_ui = Cast<UPauseUI>(CreateWidget(GetWorld(), WidgetClass));

		if (pause_ui){
			pause_ui->AddToViewport();
		}
	}
}

