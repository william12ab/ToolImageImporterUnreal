#include "MainMenuUIWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Components/Image.h"
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

	buttons_.Add(large_button);
	buttons_.Add(largec_button);
	buttons_.Add(largew_button);
	buttons_.Add(regw_button);
	buttons_.Add(regc_button);
	buttons_.Add(regb_button);
	for (int32 i = 0; i < buttons_.Num(); ++i){
		UButton* Button = buttons_[i];
		SButton* ButtonWidget = (SButton*)&(Button->TakeWidget().Get());
		ButtonWidget->SetOnClicked(FOnClicked::CreateLambda([this, i](){
				OnClickSelectLevel(i);
				return FReply::Handled();
			}));
	}

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
		level_loader.ReadTrackImage(0, GetWorld());
		//
		SetLoading("/Game/default_tracks/text");
		//
		TimerDelay();
	}
}

void UMainMenuUIWidget::SetLoading(const FString& name_) {
	loading_panel->SetVisibility(ESlateVisibility::Visible);
	name_;
	UTexture2D* temp_texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *name_));
	track_image->SetBrushFromTexture(temp_texture, true);
}

void UMainMenuUIWidget::OnClickSelectLevel(const int& index) {
	FString button_name = buttons_[index]->GetName();
	
	auto index_=button_name.Find("_");
	button_name.RemoveAt(index_,7);
	
	PlaySound(button_sound_base);
	FString base_folder = "F:/ToolImageImporter/Content/default_tracks/";
	FString track_name = "/0track_image.png";
	FString name_ = base_folder + button_name + track_name;
	file_opener.SetIsLoaded(true);
	file_opener.SetFolderName(name_);
	file_opener.SetExtension(button_name);
	level_name = "Main";
	SetLoading("Game/textures/large");
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