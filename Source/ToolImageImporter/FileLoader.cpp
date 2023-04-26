// Fill out your copyright notice in the Description page of Project Settings.


#include "FileLoader.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include <Runtime/Engine/Public/ImageUtils.h>
FileLoader::FileLoader()
{
}

FileLoader::~FileLoader()
{
}

bool ReadFileInfoApp(const FString& dialog_name_, FString& file_name) {
	FString default_path = "";
	FString dialog_name = dialog_name_;
	FString default_file = "";
	FString file_types = "";
	TArray<FString> outfile_names;			//stores the file
	uint32 flags_ = 1;
	IDesktopPlatform* fpl = FDesktopPlatformModule::Get();
	if (fpl->OpenFileDialog(0, dialog_name, default_path, default_file, file_types, flags_, outfile_names)) {
		file_name = outfile_names[0];
		return true;
	}
	else {
		return false;
	}
	
}
void FileLoader::OpenApplication() {
	FString dialog_name = "Open Heightmap";
	FString return_name;
	if (ReadFileInfoApp(dialog_name, return_name)) {
		name_ = return_name;
		const TCHAR* name_char_ = *name_;
		//FPlatformProcess::CreateProc(name_char_, nullptr, true, false, false, nullptr, 0, *FPaths::GetPath(name_), nullptr);
		FProcHandle Proc = FPlatformProcess::CreateProc(name_char_, nullptr, true, false, false, nullptr, 0, *FPaths::GetPath(name_), nullptr);
		FPlatformProcess::WaitForProc(Proc);
		if (Proc.IsValid()){
			FPlatformProcess::CloseProc(Proc);
		}
	
	}
	
	//C:/Users/willu/Desktop/SFML_RBS/Executable/final.png
}

void FileLoader::ReadHeightMapInfo(const FString& heightmap_name) {
	UTexture2D* texture_ = FImageUtils::ImportFileAsTexture2D(heightmap_name);
	texture_->AddToRoot();

	//since grey scale already. each rgb component should be greyscale value. therefore no need to add up and divide by 3.
	//rgb comp is uint8 value. so, use this as the height and modifiy the height of terrain.
	const FColor* formated_image_data = static_cast<const FColor*>(texture_->PlatformData->Mips[0].BulkData.LockReadOnly());
	image_height_ = texture_->PlatformData->Mips[0].SizeY;
	image_width_ = texture_->PlatformData->Mips[0].SizeX;
	for (int32 y_ = 0; y_ < image_height_; y_++) {
		for (int32 x_ = 0; x_ < image_width_; x_++) {
			FColor pixel_color = formated_image_data[y_ * texture_->GetSizeX() + x_]; // Do the job with the pixel
			float pixel_colour_float = pixel_color.R;
			height_map_colors.Add(pixel_colour_float);
		}
	}
	texture_->PlatformData->Mips[0].BulkData.Unlock();
	texture_->UpdateResource();
}

