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
public:
	void SetActive(bool f) { is_active = f; }
	bool GetIsActive() { return is_active; }
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
	UFUNCTION()
		void OptionsButton();
	void RenderPauseDisplay();
	//ui properties
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
		class UImage* background_;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* return_to_menu_image;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* return_to_menu_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* return_to_game_image;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* return_to_game_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* options_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* wrapper_;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* restart_game_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* restart_image;
	AVehicleController* vehicle_pawn;//ref for pause

	bool is_active;
};
