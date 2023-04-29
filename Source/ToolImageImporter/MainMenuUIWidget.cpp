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
}
void UMainMenuUIWidget::OnClickPlay() {
	UE_LOG(LogTemp, Warning, TEXT("play"));
	file_opener.OpenApplication();
	if (file_opener.GetIsOpened()){
		UGameplayStatics::OpenLevel(GetWorld(), "Main");
	}
}

void UMainMenuUIWidget::OnClickTestArena() {
	UE_LOG(LogTemp, Warning, TEXT("test a"));
	UGameplayStatics::OpenLevel(GetWorld(), "TestArenaLevel");
}

void UMainMenuUIWidget::OnClickOptions() {
	UE_LOG(LogTemp, Warning, TEXT("options"));
}

void UMainMenuUIWidget::OnClickQuit() {
	UE_LOG(LogTemp, Warning, TEXT("quit "));
}