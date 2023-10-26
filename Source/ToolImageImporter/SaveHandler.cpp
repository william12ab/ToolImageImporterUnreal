#include "SaveHandler.h"
SaveHandler::SaveHandler(){
}

SaveHandler::~SaveHandler(){
}

TArray<FString> SaveHandler::LoadTextFile(const FString& name_) {
	TArray<FString> array_;
	FFileHelper::LoadFileToStringArray(array_, *name_, FFileHelper::EHashOptions::None);
	return array_;
}

void SaveHandler::ExtractData(int& num_turns, int& length, FString& time_, const FString& name_) {
	auto meta_name = name_ + "meta.txt";
	auto ar= LoadTextFile(meta_name);
	if (ar.IsValidIndex(3)) {
		length = FCString::Atoi(*ar[3]);
		num_turns = FCString::Atoi(*ar[4]);
	}
	meta_name = name_ + "time.txt";
	ar = LoadTextFile(meta_name);
	time_ = ar[0];
}

void SaveHandler::CheckForExist(int& local_count, const FString&path_) {
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (platformFile.DirectoryExists(*path_)) {
		local_count++;
	}
}
