// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
	w_ = 4;
	h_ = 4;
	s_ = 50.0f;
	Label->SetText(FText::FromString("Plane Generator"));

	generate_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClick);
	delete_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickDelete);
	height_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickHeight);

}

void UUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Do your custom tick stuff here]
	OnEnterText();
}

void UUIWidget::GeneratePlane()
{
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = FVector(0,0,112);
	p_mesh = GetWorld()->SpawnActor<AMyProceduralMesh>(myLoc, myRot, SpawnInfo);

	p_mesh->CreateMesh(h_,w_,s_);
}

void UUIWidget::OnClick()
{
	GeneratePlane();
}

void UUIWidget::OnClickDelete()
{
	DeletePlane();
}

void UUIWidget::DeletePlane()
{
	p_mesh->Destroy();
}

void UUIWidget::OnEnterText()
{
	FText height_text = height_label->GetText();
	h_= FCString::Atof(*height_text.ToString());
	FText width_text = width_label->GetText();
	w_ = FCString::Atof(*width_text.ToString());
	FText spacing_text = spacing_label->GetText();
	s_ = FCString::Atof(*spacing_text.ToString());
}


void UUIWidget::OnClickHeight()
{
	p_mesh->ModiVerts();
}