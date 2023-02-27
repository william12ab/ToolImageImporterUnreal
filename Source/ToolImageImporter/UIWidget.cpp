// Fill out your copyright notice in the Description page of Project Settings.


#include "UIWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include <Runtime/Engine/Public/ImageUtils.h>
void UUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
	w_ = 4;
	h_ = 4;
	s_ = 20.0f;
	m_ = 3;
	Label->SetText(FText::FromString("Plane Generator"));

	delete_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickDelete);
	file_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickLoadNewTrack);
	add_texture_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnAddTexture);
	update_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickUpdateButton);
}

void UUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);
	// Do your custom tick stuff here
	OnEnterText();
	ReadSliders();
}

void UUIWidget::OnClickLoadNewTrack()
{
	if (OpenFileWindow()){
		OnClickHeightmapButton();
		LoadTrackPointsIn();
	}
}

void UUIWidget::OnClickUpdateButton()
{
	//update mesh after changing padding
	p_mesh->Destroy();
	GeneratePlane();
	p_mesh->ModiVerts(m_colors, m_);
	p_mesh->SetMaterial(t_);
}

void UUIWidget::GeneratePlane()
{
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = FVector(0,0,0);
	p_mesh = GetWorld()->SpawnActor<AMyProceduralMesh>(myLoc, myRot, SpawnInfo);
	p_mesh->CreateMesh(h_,w_,s_);
}

void UUIWidget::OnClickDelete()
{
	DeletePlane();
}

void UUIWidget::DeletePlane()
{
	if (p_mesh)
	{
		p_mesh->Destroy();
		track_mesh->Destroy();
	}
	
	
	name_.Empty();
	m_colors.Empty();
	track_points.Empty();
}

void UUIWidget::OnEnterText()
{
	FText spacing_text = spacing_label->GetText();
	s_ = FCString::Atof(*spacing_text.ToString());
	FText height_text = height_modi->GetText();
	m_ = FCString::Atof(*height_text.ToString());
}

bool UUIWidget::OpenFileWindow()
{
	FString default_path = "";
	FString dialog_name = "Open Heightmap";
	FString default_file = "";
	FString file_types = "";
	TArray<FString> outfile_names;			//stores the file
	uint32 flags_ = 1;
	IDesktopPlatform* fpl = FDesktopPlatformModule::Get();
	if (fpl->OpenFileDialog(0, dialog_name, default_path, default_file, file_types, flags_, outfile_names)){
		name_.Append(outfile_names[0]);
		return true;
	}
	else { return false; }
}

void UUIWidget::OnClickHeightmapButton()
{
	ReadFileInfo(name_);
}

void UUIWidget::SliderFunc(const int& val_, UEditableTextBox* text_box)
{
	FString string_ = FString::FromInt(int(val_));
	text_box->SetText(FText::FromString(string_));
}

void UUIWidget::ReadSliders()
{
	SliderFunc(modi_slider->GetValue(),height_modi);
	SliderFunc(padding_slider->GetValue(),spacing_label);
	height_label->SetText(FText::FromString(FString::FromInt(h_)));
	width_label->SetText(FText::FromString(FString::FromInt(w_)));
}

void UUIWidget::OnAddTexture()
{
	FString default_path = "";
	FString dialog_name = "Open Texture";
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

void UUIWidget::LoadTrackPointsIn()
{
	FString default_path = "";
	FString dialog_name = "Open New Track";
	FString default_file = "";
	FString file_types = "";
	TArray<FString> outfile_names;			//stores the file
	uint32 flags_ = 1;
	IDesktopPlatform* fpl = FDesktopPlatformModule::Get();
	fpl->OpenFileDialog(0, dialog_name, default_path, default_file, file_types, flags_, outfile_names);

	IPlatformFile& file_manager= FPlatformFileManager::Get().GetPlatformFile();
	
	TArray<FString> array_;
	if (file_manager.FileExists(*outfile_names[0])){
		
		if (FFileHelper::LoadFileToStringArray(array_, *outfile_names[0])){
		}
		else{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not track from file"));
		}
	}
	for (int i = 0; i < array_.Num(); i++){
		auto index_ = array_[0].Find(" ");
		track_points.Add(FVector2D(FCString::Atoi(*array_[i]), FCString::Atoi(*array_[i].Right(index_+1))));
	}
	CreateTrack();
}

void UUIWidget::CreateTrack()
{
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = FVector(0, 0, 0.5f);

	p_mesh->ChangeTest(track_points);
	p_mesh->ModiVerts(m_colors, m_);
	track_mesh = GetWorld()->SpawnActor<ATrackMesh>(myLoc, myRot, SpawnInfo);
	track_mesh->CreateTrack(track_points,m_colors,m_,p_mesh->m_norms);



	int max = 0;
	int min = 100000000;
	for (int i = 0; i < p_mesh->m_verts.Num(); i++) {
		if (p_mesh->m_verts[i].Z > max) {
			max = p_mesh->m_verts[i].Z;
		}
		if (p_mesh->m_verts[i].Z < min) {
			min = p_mesh->m_verts[i].Z;
		}
	}

	for (int i = 0; i < 4; i++) {
		ABasicTree* tree_instancea;
		FActorSpawnParameters SpawnInfoTree;
		FRotator myRotTree(0, 0, 0);
		FVector myLocTree = FVector(0, 0, 0);
		tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
		tree_instancea->AddClusterTrees(p_mesh->m_verts,max,min, track_points,false);
	}
	for (int i = 0; i < 2; i++) {
		ABasicTree* tree_instancea;
		FActorSpawnParameters SpawnInfoTree;
		FRotator myRotTree(0, 0, 0);
		FVector myLocTree = FVector(0, 0, 0);
		tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
		tree_instancea->AddClusterTrees(p_mesh->m_verts, max, min, track_points, true);
	}
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
			float pixel_colour_float = pixel_color.R;
			m_colors.Add(pixel_colour_float);
		}
	}
	texture_->PlatformData->Mips[0].BulkData.Unlock();
	texture_->UpdateResource();
	GeneratePlane();
}
