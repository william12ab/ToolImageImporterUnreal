#include "HeightmapHandler.h"
#include <Runtime/Engine/Public/ImageUtils.h>

HeightmapHandler::HeightmapHandler(){
}
HeightmapHandler::~HeightmapHandler(){
}

TArray<float> HeightmapHandler::ReadFileInfo(const FString& name_, int& height_, int& width_) {
	UTexture2D* texture_ = FImageUtils::ImportFileAsTexture2D(name_);
	texture_->AddToRoot();

	TArray<float> temp_arr;

	//since grey scale already. each rgb component should be greyscale value. therefore no need to add up and divide by 3.
	//rgb comp is uint8 value. so, use this as the height and modifiy the height of terrain.
	const FColor* formated_image_data = static_cast<const FColor*>(texture_->PlatformData->Mips[0].BulkData.LockReadOnly());
	h_ = texture_->PlatformData->Mips[0].SizeY;
	w_ = texture_->PlatformData->Mips[0].SizeX;
	for (int32 y_ = 0; y_ < h_; y_++) {
		for (int32 x_ = 0; x_ < w_; x_++) {
			FColor pixel_color = formated_image_data[y_ * texture_->GetSizeX() + x_]; // Do the job with the pixel
			float pixel_colour_float = pixel_color.R;
			temp_arr.Add(pixel_colour_float);
		}
	}
	texture_->PlatformData->Mips[0].BulkData.Unlock();
	texture_->UpdateResource();
	height_ = h_;
	width_ = w_;
	return temp_arr;
}


bool HeightmapHandler::ReadTrackPoints(const FString& name_, TArray<FVector2D>& track_points, TArray<FVector2D>& control_points) {
	bool point_type;
	IPlatformFile& file_manager = FPlatformFileManager::Get().GetPlatformFile();
	TArray<FString> array_;
	if (file_manager.FileExists(*name_)) {
		if (FFileHelper::LoadFileToStringArray(array_, *name_)) {
		}
		else {
		}
	}
	bool found_ = false;
	for (int i = 0; i < array_.Num(); i++) {
		if (i > 0) {
			if (array_[i].Contains(FString("end"))) {
				found_ = true;
			}
			else {
				if (!found_) {
					auto index_ = array_[1].Find(" ");
					control_points.Add(FVector2D(FCString::Atoi(*array_[i]), FCString::Atoi(*array_[i].Right(index_ + 1))));
				}
				else {
					auto index_ = array_[1].Find(" ");
					track_points.Add(FVector2D(FCString::Atoi(*array_[i]), FCString::Atoi(*array_[i].Right(index_ + 1))));
				}
			}
		}
		else {
			if (array_[0] == "t.p") {
				point_type = true;
			}
			else {
				point_type = false;
			}
		}
	}

	return point_type;
}