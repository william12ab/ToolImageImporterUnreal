#include "HeightmapHandler.h"
#include <Runtime/Engine/Public/ImageUtils.h>
#include <AssetRegistryModule.h>
#include "Kismet/KismetRenderingLibrary.h"


HeightmapHandler::HeightmapHandler() {
	FString exe_name = "SFML_RuleBasedSystem.exe";
	if (!f_l.GetIsLoaded()) {

	}
	else {
		exe_name = "0track_image.png";
	}
	auto fil_name = f_l.GetFileName();
	auto n = fil_name.Find(FString(exe_name));
	fil_name.RemoveAt(n, exe_name.Len());
	track_points_name = fil_name += "track_points.txt";
	fil_name = f_l.GetFileName();
	if (f_l.GetIsLoaded()) {
		n = fil_name.Find(FString(exe_name));
		fil_name.RemoveAt(n, exe_name.Len());
		auto temp_file_name = fil_name;
		auto b = temp_file_name.Find(FString(f_l.GetFileExtension()));
		temp_file_name.RemoveAt(b, f_l.GetFileExtension().Len());
		temp_file_name.RemoveFromEnd(FString("/"));
		temp_file_name = temp_file_name + "heightmap/";
		heightmap_name = temp_file_name += "final.png";
	}
	else {
		n = fil_name.Find(FString(exe_name));
		fil_name.RemoveAt(n, exe_name.Len());
		heightmap_name = fil_name += "final.png";
	}


	fil_name = f_l.GetFileName();
	n = fil_name.Find(FString(exe_name));
	fil_name.RemoveAt(n, exe_name.Len());
	meta_file_name = fil_name += "meta.txt";
	is_chunking = false;

	fil_name = f_l.GetFileName();
	n = fil_name.Find(FString(exe_name));
	fil_name.RemoveAt(n, exe_name.Len());
	pacenote_file_name = fil_name += "pacenote_info.txt";

	total_length = 0;
	number_of_turns = 0;
}
HeightmapHandler::~HeightmapHandler() {
}


void HeightmapHandler::GetTrackImageName(const int& index_) {
	auto fil_name = f_l.GetFileName();
	FString exe_name = "SFML_RuleBasedSystem.exe";
	auto nt = fil_name.Find(FString("SFML_RuleBasedSystem.exe"));
	fil_name = f_l.GetFileName();
	nt = fil_name.Find(FString("SFML_RuleBasedSystem.exe"));
	fil_name.RemoveAt(nt, exe_name.Len());
	track_image_name = fil_name += "track_image.png";
	//getting the folder location

	auto n = track_image_name.Find(FString("track_image.png"));
	track_image_name.InsertAt(n, FString::FromInt(index_));
}

UTexture2D* HeightmapHandler::LoadImage(const int& index_) {
	UTexture2D* texture_ = FImageUtils::ImportFileAsTexture2D(track_image_name);
	return texture_;
}
bool HeightmapHandler::ReadMetaTracK( const int& index_) {
	meta_file_name = track_image_name;
	FString track_name = FString::FromInt(index_) + FString("track_image.png");
	auto index_t = meta_file_name.Find(track_name);
	meta_file_name.RemoveAt(index_t, track_name.Len());
	meta_file_name = meta_file_name + "meta.txt";
	auto s = ReadMetaFile();
	return s;
}

void HeightmapHandler::ReturnColor(UTexture2D* texture_, TArray<FColor>& color_array, const int& x_add, const int& y_add, const int& grid_size) {
	const FColor* formated_image_data = static_cast<const FColor*>(texture_->PlatformData->Mips[0].BulkData.LockReadOnly());
	heightmap_h_ = texture_->PlatformData->Mips[0].SizeY;
	heightmap_w_ = texture_->PlatformData->Mips[0].SizeX;
	for (int32 y_ = 0; y_ < heightmap_h_; y_++) {
		for (int32 x_ = 0; x_ < heightmap_w_; x_++) {
			FColor pixel_color = formated_image_data[y_ * texture_->GetSizeX() + x_]; // Do the job with the pixel
			color_array[(y_+y_add) *grid_size + (x_+ x_add)]=pixel_color;
		}
	}
	texture_->PlatformData->Mips[0].BulkData.Unlock();
	texture_->UpdateResource();
}

