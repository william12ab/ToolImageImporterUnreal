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
	ConstructorHelpers::FClassFinder<UEndUI> EndWidgetClassFinder(TEXT("/Game/UI_BP/EndBP"));
	if (EndWidgetClassFinder.Succeeded()) {
		end_class = EndWidgetClassFinder.Class;
	}
	SetTickableWhenPaused(true);
	sound_ref= Cast<AMainSounds>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainSounds::StaticClass()));
	is_set_end = false;
	auto f =UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
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

	if (sound_ref != nullptr) {
		sound_ref->PlayBackgroundMusic();
	}
	
}


void ACarGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (pause_ui->GetIsActive()) {
		options_ui->SetOpen();
		pause_ui->SetActive(false);
	}
	if (sound_ref!=nullptr){
		//sounds
		sound_ref->SetVolume(options_ui->GetMusicVol());
		sound_ref->SetVolumeVoice(options_ui->GetVoiceVol());
		sound_ref->SetVolumeEngine(options_ui->GetEngineVol());
	}
	if (vehicle_ref!=nullptr){
		vehicle_ref->SetIsRenderSpedo(options_ui->GetIsSpedoDisp());
		vehicle_ref->SetIsRenderTimer(options_ui->GetIsTimerDisp());
		if (vehicle_ref->GetBoolEnd()){
			//display end UI
			if (!is_set_end){
				if (IsValid(SomeWidgetClass)) {
					end_ui = CreateWidget<UEndUI>(UGameplayStatics::GetPlayerController(GetWorld(), 0), end_class);
					if (end_ui) {
						end_ui->AddToViewport(9919);
					}
				}
				end_ui->SetTimer();
				is_set_end = true;
				UGameplayStatics::SetGamePaused(UGameplayStatics::GetPlayerPawn(GetWorld(), 0), true);
			}
		}
	}
	else {
		vehicle_ref = Cast<AVehicleController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	}
}
