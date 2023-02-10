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
	update_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickUpdateButton);
	load_file_track_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::LoadTrackPointsIn);
}

void UUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);
	// Do your custom tick stuff here
	OnEnterText();
	ReadSliders();
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
	FText spacing_text = spacing_label->GetText();
	s_ = FCString::Atof(*spacing_text.ToString());
	FText height_text = height_modi->GetText();
	m_ = FCString::Atof(*height_text.ToString());
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

void UUIWidget::LoadTrackPointsIn()
{
	FString default_path = "";
	FString dialog_name = "";
	FString default_file = "";
	FString file_types = "";
	TArray<FString> outfile_names;			//stores the file
	uint32 flags_ = 1;
	IDesktopPlatform* fpl = FDesktopPlatformModule::Get();
	fpl->OpenFileDialog(0, dialog_name, default_path, default_file, file_types, flags_, outfile_names);

	IPlatformFile& file_manager= FPlatformFileManager::Get().GetPlatformFile();
	
	TArray<FString> array_;
	if (file_manager.FileExists(*outfile_names[0]))
	{
		
		if (FFileHelper::LoadFileToStringArray(array_, *outfile_names[0])){
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: size of File: %d"), array_.Num());
		}
		else{
			UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Did not load text from file"));
		}
	}
	for (int i = 0; i < array_.Num(); i++)
	{
		auto index_ = array_[0].Find(" ");
		track_points.Add(FVector2D(FCString::Atoi(*array_[i]), FCString::Atoi(*array_[i].Right(index_+1))));
	}
	CreateTrack();
}

float UUIWidget::AngleCalculator(FVector2D& p1, FVector2D& p2)
{
	if (p1==p2){
		return 0;
		UE_LOG(LogTemp, Warning, TEXT("case"));
	}
	else{
		auto p3 = FVector2D(p2.X, p1.Y);

		float lx = p3.X - p1.X;
		float ly = p2.Y - p3.Y;

		float theta = ly / lx;
		float angle = atan(theta) * 180.0f / 3.14159265;
		return angle;
	}	
}

void UUIWidget::CreateTrack()
{
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = FVector(0, 0, 112);
	track_= GetWorld()->SpawnActor<ATrackInstance>(myLoc, myRot, SpawnInfo);


	float angle_ = 0;
	for (int i = 0; i < track_points.Num(); i++){
		if (i==track_points.Num()-1)	{

		}
		else{
			angle_ = 0;// AngleCalculator(track_points[i], track_points[i + 1]);
		}
		FTransform scale_ = SetTranslationActor(FVector((int)track_points[i].X, (int)track_points[i].Y, (m_colors[(int)track_points[i].Y * h_ + (int)track_points[i].X] * s_) / m_), FVector(1.05f, 1.050f, 1.050f), FRotator(angle_, 0, 0));
		track_->AddTrackComp(scale_);
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
			m_colors.Add(pixel_color.R);
		}
	}
	texture_->PlatformData->Mips[0].BulkData.Unlock();
	texture_->UpdateResource();

	GeneratePlane();
	p_mesh->ModiVerts(m_colors,m_);
}

FTransform UUIWidget::SetTranslationActor(FVector position_vector, FVector scale_vector, FRotator rotation_rotator)
{
	FTransform local_trans = FTransform(FVector(0,0,0));
	local_trans.AddToTranslation(position_vector);
	FTranslationMatrix mat_translation_(local_trans.GetLocation());
	FVector scale_ = scale_vector;
	FScaleMatrix mat_scale_(scale_);
	FRotator rot_ = rotation_rotator;
	FRotationMatrix mat_rot_(rot_);
	FMatrix mat_final = mat_scale_ * mat_rot_ * mat_translation_;
	return FTransform(mat_final);
}