uint8* HeightmapHandler::CreatePixels(const int& height, TArray<FColor>& color_array, const bool& loc_chunk) {
	int grid_size = 400;
	if (loc_chunk){
		grid_size = 800;
	}
	uint8* Pixels = new uint8[height * height * 4];
	for (int32 y = 0; y < height; y++) {
		for (int32 x = 0; x < height; x++) {
			int32 curPixelIndex = ((y * height) + x);
			if (x < grid_size && y < grid_size) {
				Pixels[4 * curPixelIndex] = color_array[y * grid_size + x].B;
				Pixels[4 * curPixelIndex + 1] = color_array[y * grid_size + x].G;
				Pixels[4 * curPixelIndex + 2] = color_array[y * grid_size + x].R;
				Pixels[4 * curPixelIndex + 3] = color_array[y * grid_size + x].A;
			}
			else {
				Pixels[4 * curPixelIndex] = 0;
				Pixels[4 * curPixelIndex + 1] = 0;
				Pixels[4 * curPixelIndex + 2] = 0;
				Pixels[4 * curPixelIndex + 3] = 255;
			}
		}
	}
	return Pixels;
}

UTexture2D* HeightmapHandler::CreateNewTexture(const int& height_, const FString& name_, UPackage* package_) {
	UTexture2D* NewTexture = NewObject<UTexture2D>(package_, *name_, RF_Public | RF_Standalone | RF_MarkAsRootSet);
	NewTexture->AddToRoot();				// This line prevents garbage collection of the texture
	NewTexture->PlatformData = new FTexturePlatformData();	// Then we initialize the PlatformData

	NewTexture->PlatformData->SizeX = height_;
	NewTexture->PlatformData->SizeY = height_;
	NewTexture->PlatformData->SetNumSlices(1);
	NewTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;
	return NewTexture;
}

UTexture2D* HeightmapHandler::FromSelect(const FString& name_) {
	//	load it
	UTexture2D* tmpTex = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *name_));
	return tmpTex;
}

void HeightmapHandler::ReadTrackImage(const int& index_, UObject* world_) {
	int height_ = 512;
	int local_index = 1;
	TArray<FColor>color_array;
	int grid_size = 400;
	GetTrackImageName(index_);//gets the name w/ extension
	bool is_chunking_loc = ReadMetaTracK(index_);//checks if chunking
	if (is_chunking_loc) {
		grid_size = 800;
		local_index = 4;
		color_array.SetNum(grid_size * grid_size);
		height_ = 1024;
	}
	else {
		color_array.SetNum(grid_size * grid_size);
	}

	for (int i = 0; i < local_index; i++) {
		GetTrackImageName(i);
		UTexture2D* texture_ = LoadImage(i);//loads texture
		int x_addition = 0;
		int y_addition = 0;
		if (is_chunking_loc) {
			switch (i)
			{
			case 0: {
				break;
			}
			case 1: {
				x_addition = 400;
				break;
			}
			case 2: {
				y_addition = 400;
				break;
			}
			case 3: {
				x_addition = 400;
				y_addition = 400;
				break;
			}
			}
		}
		ReturnColor(texture_, color_array, x_addition, y_addition, grid_size);//returns color array
	}

	//creates the package
	FString PackageName = TEXT("/Game/default_tracks/");
	FString TextureName = "text";
	PackageName += TextureName;
	UPackage* Package = CreatePackage(NULL, *PackageName);
	Package->FullyLoad();

	//creates new texture
	UTexture2D* new_texture = CreateNewTexture(height_, TextureName, Package);

	//creates pixe;s
	auto pixels_ = CreatePixels(height_, color_array, is_chunking_loc);

	FTexture2DMipMap* Mip = new(new_texture->PlatformData->Mips) FTexture2DMipMap();
	Mip->SizeX = height_;
	Mip->SizeY = height_;

	//saving the final product
	Mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* TextureData = (uint8*)Mip->BulkData.Realloc(height_ * height_ * 4);
	FMemory::Memcpy(TextureData, pixels_, sizeof(uint8) * height_ * height_ * 4);
	Mip->BulkData.Unlock();
	new_texture->Source.Init(height_, height_, 1, 0, ETextureSourceFormat::TSF_BGRA8, pixels_);
	
	new_texture->UpdateResource();
	Package->MarkPackageDirty();
	FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	bool bSaved = UPackage::SavePackage(Package, new_texture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);
	delete[] pixels_;
}

