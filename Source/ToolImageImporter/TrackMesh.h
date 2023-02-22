// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyProceduralMesh.h"
#include "TrackMesh.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API ATrackMesh : public AMyProceduralMesh
{
	GENERATED_BODY()

public:
	
	void GenerateTrackVerts(const TArray<FVector2D>& track_points, const TArray<int32>& c_, const int& m_, const TArray<FVector>& m_norms_terrain);
	void AddVert(float x, float y, const TArray<int32>& c_, const int& m_, int og_x, int og_y, const TArray<FVector>& m_norms_terrain);
	void AddMultiVerts(float x, float y, const TArray<int32>& c_, const int& m_, int og_x, int og_y, const TArray<FVector>& m_norms_terrain);
	void GenerateTrackTris();
	void CreateTrack(const TArray<FVector2D>& track_points, const TArray<int32>& c_, const int& m_, const TArray<FVector>& m_norms_terrain);
	void CoordAdjuster(float& x, float& y, const int& index, float part_added);
	float FindHeight(float x, float y, int og_x, int og_y, const TArray<int32>& c_);
	float Lerp(const float& p1, const float& p2, const float& t);

private:
	FVector2D uv_spacing_track;

};
