#pragma once
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "PauseUI.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API UPauseUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//functions
	void NativeConstruct() override;
	UFUNCTION()
		void ReturnToMenu();
	//ui properties
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
		class UImage* background_;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* return_to_menu_text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* return_to_menu_button;

};