TArray<float> HeightmapHandler::ReadFileInfo(int& height_, int& width_, const int& index_) {
	auto n = heightmap_name.Find(FString("final.png"));
	if (index_ > 0) {
		heightmap_name.RemoveAt(n - 1);
		n = heightmap_name.Find(FString("final.png"));
	}
	heightmap_name.InsertAt(n, FString::FromInt(index_));
	UTexture2D* texture_ = FImageUtils::ImportFileAsTexture2D(heightmap_name);
	texture_->AddToRoot();

	TArray<float> temp_arr;
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
	if (array_[0].Contains(FString("true"))) {
		is_chunking = true;
	}
	else {
		is_chunking = false;
	}
	auto index_ = array_[1].Find(" ");
	start_pos = (FVector2D(FCString::Atoi(*array_[1]), FCString::Atoi(*array_[1].RightChop(index_))));
	index_ = array_[2].Find(" ");
	end_pos = (FVector2D(FCString::Atoi(*array_[2]), FCString::Atoi(*array_[2].RightChop(index_))));
	total_length = FCString::Atoi(*array_[3]);
	number_of_turns = FCString::Atoi(*array_[4]);
	return is_chunking;
}

void HeightmapHandler::ReadPaceNoteFile(TArray<int>& lengths_, TArray<int>& angles_, TArray<int>& inclines_, TArray<int>& widths_, TArray<int>& directions_, TArray<int>& cp_width_) {
	IPlatformFile& file_manager = FPlatformFileManager::Get().GetPlatformFile();
	TArray<FString> array_;
	if (file_manager.FileExists(*pacenote_file_name)) {
		if (FFileHelper::LoadFileToStringArray(array_, *pacenote_file_name)) {
		}//yes
		else {
		}//no
	}
	int chooser = 0;
	for (size_t i = 1; i < array_.Num(); i++) {//skip first element because it is indication
		bool is_chosen = false;
		if (array_[i].Contains(FString("d"))) {
			chooser = 4;
			is_chosen = true;
		}
		if (array_[i].Contains(FString("l"))) {
			chooser = 1;
			is_chosen = true;
		}
		else if (array_[i].Contains(FString("i"))) {
			chooser = 2;
			is_chosen = true;
		}
		else if (array_[i].Contains(FString("w"))) {
			chooser = 3;
			is_chosen = true;
		}
		else if (array_[i].Contains(FString("n"))) {
			chooser = 5;
			is_chosen = true;
		}
		else if (array_[i].Contains(FString("c"))) {
			chooser = 6;
			is_chosen = true;
		}
		if (!is_chosen) {
			switch (chooser)
			{
			case 0: {//angle
				angles_.Add(FCString::Atoi(*array_[i]));
				break;
			}
			case 1: {//length
				lengths_.Add(FCString::Atoi(*array_[i]));
				break;
			}
			case 2: {//incline
				inclines_.Add(FCString::Atoi(*array_[i]));
				break;
			}
			case 3: {//w
				widths_.Add(FCString::Atoi(*array_[i]));
				break;
			}
			case 4: {//w
				directions_.Add(FCString::Atoi(*array_[i]));
				break;
			}
			case 6: {//control point width
				cp_width_.Add(FCString::Atoi(*array_[i]));
				break;
			}
			}
		}
	}
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
					auto index_ = array_[i].Find(" ");
					control_points.Add(FVector2D(FCString::Atoi(*array_[i]), FCString::Atoi(*array_[i].RightChop(index_))));
				}
				else {
					auto index_ = array_[i].Find(" ");
					track_points.Add(FVector2D(FCString::Atoi(*array_[i]), FCString::Atoi(*array_[i].RightChop(index_))));
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


FVector2D HeightmapHandler::GetStartEndPos(int a) {
	FVector2D return_val;
	if (a == 0) {
		return_val = start_pos;
	}
	else {
		return_val = end_pos;
	}
	return return_val;
}