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
	file_opener.OpenApplication();
	if (file_opener.GetIsOpened()){
		UGameplayStatics::OpenLevel(GetWorld(), "Main");
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(1, 1);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
	}
}

void UMainMenuUIWidget::OnClickTestArena() {
	UGameplayStatics::OpenLevel(GetWorld(), "TestArenaLevel");
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(1, 1);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
}

void UMainMenuUIWidget::OnClickOptions() {
}

void UMainMenuUIWidget::OnClickQuit() {
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand("quit");
}