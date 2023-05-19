#include "MainMenuUIWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
void UMainMenuUIWidget::NativeConstruct(){
	Super::NativeConstruct();
	play_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickPlay);
	test_arena_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickTestArena);
	options_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickOptions);
	quit_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickQuit);
	FVector2D size_;
	GEngine->GameViewport->GetViewportSize(size_);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(size_.X / 2, size_.Y / 2);

}


void UMainMenuUIWidget::OnClickPlay() {
	level_name = "Main";
	TimerDelay();
}

void UMainMenuUIWidget::OnClickTestArena() {
	level_name = "TestArenaLevel";
	TimerDelay();
}

void UMainMenuUIWidget::OnClickOptions() {
	UE_LOG(LogTemp, Warning, TEXT("test"));
	PlaySound(button_sound_base);
}

void UMainMenuUIWidget::OnClickQuit() {
	PlaySound(button_sound_base);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand("quit");
}

void UMainMenuUIWidget::TimerDelay() {
	PlaySound(button_sound_base);
	FName new_name = level_name;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			LevelFunc();
		}, 0.3f, false);
}

void UMainMenuUIWidget::LevelFunc() {
	UGameplayStatics::OpenLevel(GetWorld(), level_name);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(1, 1);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
}