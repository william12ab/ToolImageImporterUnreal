// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MyProceduralMesh.h"
#include "UIWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOOLIMAGEIMPORTER_API UUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void GeneratePlane();
protected:
	

	UPROPERTY(meta = (BindWidget))
		class UButton* GenerateButton;

	AMyProceduralMesh p_mesh;
};
