#pragma once
class TOOLIMAGEIMPORTER_API FileLoader
{
public:
	FileLoader();
	~FileLoader();

	void OpenApplication();

protected:
	FString name_;
};
