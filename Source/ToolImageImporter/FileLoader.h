#pragma once
class TOOLIMAGEIMPORTER_API FileLoader
{
public:
	FileLoader();
	~FileLoader();

	void OpenApplication();
	
	void SetFolderName(FString a) { folder_name = a; };

	FString &GetFileName() { return application_name_; }
	bool GetIsOpened() { return is_opened_;}
protected:
	static FString application_name_;
	bool is_opened_;

	static FString folder_name;
};
