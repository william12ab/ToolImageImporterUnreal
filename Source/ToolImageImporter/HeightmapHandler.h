// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
class TOOLIMAGEIMPORTER_API HeightmapHandler
{
public:
	HeightmapHandler();
	~HeightmapHandler();

	TArray<float> ReadFileInfo(const FString& name_, int &height_, int &width_);		//reads file for height map also calls generate plane and modi verts.

	bool ReadTrackPoints(const FString& name_, TArray<FVector2D>&track_points,TArray<FVector2D> &control_points);

protected:
	int h_;
	int w_;
};
