#pragma once
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/CanvasPanel.h"
#include "Components/CheckBox.h"
#include "OptionsWidget.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API UOptionsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOpen();
	bool GetActive() { return is_active; }
	float GetMusicVol() { return music_vol; }
	float GetVoiceVol() { return voice_vol; }
	float GetEngineVol() { return engine_vol; }
	bool GetIsTimerDisp() { return is_timer_disp; }
	bool GetIsSpedoDisp() { return is_spedo_disp; }
protected:
	//functions
	void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
		void ReturnFunction();
	UFUNCTION()
		void SoundButtonFunction(); 
	UFUNCTION()
		void DisplayButtonFunction();
	UFUNCTION()
		void ClosePanel();
	
	//ui properties
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* return_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* close_button_panel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* sound_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* display_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* sound_panel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* display_panel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* whole_panel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USlider* music_slider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USlider* car_sfx_slider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USlider* voice_slider;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCheckBox* timer_box;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCheckBox* progress_box;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCheckBox* codriver_box;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCheckBox* spedo_box;


	float music_vol;
	float engine_vol;
	float voice_vol;
	bool is_timer_disp;
	bool is_progress_disp;
	bool is_codriver_disp;
	bool is_spedo_disp;
	bool is_active;
};
