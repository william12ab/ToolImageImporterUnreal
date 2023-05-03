#pragma once
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "VehicleController.h"
#include "PauseUI.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API UPauseUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	//functions
	void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION()
		void ReturnToMenu();
	UFUNCTION()
		void ReturnToGame();
	UFUNCTION()
		void RestartFunction();
	void RenderPauseDisplay();
	//ui properties
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
		class UImage* background_;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* return_to_menu_text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* return_to_menu_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* return_to_game_text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* return_to_game_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* wrapper_;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* restart_game_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* restart_text;
	AVehicleController* vehicle_pawn;//ref for pause
};
