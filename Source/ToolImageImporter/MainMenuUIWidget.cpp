#include "MainMenuUIWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

void UMainMenuUIWidget::NativeConstruct(){
	Super::NativeConstruct();
	play_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickPlay);
	test_arena_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickTestArena);
	options_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickOptions);
	quit_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickQuit);
	select_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickSelect);
	close_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickCloseButton);
	large_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickLoad);
	
	FVector2D size_;
	GEngine->GameViewport->GetViewportSize(size_);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(size_.X / 2, size_.Y / 2);
	is_active = false;
}


void UMainMenuUIWidget::OnClickPlay() {
	file_opener.OpenApplication();
	if (file_opener.GetIsOpened()) {
		level_name = "Main";
		TimerDelay();
	}
}

void UMainMenuUIWidget::OnClickLoad() {
	PlaySound(button_sound_base);
	
	UE_LOG(LogTemp, Warning, TEXT("The Actor's name is %s"), *large_button->GetDisplayLabel());
	/*f_l.SetFolderName();
	level_name = "Main";
	TimerDelay();*/
}

void UMainMenuUIWidget::OnClickTestArena() {
	level_name = "TestArenaLevel";
	TimerDelay();
}

void UMainMenuUIWidget::OnClickOptions() {
	PlaySound(button_sound_base);
	is_active = true;
}

void UMainMenuUIWidget::OnClickSelect() {
	PlaySound(button_sound_base);
	default_levels->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuUIWidget::OnClickCloseButton() {
	PlaySound(button_sound_base);
	default_levels->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuUIWidget::OnClickQuit() {
	PlaySound(button_sound_base);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand("quit");
}

void UMainMenuUIWidget::TimerDelay() {
	PlaySound(button_sound_base);
	FName new_name = level_name;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&](){
		LevelFunc();
	}, 0.3f, false);
}

void UMainMenuUIWidget::LevelFunc() {
	UGameplayStatics::OpenLevel(GetWorld(), level_name);
}