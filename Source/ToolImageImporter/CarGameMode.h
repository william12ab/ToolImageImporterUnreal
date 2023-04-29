// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameMode.h"
#include "CarGameMode.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API ACarGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ACarGameMode();

protected:
	UPROPERTY(EditAnywhere, Category = "Class Types")
		TSubclassOf<UUserWidget> WidgetClass;
	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
		class UPauseUI*pause_ui;

	virtual void BeginPlay() override;
};
