// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "FileLoader.h"
class TOOLIMAGEIMPORTER_API HeightmapHandler
{
public:
	HeightmapHandler();
	~HeightmapHandler();

	TArray<float> ReadFileInfo(int &height_, int &width_);//reads file for height map returns array of float of colours
	bool ReadTrackPoints(TArray<FVector2D>&track_points,TArray<FVector2D> &control_points);//returns filled arrays and bool of what type track(used in construction)

	FileLoader f_l;

protected:
	int heightmap_h_;//height,width
	int heightmap_w_;
	FString heightmap_name;//filenames of heightmap and trackpoints:)
	FString track_points_name;
};
