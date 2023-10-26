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

void SaveHandler::LoadArrayFile(FString name_) {
	TArray<int32> result_;
	///FFileHelper::LoadFileToArray(result_, *name_);
}

void SaveHandler::ExtractData(int& num_turns, int& length, FString& time_, const FString& name_) {
	auto meta_name = name_ + "meta.txt";
	auto ar= LoadTextFile(name_);
	if (ar.IsValidIndex(3)) {
		length = FCString::Atoi(*ar[3]);
		num_turns = FCString::Atoi(*ar[4]);
	}
	meta_name = name_ + "time.txt";
	ar = LoadTextFile(name_);
	time_ = ar[0];
}
