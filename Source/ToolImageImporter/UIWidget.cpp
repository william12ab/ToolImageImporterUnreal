// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWidget.h"

void UUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//set score here default
	//calss once widget is constructed

	GenerateButton->OnClicked.AddUniqueDynamic(this, &UUIWidget::GeneratePlane);
}

void UUIWidget::GeneratePlane()
{
	p_mesh->CreateMesh();
}