// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackMesh.h"



float ATrackMesh::Lerp(const float& p1, const float& p2, const float& t)
{
	//auto p3 = p1 + ((p2 - p1) * t);
	//return (1 - t) * v0 + t * v1;
	auto c = (1.0 - t) * p1 + t * p2;
	return c;
}
float ATrackMesh::FindHeight(float x, float y, int og_x, int og_y, const TArray<int32>& c_)
{
	float height;
	if (x == (float)og_x) {				//VERTICAL LINE on left side
		//p1=og_x,og_y. p2=og_x,og_y+1
		float t = y - (float)(og_y);
		height = Lerp(c_[og_y * 400 + og_x], c_[(og_y + 1) * 400 + og_x], t);
	}
	else if (x == (float)(og_x + 1)) {			//VERTICAL LINE on right side
		//p1=og_x+1,og_y. p2=og_x+1,og_y+1
		float t = y - (float)(og_y);
		height = Lerp(c_[og_y * 400 + (og_x + 1)], c_[(og_y + 1) * 400 + (og_x + 1)], t);
	}
	else if (y == (float)og_y) {				//HORIZONTAL LINE - bottom side
		//p1=og_x,og_y. p2=og_x+1,og_y
		float t = x - (float)(og_x);
		height = Lerp(c_[og_y * 400 + og_x], c_[(og_y) * 400 + (og_x + 1)], t);
	}
	else if (y == (float)(og_y + 1)) {		//HORIZONTAL LINE - top side
		//p1=og_x,og_y+1. p2=og_x+1,og_y+1
		float t = x - (float)(og_x);
		height = Lerp(c_[(og_y + 1) * 400 + (og_x)], c_[(og_y + 1) * 400 + (og_x + 1)], t);
	}
	else {
		float p1_t = y - (float)(og_y);//t on the coord p1. see diagram
		float p1_height = Lerp(c_[og_y * 400 + (og_x)], c_[(og_y + 1) * 400 + (og_x)], p1_t);
		float p2_t = y - (float)(og_y);//t on the coord p2.
		float p2_height = Lerp(c_[og_y * 400 + (og_x + 1)], c_[(og_y + 1) * 400 + (og_x + 1)], p2_t);
		float t = x - (float)(og_x);
		height = Lerp(p1_height, p2_height, t);

		float xp1_t = x - (float)(og_x);//t on the coord p1. see diagram
		float xp1_height = Lerp(c_[og_y * 400 + (og_x)], c_[(og_y) * 400 + (og_x + 1)], xp1_t);
		float xp2_t = x - (float)(og_x);//t on the coord p2.
		float xp2_height = Lerp(c_[(og_y + 1) * 400 + (og_x)], c_[(og_y + 1) * 400 + (og_x + 1)], xp2_t);
		float xt = y - (float)(og_y);
		float xheight = Lerp(xp1_height, xp2_height, xt);

		if (height != xheight) {
			height = ((height + xheight) / 2);
		}
	}
	return height;
}

void ATrackMesh::CoordAdjuster(float& x, float& y, const int& index, float part_added)
{
	if (index == 0) {
		x += part_added;	//moving to br
		uv_spacing_track.X += part_added;
	}
	if (index == 1) {			//moving to tl
		x -= part_added;
		y += part_added;
		uv_spacing_track.Y += part_added;
		uv_spacing_track.X -= part_added;
	}
	if (index == 2) {			//to tr
		x += part_added;
		uv_spacing_track.Y += part_added;
		uv_spacing_track.X += part_added;
	}
	if (index == 3) {
		x -= part_added;
		y -= part_added;
		uv_spacing_track.Y -= part_added;
		uv_spacing_track.X -= part_added;
	}
}

void ATrackMesh::AddMultiVerts(float x, float y, const TArray<int32>& c_, const int& m_, int og_x, int og_y, const TArray<FVector>& m_norms_terrain)
{
	float part_added = 0.0625;
	for (int i = 0; i < 4; i++) {
		AddVert(x, y, c_, m_, og_x, og_y, m_norms_terrain);			//bottom left
		uv_spacing_track.X += part_added;
		AddVert(x + part_added, y, c_, m_, og_x, og_y, m_norms_terrain);			//bottom right
		uv_spacing_track.Y += part_added;
		uv_spacing_track.X -= part_added;
		AddVert(x, y + part_added, c_, m_, og_x, og_y, m_norms_terrain);			//top left
		uv_spacing_track.X += part_added;
		AddVert(x + part_added, y + part_added, c_, m_, og_x, og_y, m_norms_terrain);			//top right
		uv_spacing_track.Y -= part_added;
		uv_spacing_track.X -= part_added;
		GenerateTrackTris();
		CoordAdjuster(x, y, i, part_added);
	}
}

void ATrackMesh::AddVert(float x, float y, const TArray<int32>& c_, const int& m_, int og_x, int og_y, const TArray<FVector>&m_norms_terrain)
{
	float cal_height = FindHeight(x, y, og_x, og_y, c_);
	m_verts.Add(FVector(x * spacing_, y * spacing_, (cal_height * spacing_) / m_));

	m_norms.Add(m_norms_terrain[og_y * 400 + (og_x)]);

	m_u_vs.Add(FVector2D(uv_spacing_track.X, uv_spacing_track.Y));
	m_vert_colors.Add(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	m_tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
}

void ATrackMesh::GenerateTrackVerts(const TArray<FVector2D>& track_points, const TArray<int32>& c_, const int& m_, const TArray<FVector>& m_norms_terrain)
{
	float outmost_part = 0.5;
	float outer_part_added = 0.25;
	float inner_part_added = 0.125;
	for (int i = 0; i < track_points.Num(); i++) {
		float x = track_points[i].X;
		float y = track_points[i].Y;
		int og_x = track_points[i].X;
		int og_y = track_points[i].Y;
		uv_spacing_track = FVector2D(0.0f, 0.0f);
		for (int f = 0; f < 4; f++) {
			for (int j = 0; j < 4; j++) {
				for (int index_ = 0; index_ < 4; index_++) {
					AddMultiVerts(x, y, c_, m_, og_x, og_y, m_norms_terrain);
					CoordAdjuster(x, y, index_, inner_part_added);
				}
				CoordAdjuster(x, y, j, outer_part_added);
			}
			CoordAdjuster(x, y, f, outmost_part);
		}
	}
}
void ATrackMesh::GenerateTrackTris()
{
	m_tris.Add(count);
	m_tris.Add(count + 2);
	m_tris.Add(count + 1);
	m_tris.Add(count + 1);
	m_tris.Add(count + 2);
	m_tris.Add(count + 3);
	count += 4;
}

void ATrackMesh::CreateTrack(const TArray<FVector2D>& track_points, const TArray<int32>& c_, const int& m_, const TArray<FVector>& m_norms_terrain)
{
	ClearMeshData();
	GenerateTrackVerts(track_points, c_, m_, m_norms_terrain);
	//Function that creates mesh section
	procedural_mesh_comp->CreateMeshSection_LinearColor(0, m_verts, m_tris, m_norms, m_u_vs, m_vert_colors, m_tangents, false);
	procedural_mesh_comp->bCastDynamicShadow = false;
	material_interface = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/Materials/TrackMaterial.TrackMaterial'"));
	material_instance = UMaterialInstanceDynamic::Create(material_interface, this);
	procedural_mesh_comp->SetMaterial(0, material_instance);
}