// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "FileLoader.h"
class TOOLIMAGEIMPORTER_API HeightmapHandler
{
public:
	HeightmapHandler();
	~HeightmapHandler();

	TArray<float> ReadFileInfo(int &height_, int &width_, const int&index_);//reads file for height map returns array of float of colours
	bool ReadTrackPoints(TArray<FVector2D>&track_points,TArray<FVector2D> &control_points, const int& index_p);//returns filled arrays and bool of what type track(used in construction)
	bool ReadMetaFile();
	void ReadPaceNoteFile(TArray<int>&lengths_, TArray<int>&angles_,TArray<int>&inclines_, TArray<int>&widths_, TArray<int>& directions_, TArray<int>&cp_width_);
	FileLoader f_l;

	FVector2D GetStartEndPos(int a);

protected:
	int heightmap_h_;//height,width
	int heightmap_w_;
	FString heightmap_name;//filenames of heightmap and trackpoints:)
	FString track_points_name;
	FString meta_file_name;
	FString pacenote_file_name;
	bool is_chunking;
	FVector2D start_pos;
	FVector2D end_pos;
};
