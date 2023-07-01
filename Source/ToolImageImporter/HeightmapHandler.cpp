#include "HeightmapHandler.h"
#include <Runtime/Engine/Public/ImageUtils.h>

HeightmapHandler::HeightmapHandler(){
	auto fil_name = f_l.GetFileName();
	FString exe_name = "SFML_RuleBasedSystem.exe";
	auto n = fil_name.Find(FString("SFML_RuleBasedSystem.exe"));
	fil_name.RemoveAt(n, exe_name.Len());
	track_points_name = fil_name += "track_points.txt";
	fil_name = f_l.GetFileName();
	exe_name = "SFML_RuleBasedSystem.exe";
	n = fil_name.Find(FString("SFML_RuleBasedSystem.exe"));
	fil_name.RemoveAt(n, exe_name.Len());
	heightmap_name = fil_name += "final.png";

	fil_name = f_l.GetFileName();
	n = fil_name.Find(FString("SFML_RuleBasedSystem.exe"));
	fil_name.RemoveAt(n, exe_name.Len());
	meta_file_name = fil_name += "meta.txt";
	is_chunking = false;
}
HeightmapHandler::~HeightmapHandler(){
}

TArray<float> HeightmapHandler::ReadFileInfo(int& height_, int& width_, const int& index_) {
	auto n = heightmap_name.Find(FString("final.png"));
	if (index_>0){
		heightmap_name.RemoveAt(n-1);
		n = heightmap_name.Find(FString("final.png"));
	}
	heightmap_name.InsertAt(n, FString::FromInt(index_));
	UTexture2D* texture_ = FImageUtils::ImportFileAsTexture2D(heightmap_name);
	texture_->AddToRoot();

	TArray<float> temp_arr;
	//since grey scale already. each rgb component should be greyscale value. therefore no need to add up and divide by 3.
	//rgb comp is uint8 value. so, use this as the height and modifiy the height of terrain.
	const FColor* formated_image_data = static_cast<const FColor*>(texture_->PlatformData->Mips[0].BulkData.LockReadOnly());
	heightmap_h_ = texture_->PlatformData->Mips[0].SizeY;
	heightmap_w_ = texture_->PlatformData->Mips[0].SizeX;
	for (int32 y_ = 0; y_ < heightmap_h_; y_++) {
		for (int32 x_ = 0; x_ < heightmap_w_; x_++) {
			FColor pixel_color = formated_image_data[y_ * texture_->GetSizeX() + x_]; // Do the job with the pixel
			float pixel_colour_float = pixel_color.R;
			temp_arr.Add(pixel_colour_float);
		}
	}
	texture_->PlatformData->Mips[0].BulkData.Unlock();
	texture_->UpdateResource();
	height_ = heightmap_h_;
	width_ = heightmap_w_;
	return temp_arr;
}
bool HeightmapHandler::ReadMetaFile() {
	IPlatformFile& file_manager = FPlatformFileManager::Get().GetPlatformFile();
	TArray<FString> array_;
	if (file_manager.FileExists(*meta_file_name)) {
		if (FFileHelper::LoadFileToStringArray(array_, *meta_file_name)) {
		}//yes
		else {
		}//no
	}
	if (array_[0].Contains(FString("true"))){
		is_chunking = true;
	}
	else {
		is_chunking = false;
	}
	return is_chunking;
}

bool HeightmapHandler::ReadTrackPoints(TArray<FVector2D>& track_points, TArray<FVector2D>& control_points, const int& index_p) {
	bool point_type;
	IPlatformFile& file_manager = FPlatformFileManager::Get().GetPlatformFile();
	TArray<FString> array_;
	auto n = track_points_name.Find(FString("track_points.txt"));
	if (index_p > 0) {
		track_points_name.RemoveAt(n - 1);
		n = track_points_name.Find(FString("track_points.txt"));
	}
	track_points_name.InsertAt(n, FString::FromInt(index_p));
	if (file_manager.FileExists(*track_points_name)) {
		if (FFileHelper::LoadFileToStringArray(array_, *track_points_name)) {
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