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
FileLoader::FileLoader(){
	is_opened_ = false;
}

FileLoader::~FileLoader(){
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
		if (Proc.IsValid()){
			FPlatformProcess::CloseProc(Proc);
		}
	}
	FString* address = &application_name_;
}
