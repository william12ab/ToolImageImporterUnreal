// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include <Runtime/Engine/Public/ImageUtils.h>
void UUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
	w_ = 4;
	h_ = 4;
	s_ = 50.0f;
	m_ = 1;
	Label->SetText(FText::FromString("Plane Generator"));

	generate_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClick);
	delete_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickDelete);
	file_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnFileButton);
	create_heightmap_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickHeightmapButton);
	add_texture_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnAddTexture);
}

void UUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);
	// Do your custom tick stuff here
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
	name_.Empty();
}

void UUIWidget::OnEnterText()
{
	FText height_text = height_label->GetText();
	h_= FCString::Atof(*height_text.ToString());
	FText width_text = width_label->GetText();
	w_ = FCString::Atof(*width_text.ToString());
	FText spacing_text = spacing_label->GetText();
	s_ = FCString::Atof(*spacing_text.ToString());
	FText modi_text = height_modi->GetText();
	m_ = FCString::Atof(*modi_text.ToString());
}

void UUIWidget::OnFileButton()
{
	OpenFileWindow();
}

void UUIWidget::OpenFileWindow()
{
	FString default_path = "";
	FString dialog_name = "";
	FString default_file = "";
	FString file_types = "";
	TArray<FString> outfile_names;			//stores the file
	uint32 flags_ = 1;
	IDesktopPlatform* fpl = FDesktopPlatformModule::Get();
	fpl->OpenFileDialog(0, dialog_name, default_path, default_file, file_types, flags_, outfile_names);
	name_.Append(outfile_names[0]);
	Label->SetText(FText::FromString(name_));
}

void UUIWidget::CreateHeightmap()
{
	ReadFileInfo(name_);
}

void UUIWidget::OnClickHeightmapButton()
{
	CreateHeightmap();
}

void UUIWidget::OnAddTexture()
{
	FString default_path = "";
	FString dialog_name = "";
	FString default_file = "";
	FString file_types = "";
	TArray<FString> outfile_names;			//stores the file
	uint32 flags_ = 1;
	IDesktopPlatform* fpl = FDesktopPlatformModule::Get();
	fpl->OpenFileDialog(0, dialog_name, default_path, default_file, file_types, flags_, outfile_names);
	UTexture2D* texture_ = FImageUtils::ImportFileAsTexture2D(outfile_names[0]);
	texture_->AddToRoot();
	t_ = texture_;
	t_->UpdateResource();
	p_mesh->SetMaterial(t_);
}

void UUIWidget::ReadFileInfo(const FString& name__)
{
	UTexture2D* texture_ = FImageUtils::ImportFileAsTexture2D(name__);
	texture_->AddToRoot();
	
	//since grey scale already. each rgb component should be greyscale value. therefore no need to add up and divide by 3.
	//rgb comp is uint8 value. so, use this as the height and modifiy the height of terrain.
	const FColor* formated_image_data = static_cast<const FColor*>(texture_->PlatformData->Mips[0].BulkData.LockReadOnly());
	h_ = texture_->PlatformData->Mips[0].SizeY;
	w_ = texture_->PlatformData->Mips[0].SizeX;
	for (int32 y_ = 0; y_ < h_; y_++) {
		for (int32 x_ = 0; x_ < w_; x_++) {
			FColor pixel_color = formated_image_data[y_ * texture_->GetSizeX() + x_]; // Do the job with the pixel
			m_colors.Add(pixel_color.R);
		}
	}
	texture_->PlatformData->Mips[0].BulkData.Unlock();
	texture_->UpdateResource();

	GeneratePlane();
	p_mesh->ModiVerts(m_colors,m_);
}