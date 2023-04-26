#pragma once
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MainMenuUIWidget.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API UMainMenuUIWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	//functions
	void NativeConstruct() override;
	//ui
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* play_text;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* test_arena_text;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* options_text;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* quit_text;
	UPROPERTY(meta = (BindWidget))
		class UButton* play_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* test_arena_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* options_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* quit_button;
	//ufunctions
	UFUNCTION()
		void OnClickPlay();
	UFUNCTION()
		void OnClickTestArena();
	UFUNCTION()
		void OnClickOptions();
	UFUNCTION()
		void OnClickQuit();
};
