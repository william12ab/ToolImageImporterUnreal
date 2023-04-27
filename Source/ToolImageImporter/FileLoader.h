#pragma once
class TOOLIMAGEIMPORTER_API FileLoader
{
public:
	FileLoader();
	~FileLoader();

	void OpenApplication();
	
	FString &GetFileName() { return name_; }
	bool GetIsOpened() { return is_opened_;}
protected:
	static FString name_;
	bool is_opened_;

};
