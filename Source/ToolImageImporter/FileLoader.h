#pragma once
class TOOLIMAGEIMPORTER_API FileLoader
{
public:
	FileLoader();
	~FileLoader();

	void OpenApplication();

	void SetFolderName(FString a) { application_name_ = a; };
	void SetIsLoaded(bool b) { is_loaded_ = b; }
	void SetIsSaved(bool b) { is_saved_ = b; }
	void SetExtension(FString a) { folder_extension = a; }
	void SetTimeToSave(FString a) { time_to_save=a; }
	void SetIsChunked(bool b) { is_chunked_ = b; }
	FString& GetFileName() { return application_name_; }
	FString& GetFileExtension() { return folder_extension; }
	bool GetIsOpened() { return is_opened_; }
	bool GetIsLoaded() { return is_loaded_; }
	bool GetIsSaved() { return is_saved_; }
	int GetCount() { return count; }


	void CreateNewFolder();
	void CreateMetaFile();
	void SetCount();
	void CopyAndSave(const FString& path_, const FString& file_to_save);

	void SaveFiles(const FString& path_way);
protected:
	static FString application_name_;
	static FString folder_name;
	static FString folder_extension;

	bool is_opened_;
	static bool is_loaded_;
	static bool is_chunked_;
	static bool is_saved_;

	int count;

	FString time_to_save;
};
