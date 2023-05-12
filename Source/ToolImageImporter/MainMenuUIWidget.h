#pragma once
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "FileLoader.h"
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
		class UImage* play_image;
	UPROPERTY(meta = (BindWidget))
		class UImage* test_arena_image;
	UPROPERTY(meta = (BindWidget))
		class UImage* options_image;
	UPROPERTY(meta = (BindWidget))
		class UImage* quit_image;
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

	//accessers
	FileLoader file_opener;
};
