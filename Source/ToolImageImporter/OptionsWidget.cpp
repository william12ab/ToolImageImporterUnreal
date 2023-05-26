#include "OptionsWidget.h"

void UOptionsWidget::NativeConstruct() {
	Super::NativeConstruct();
	return_button->OnClicked.AddUniqueDynamic(this, &UOptionsWidget::ReturnFunction);
	sound_button->OnClicked.AddUniqueDynamic(this, &UOptionsWidget::SoundButtonFunction);
	display_button->OnClicked.AddUniqueDynamic(this, &UOptionsWidget::DisplayButtonFunction);
	close_button_panel->OnClicked.AddUniqueDynamic(this, &UOptionsWidget::ClosePanel);
	music_vol = 1.0f;
	engine_vol = 1.0f;
	voice_vol = 1.0f;
	is_timer_disp=true;
	is_progress_disp=true;
	is_codriver_disp=true;
	is_spedo_disp=true;
	is_active = false;
}

void UOptionsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	if (sound_panel->GetVisibility() == ESlateVisibility::Visible){
		UE_LOG(LogTemp, Warning, TEXT("test"));
		music_vol = music_slider->GetValue();
		engine_vol = car_sfx_slider->GetValue();
		voice_vol = voice_slider->GetValue();
	}
	if (display_panel->GetVisibility() == ESlateVisibility::Visible){
		UE_LOG(LogTemp, Warning, TEXT("test2"));
		is_timer_disp = timer_box->IsChecked();
		is_progress_disp = progress_box->IsChecked();
		is_codriver_disp = codriver_box->IsChecked();
		is_spedo_disp = spedo_box->IsChecked();
	}
}

void UOptionsWidget::ReturnFunction() {
	whole_panel->SetVisibility(ESlateVisibility::Hidden);
	is_active = false;
}
void UOptionsWidget::SetOpen() {
	whole_panel->SetVisibility(ESlateVisibility::Visible);
	is_active = true;
}

void UOptionsWidget::SoundButtonFunction() {
	sound_panel->SetVisibility(ESlateVisibility::Visible);
	close_button_panel->SetVisibility(ESlateVisibility::Visible);
}

void UOptionsWidget::DisplayButtonFunction() {
	display_panel->SetVisibility(ESlateVisibility::Visible);
	close_button_panel->SetVisibility(ESlateVisibility::Visible);
}

void UOptionsWidget::ClosePanel() {
	if (sound_panel->IsVisible()){
		sound_panel->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		display_panel->SetVisibility(ESlateVisibility::Hidden);
	}
	close_button_panel->SetVisibility(ESlateVisibility::Hidden);
}