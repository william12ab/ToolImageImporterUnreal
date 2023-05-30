#pragma once
#include "Blueprint/UserWidget.h"
#include "VehicleController.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "EndUI.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API UEndUI : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetTimer();
protected:
	//functions
	void NativeConstruct() override;
	

	UFUNCTION()
		void RestartFunction();
	UFUNCTION()
		void GoToMenuFunction();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* restart_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* menu_button;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* stage_time_text;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* penalties;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* best_time_text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* panel;
	AVehicleController* vehicle_pawn;//ref for time etc
};
