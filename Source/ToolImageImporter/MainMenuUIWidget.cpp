#include "MainMenuUIWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Widgets/Input/SButton.h"
#include "UObject/ConstructorHelpers.h"

void UMainMenuUIWidget::NativeConstruct() {
	Super::NativeConstruct();
	play_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickPlay);
	test_arena_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickTestArena);
	options_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickOptions);
	quit_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickQuit);
	select_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickSelect);
	close_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickCloseButton);
	large_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickLoadLarge);
	largec_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickLoadLargeC);
	largew_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickLoadLargeW);
	regc_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickLoadRegC);
	regb_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickLoadRegB);
	regw_button->OnClicked.AddUniqueDynamic(this, &UMainMenuUIWidget::OnClickLoadRegW);

	FVector2D size_;
	GEngine->GameViewport->GetViewportSize(size_);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(size_.X / 2, size_.Y / 2);
	is_active = false;
}

void UMainMenuUIWidget::OnClickPlay() {
	file_opener.OpenApplication();
	if (file_opener.GetIsOpened()) {
		level_name = "Main";
		file_opener.SetIsLoaded(false);
		TimerDelay();
		level_loader.ReadTrackImage(0, GetWorld());
		

	}
}


void UMainMenuUIWidget::OnClickLoadRegB() {
	PlaySound(button_sound_base);
	FString name_ = "F:/ToolImageImporter/Content/default_tracks/regb/0track_image.png";
	file_opener.SetIsLoaded(true);
	file_opener.SetFolderName(name_);
	file_opener.SetExtension("regb");
	level_name = "Main";
	TimerDelay();
}
void UMainMenuUIWidget::OnClickLoadRegW() {
	PlaySound(button_sound_base);
	FString name_ = "F:/ToolImageImporter/Content/default_tracks/regw/0track_image.png";
	file_opener.SetIsLoaded(true);
	file_opener.SetFolderName(name_);
	file_opener.SetExtension("regw");
	level_name = "Main";
	TimerDelay();
}
void UMainMenuUIWidget::OnClickLoadRegC() {
	PlaySound(button_sound_base);
	FString name_ = "F:/ToolImageImporter/Content/default_tracks/regc/0track_image.png";
	file_opener.SetIsLoaded(true);
	file_opener.SetFolderName(name_);
	file_opener.SetExtension("regc");
	level_name = "Main";
	TimerDelay();
}

void UMainMenuUIWidget::OnClickLoadLarge() {
	PlaySound(button_sound_base);
	FString name_ = "F:/ToolImageImporter/Content/default_tracks/large/0track_image.png";
	file_opener.SetIsLoaded(true);
	file_opener.SetFolderName(name_);
	file_opener.SetExtension("large");
	level_name = "Main";
	TimerDelay();
}
void UMainMenuUIWidget::OnClickLoadLargeC() {
	PlaySound(button_sound_base);
	FString name_ = "F:/ToolImageImporter/Content/default_tracks/largec/0track_image.png";
	file_opener.SetIsLoaded(true);
	file_opener.SetFolderName(name_);
	file_opener.SetExtension("largec");
	level_name = "Main";
	TimerDelay();
}
void UMainMenuUIWidget::OnClickLoadLargeW() {
	PlaySound(button_sound_base);
	FString name_ = "F:/ToolImageImporter/Content/default_tracks/largew/0track_image.png";
	file_opener.SetIsLoaded(true);
	file_opener.SetFolderName(name_);
	file_opener.SetExtension("largew");
	level_name = "Main";
	TimerDelay();
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
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]() {
		LevelFunc();
		}, 0.3f, false);
}

void UMainMenuUIWidget::LevelFunc() {
	UGameplayStatics::OpenLevel(GetWorld(), level_name);
}