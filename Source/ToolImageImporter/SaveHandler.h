#pragma once
class TOOLIMAGEIMPORTER_API SaveHandler
{
public:
	SaveHandler();
	~SaveHandler();

	TArray<FString> LoadTextFile(const FString& name_);
	void LoadArrayFile(FString name_);

	void ExtractData(int&num_turns, int&length, FString&time_, const FString& name_);

	void CheckForExist(int& local_count, const FString& path_);
};

