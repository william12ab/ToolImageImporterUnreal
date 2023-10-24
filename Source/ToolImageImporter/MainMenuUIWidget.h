#pragma once
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "FileLoader.h"
#include "SoundHandler.h"
#include "HeightmapHandler.h"
#include "Components/CanvasPanel.h"
#include "MainMenuUIWidget.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API UMainMenuUIWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetActive(bool f) { is_active = f; }
	bool GetIsActive() { return is_active; }
protected:

	UPROPERTY(Category = MainMenuWidg, EditAnywhere, BlueprintReadWrite)
		USoundBase* button_sound_base;

	//functions
	void NativeConstruct() override;
	void TimerDelay();
	void LevelFunc();
	void SetLoading(const FString& name_);

	//ui
	UPROPERTY(meta = (BindWidget))
		class UImage* select_image;
	UPROPERTY(meta = (BindWidget))
		class UImage* play_image;
	UPROPERTY(meta = (BindWidget))
		class UImage* test_arena_image;
	UPROPERTY(meta = (BindWidget))
		class UImage* options_image;
	UPROPERTY(meta = (BindWidget))
		class UImage* quit_image;
	UPROPERTY(meta = (BindWidget))
		class UButton* select_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* play_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* test_arena_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* options_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* quit_button;

	//default levels ui
	UPROPERTY(meta = (BindWidget))
		class UButton* large_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* largec_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* largew_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* regw_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* regc_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* regb_button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UCanvasPanel* default_levels;
	UPROPERTY(meta = (BindWidget))
		class UButton* close_button;

	//loading panels UI
	UPROPERTY(meta = (BindWidget))
		class UImage* track_image;
	UPROPERTY(meta = (BindWidget))
		class UCanvasPanel* loading_panel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* length_text;
	


	//ufunctions
	UFUNCTION()
		void OnClickSelect();
	UFUNCTION()
		void OnClickPlay();
	UFUNCTION()
		void OnClickTestArena();
	UFUNCTION()
		void OnClickOptions();
	UFUNCTION()
		void OnClickQuit();
	UFUNCTION()
		void OnClickCloseButton();
	UFUNCTION()
		void OnClickSelectLevel(const int& index);
	//accessers
	FileLoader file_opener;
	FName level_name;
	bool is_active;
	HeightmapHandler level_loader;


	TArray<UButton*> buttons_;
};