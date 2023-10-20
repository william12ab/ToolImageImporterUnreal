#pragma once
class TOOLIMAGEIMPORTER_API FileLoader
{
public:
	FileLoader();
	~FileLoader();

	void OpenApplication();

	void SetFolderName(FString a) { application_name_ = a; };
	void SetIsLoaded(bool b) { is_loaded_ = b; }
	void SetExtension(FString a) { folder_extension = a; }
	FString& GetFileName() { return application_name_; }
	FString& GetFileExtension() { return folder_extension; }
	bool GetIsOpened() { return is_opened_; }
	bool GetIsLoaded() { return is_loaded_; }
protected:
	static FString application_name_;
	bool is_opened_;

	static FString folder_name;
	static FString folder_extension;
	static bool is_loaded_;
};
