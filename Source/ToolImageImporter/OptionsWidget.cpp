#include "OptionsWidget.h"

void UOptionsWidget::NativeConstruct() {
	Super::NativeConstruct();
	return_button->OnClicked.AddUniqueDynamic(this, &UOptionsWidget::ReturnFunction);
	sound_button->OnClicked.AddUniqueDynamic(this, &UOptionsWidget::SoundButtonFunction);
	display_button->OnClicked.AddUniqueDynamic(this, &UOptionsWidget::DisplayButtonFunction);
}

void UOptionsWidget::ReturnFunction() {

}

void UOptionsWidget::SoundButtonFunction() {

}

void UOptionsWidget::DisplayButtonFunction() {

}