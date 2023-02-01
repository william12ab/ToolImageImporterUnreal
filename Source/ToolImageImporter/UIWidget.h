// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "MyProceduralMesh.h"
#include "UIWidget.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API UUIWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	
	void GeneratePlane();
	void DeletePlane();
	void OpenFileWindow();
	void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
		class UButton* generate_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* delete_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* height_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* file_button;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Label;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* height_label;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* width_label;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* spacing_label;

	UFUNCTION()
		void OnClick();
	UFUNCTION()
		void OnClickDelete();
	UFUNCTION()
		void OnClickHeight();
	UFUNCTION()
		void OnEnterText();
	UFUNCTION()
		void OnFileButton();

	AMyProceduralMesh* p_mesh;

	int32 w_, h_;
	float s_;
};
