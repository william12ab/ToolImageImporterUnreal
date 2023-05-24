#pragma once
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "OptionsWidget.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	//functions
	void NativeConstruct() override;
	UFUNCTION()
		void ReturnFunction();
	UFUNCTION()
		void SoundButtonFunction(); 
	UFUNCTION()
		void DisplayButtonFunction();

	//ui properties
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* return_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* sound_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* display_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* sound_panel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* display_panel;
};
