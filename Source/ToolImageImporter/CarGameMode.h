// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameMode.h"
#include "PauseUI.h"
#include "CarGameMode.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API ACarGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ACarGameMode();

protected:    
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UPauseUI> SomeWidgetClass = nullptr;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		class UPauseUI*pause_ui;

	virtual void BeginPlay() override;
};
