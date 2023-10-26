// Fill out your copyright notice in the Description page of Project Settings.


#include "FileLoader.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include <Runtime/Engine/Public/ImageUtils.h>
FString FileLoader::application_name_("");
FString FileLoader::folder_name("");
FString FileLoader::folder_extension("");
bool FileLoader::is_loaded_;
bool FileLoader::is_chunked_;
FileLoader::FileLoader() {
	is_opened_ = false;
	count = 0;
	time_to_save = "";

}

FileLoader::~FileLoader() {
}

bool ReadFileInfoApp(const FString& dialog_name_, FString& file_name) {
	FString default_path = "";
	FString dialog_name = dialog_name_;
	FString default_file = "";
	FString file_types = "";
	TArray<FString> outfile_names;//stores the file
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
	FString dialog_name = "Open Application";
	FString return_name;
	if (ReadFileInfoApp(dialog_name, return_name)) {
		is_opened_ = true;
		application_name_ = return_name;
		const TCHAR* name_char_ = *application_name_;
		FProcHandle Proc = FPlatformProcess::CreateProc(name_char_, nullptr, true, false, false, nullptr, 0, *FPaths::GetPath(application_name_), nullptr);
		FPlatformProcess::WaitForProc(Proc);
		if (Proc.IsValid()) {
			FPlatformProcess::CloseProc(Proc);
		}
	}
	FString* address = &application_name_;
}

void FileLoader::CreateNewFolder() {
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString path_ = FPaths::ProjectContentDir();

	
	SetCount();
	path_.Append("saved_tracks/");
	path_.Append(FString::FromInt(count)+"/");
	if (!platformFile.DirectoryExists(*path_)) {
		if (platformFile.CreateDirectory(*path_)) {
			SaveFiles(path_);
		}
	}
}

void FileLoader::SaveFiles(const FString& path_way) {
	CopyAndSave(path_way, "meta.txt");
	CopyAndSave(path_way, "pacenote_info.txt");
	int index = 1;
	if (is_chunked_) {
		index = 4;
	}
	for (size_t i = 0; i < index; i++) {
		FString track_name = "track_points.txt";
		track_name = FString::FromInt(i) + track_name;
		CopyAndSave(path_way, track_name);
		track_name = "track_image.png";
		track_name = FString::FromInt(i) + track_name;
		CopyAndSave(path_way, track_name);
	}
	FString time_file = path_way + "time.txt";
	FFileHelper::SaveStringToFile(time_to_save, *time_file);
	count++;
	CreateMetaFile();
}

void FileLoader::CopyAndSave(const FString& path_, const FString& file_to_save) {
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	FString exe_loc = application_name_;
	FString exe_name = "SFML_RuleBasedSystem.exe";
	auto n = exe_loc.Find(FString(exe_name));
	exe_loc.RemoveAt(n, exe_name.Len());
	FString loc_path = path_ + file_to_save;
	exe_loc += file_to_save;
	if (FileManager.CopyFile(*loc_path, *exe_loc, EPlatformFileRead::None, EPlatformFileWrite::None)){
		UE_LOG(LogTemp, Warning, TEXT("FilePaths: File Copied!"));
	}
}


void FileLoader::CreateMetaFile() {
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString path_ = FPaths::ProjectContentDir();
	path_.Append("saved_tracks/");
	path_.Append("meta_.txt");
	if (platformFile.FileExists(*path_)){
		//
		FFileHelper::SaveStringToFile(FString::FromInt(count), *path_);
	}
	else {
		FFileHelper::SaveStringToFile(FString::FromInt(count), *path_, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}	
}

void FileLoader::SetCount() {
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString path_ = FPaths::ProjectContentDir();
	path_.Append("saved_tracks/");
	path_.Append("meta_.txt");
	if (platformFile.FileExists(*path_)) {
		FString temp;
		FFileHelper::LoadFileToString(temp, *path_, FFileHelper::EHashOptions::None);
		count = FCString::Atoi(*temp);
	}
}