#pragma once
class TOOLIMAGEIMPORTER_API FileLoader
{
public:
	FileLoader();
	~FileLoader();


	void OpenApplication();
	void ReadHeightMapInfo(const FString& heightmap_name);

protected:
	FString name_;

	float image_height_;
	float image_width_;
	TArray<float> height_map_colors;//heightmapcolours

};
