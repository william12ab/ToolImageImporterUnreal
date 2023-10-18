#pragma once
#include "GameFramework/GameMode.h"
#include "OptionsWidget.h"
#include "MainMenuUIWidget.h"
#include "MenuGameMode.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API AMenuGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AMenuGameMode();
	virtual void Tick(float DeltaTime)override;
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UOptionsWidget> options_class = nullptr;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		class UOptionsWidget* options_ui;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UMainMenuUIWidget> menu_class = nullptr;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		class UMainMenuUIWidget* menu_ui;

	virtual void BeginPlay() override;

};
