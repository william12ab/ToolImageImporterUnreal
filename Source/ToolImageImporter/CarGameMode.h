// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameMode.h"
#include "PauseUI.h"
#include "OptionsWidget.h"
#include "CarGameMode.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API ACarGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ACarGameMode();
	virtual void Tick(float DeltaTime)override;
protected:    
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UPauseUI> SomeWidgetClass = nullptr;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UOptionsWidget> options_class= nullptr;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		class UPauseUI*pause_ui;
	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		class UOptionsWidget* options_ui;

	virtual void BeginPlay() override;
};
