// Fill out your copyright notice in the Description page of Project Settings.
#include "MyProceduralMesh.h"
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include <Runtime/Engine/Public/ImageUtils.h>
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using the_clock = std::chrono::steady_clock;
// Sets default values
AMyProceduralMesh::AMyProceduralMesh() {
	PrimaryActorTick.bCanEverTick = false;
	ScnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Main"));
	SetRootComponent(ScnComponent);
	procedural_mesh_comp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	procedural_mesh_comp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	width_ = 4;
	height_ = 4;
	spacing_ = 20.0f;
	count = 0;
	is_temp = false;
	auto PhysicalMaterialAsset = ConstructorHelpers::FObjectFinder<UObject>(TEXT("PhysicalMaterial'/Game/GroundPhysMat.GroundPhysMat'"));
	if (PhysicalMaterialAsset.Object) {
		procedural_mesh_comp->BodyInstance.SetPhysMaterialOverride((UPhysicalMaterial*)PhysicalMaterialAsset.Object);
	}
	vec_m_verts.Add(m_verts);
	vec_m_verts.Add(m_verts1);
	vec_m_verts.Add(m_verts2);
	vec_m_verts.Add(m_verts3);

	vec_m_norms.Add(m_norms);
	vec_m_norms.Add(m_norms1);
	vec_m_norms.Add(m_norms2);
	vec_m_norms.Add(m_norms3);

	vec_m_vert_colors.Add(m_vert_colors);
	vec_m_vert_colors.Add(m_vert_colors1);
	vec_m_vert_colors.Add(m_vert_colors2);
	vec_m_vert_colors.Add(m_vert_colors3);

	vec_m_tris.Add(m_tris);
	vec_m_tris.Add(m_tris1);
	vec_m_tris.Add(m_tris2);
	vec_m_tris.Add(m_tris3);
	is_created = false;
}

void AMyProceduralMesh::FullSize() {
	procedural_mesh_comp->ClearAllMeshSections();
	height_ = 800;
	width_ = 800;
	ClearMeshData();
	vec_m_verts[0].SetNum(height_ * height_);
	vec_m_norms[0].SetNum(height_ * height_);
	vec_m_vert_colors[0].SetNum(height_ * height_);
	GenerateVerts(0);
	GenerateTris(0);
	ParallelFor(width_, [&](int32 i) {
		for (int32 j = 0; j < height_; j++) {
			vec_m_verts[0][(i * (height_)) + (j)].Z = z_axis[(i * (height_)) + (j)];
		}
		});
	CalculateNormals(0);
	procedural_mesh_comp->CreateMeshSection_LinearColor(0, vec_m_verts[0], vec_m_tris[0], vec_m_norms[0], m_u_vs, temp_colors, m_tangents, false);
	z_axis.Empty();
}

// Called when the game starts or when spawned
void AMyProceduralMesh::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AMyProceduralMesh::PostInitializeComponents() {
	Super::PostInitializeComponents();
}


void AMyProceduralMesh::SmoothTerrain(TArray<float>& c_) {
	int local_height;
	local_height = height_;
	if (is_chunking && !is_temp) {
		local_height *= 2;
	}
	ParallelFor(local_height, [&](int32 j) {
		//for (int32 j = 0; j < (local_height); j++){
		for (int32 i = 0; i < (local_height); i++) {
			int count_loc = 0;
			float tHeight = 0.0f;
			if (i - 1 >= 0) {
				count_loc++;
				tHeight += c_[(j * (local_height)) + (i - 1)];
			}
			if (i + 1 < (local_height)) {
				count_loc++;
				tHeight += c_[(j * (local_height)) + (i + 1)];
			}
			if (j - 1 >= 0) {
				count_loc++;
				tHeight += c_[((j - 1) * (local_height)) + i];
			}
			if (j + 1 < (local_height)) {
				count_loc++;
				tHeight += c_[((j + 1) * (local_height)) + i];
			}
			if ((i - 1 >= 0) && (j - 1 >= 0)) {
				count_loc++;
				tHeight += c_[((j - 1) * (local_height)) + (i - 1)];
			}
			if ((i + 1 < (local_height)) && (j - 1 >= 0)) {
				count_loc++;
				tHeight += c_[((j - 1) * (local_height)) + (i + 1)];
			}
			if ((i - 1 >= 0) && (j + 1 < (local_height))) {
				count_loc++;
				tHeight += c_[((j + 1) * (local_height)) + (i - 1)];
			}
			if ((i + 1 < (local_height)) && (j + 1 < (local_height))) {
				count_loc++;
				tHeight += c_[((j + 1) * (local_height)) + (i + 1)];
			}
			tHeight /= (float)count_loc;

			c_[(j * (local_height)) + i] = tHeight;
		}
		});
}


float AMyProceduralMesh::FindT(const FVector2D& p1, const FVector2D& p2, const FVector2D& p3) {
	auto a = p1 - p3;
	auto b = p1 - p2;

	float t = 0.0f;
	if (a.X == 0 || b.X == 0) {
		float c2 = ((float)a.Y / (float)b.Y);
		t = c2;
		if (t < 0.0f) {
			t *= -1;
		}
		if (a.Y == 0 && b.Y == 0) {
			t = 0;
		}
	}
	else if (a.Y == 0 || b.Y == 0) {
		float c1 = ((float)a.X / (float)b.X);
		t = c1;
		if (a.X == 0 && b.X == 0) {
			t = 0;
		}
	}
	else {
		float c1 = ((float)a.X / (float)b.X);
		float c2 = ((float)a.Y / (float)b.Y);
		t = (c1 + c2) / 2.0f;
	}
	return t;
}

void AMyProceduralMesh::SetMaterial(UTexture* t_) {
	material_interface = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/Materials/TerrainMaterial.TerrainMaterial'"));
	material_instance = UMaterialInstanceDynamic::Create(material_interface, this);
	material_instance->SetTextureParameterValue(FName("terr_text"), t_);		//this is reference also to the material obj in scene, have a look if you dont get it.
	procedural_mesh_comp->SetMaterial(0, material_instance);
}
void AMyProceduralMesh::ClearMeshData() {
	vec_m_verts[0].Empty();
	vec_m_verts[1].Empty();
	vec_m_verts[2].Empty();
	vec_m_verts[3].Empty();
//	m_verts_total.Empty();
	vec_m_tris[0].Empty();
	vec_m_tris[1].Empty();
	vec_m_tris[2].Empty();
	vec_m_tris[3].Empty();
	m_tris_total.Empty();
	m_u_vs.Empty();
	vec_m_norms[0].Empty();
	vec_m_norms[1].Empty();
	vec_m_norms[2].Empty();
	vec_m_norms[3].Empty();
	vec_m_vert_colors[0].Empty();
	vec_m_vert_colors[1].Empty();
	vec_m_vert_colors[2].Empty();
	vec_m_vert_colors[3].Empty();
	m_tangents.Empty();

}

void AMyProceduralMesh::AddVert(const int& index_, const int& x, const int& y) {
	switch (index_) {
	case 0: {
		vec_m_verts[0][y * height_ + x]=(FVector(x * spacing_, y * spacing_, 0.0f));
		//m_verts_total.Add(FVector(x * spacing_, y * spacing_, 0.0f));
		vec_m_norms[0][y * height_ + x] = (FVector(0.0f, 0.0f, .0f));
		vec_m_vert_colors[0][y * height_ + x]=(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	}
	case 1: {
		vec_m_verts[1][y * height_ + x]=(FVector((x + (400 - 1)) * spacing_, y * spacing_, 0.0f));
		//m_verts_total.Add(FVector((x + (400 - 1)) * spacing_, y * spacing_, 0.0f));
		vec_m_norms[1][y * height_ + x] = (FVector(0.0f, 0.0f, .0f));
		vec_m_vert_colors[1][y * height_ + x] = (FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	}
	case 2: {
		vec_m_verts[2][y * height_ + x]=(FVector(x * spacing_, (y + (400 - 1)) * spacing_, 0.0f));
	//	m_verts_total.Add(FVector(x * spacing_, (y + (400 - 1)) * spacing_, 0.0f));
		vec_m_norms[2][y * height_ + x] = (FVector(0.0f, 0.0f, .0f));
		vec_m_vert_colors[2][y * height_ + x] = (FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	}
	case 3: {
		vec_m_verts[3][y * height_ + x]=(FVector((x + (400 - 1)) * spacing_, (y + (400 - 1)) * spacing_, 0.0f));
	//	m_verts_total.Add(FVector((x + (400 - 1)) * spacing_, (y + (400 - 1)) * spacing_, 0.0f));
		vec_m_norms[3][y * height_ + x] = (FVector(0.0f, 0.0f, .0f));
		vec_m_vert_colors[3][y * height_ + x] = (FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	}
	}
}

void AMyProceduralMesh::GenerateVerts(const int& index_) {
	float uv_spacing = 1.0f / FMath::Max(height_, width_);
	m_u_vs.SetNum(height_* height_);
	m_tangents.SetNum(height_ * height_);
	ParallelFor(height_, [&](int32 y) {
		//for (int32 y = 0; y < height_; y++) {
		for (int32 x = 0; x < width_; x++) {
			AddVert(index_, x, y);
			m_u_vs[y * height_ + x] = FVector2D(x * uv_spacing, y * uv_spacing);
			m_tangents[y * height_ + x] = FProcMeshTangent(0.0f, 1.0f, 0.0f);
		}
		});
}

void AMyProceduralMesh::CalculateNormals(const int& index_) {
	ParallelFor((height_ - 1), [&](int32 j) {
		for (int32 i = 0; i < (width_ - 1); i++) {
			//Calculate the plane normals
			FVector a, b, c;	//Three corner vertices
			a = vec_m_verts[index_][j * height_ + i];
			b = vec_m_verts[index_][j * height_ + i + 1];
			c = vec_m_verts[index_][(j + 1) * height_ + i];

			//Two edges
			FVector ab(c.X - a.X, c.Y - a.Y, c.Z - a.Z);
			FVector ac(b.X - a.X, b.Y - a.Y, b.Z - a.Z);

			//Calculate the cross product
			FVector cross;
			cross.X = ab.Y * ac.Z - ab.Z * ac.Y;
			cross.Y = ab.Z * ac.X - ab.X * ac.Z;
			cross.Z = ab.X * ac.Y - ab.Y * ac.X;
			float mag = (cross.X * cross.X) + (cross.Y * cross.Y) + (cross.Z * cross.Z);
			mag = sqrtf(mag);
			cross.X /= mag;
			cross.Y /= mag;
			cross.Z /= mag;
			vec_m_norms[index_][j * height_ + i] = -cross;
		}
		});

	FVector smoothedNormal(0, 0, 0);
	for (int32 j = 0; j < width_; j++) {
		for (int32 i = 0; i < width_; i++) {

			smoothedNormal.X = 0;
			smoothedNormal.Y = 0;
			smoothedNormal.Z = 0;
			float count_n = 0;
			//Left planes
			if ((i - 1) >= 0) {
				//Top planes
				if ((j) < (width_ - 1)) {
					smoothedNormal.X += vec_m_norms[index_][j * width_ + (i - 1)].X;
					smoothedNormal.Y += vec_m_norms[index_][j * width_ + (i - 1)].Y;
					smoothedNormal.Z += vec_m_norms[index_][j * width_ + (i - 1)].Z;
					count_n++;
				}
				//Bottom planes
				if ((j - 1) >= 0) {
					smoothedNormal.X += vec_m_norms[index_][(j - 1) * width_ + (i - 1)].X;
					smoothedNormal.Y += vec_m_norms[index_][(j - 1) * width_ + (i - 1)].Y;
					smoothedNormal.Z += vec_m_norms[index_][(j - 1) * width_ + (i - 1)].Z;
					count_n++;
				}
			}
			//right planes
			if ((i) < (width_ - 1)) {

				//Top planes
				if ((j) < (width_ - 1)) {
					smoothedNormal.X += vec_m_norms[index_][j * width_ + i].X;
					smoothedNormal.Y += vec_m_norms[index_][j * width_ + i].Y;
					smoothedNormal.Z += vec_m_norms[index_][j * width_ + i].Z;
					count_n++;
				}
				//Bottom planes
				if ((j - 1) >= 0) {
					smoothedNormal.X += vec_m_norms[index_][(j - 1) * width_ + i].X;
					smoothedNormal.Y += vec_m_norms[index_][(j - 1) * width_ + i].Y;
					smoothedNormal.Z += vec_m_norms[index_][(j - 1) * width_ + i].Z;
					count_n++;
				}
			}
			smoothedNormal.X /= count_n;
			smoothedNormal.Y /= count_n;
			smoothedNormal.Z /= count_n;

			float mag = sqrt((smoothedNormal.X * smoothedNormal.X) + (smoothedNormal.Y * smoothedNormal.Y) + (smoothedNormal.Z * smoothedNormal.Z));
			smoothedNormal.X /= mag;
			smoothedNormal.Y /= mag;
			smoothedNormal.Z /= mag;

			vec_m_norms[index_][j * width_ + i] = smoothedNormal;
		}
	}
}

void AMyProceduralMesh::GenerateTris(const int& index_) {
	for (int32 y = 0; y < (height_ - 1); y++) {
		for (int32 x = 0; x < (width_ - 1); x++) {
			m_tris_total.Add(x + (y * width_));					//current vertex
			m_tris_total.Add(x + (y * width_) + width_);			//current vertex + row
			m_tris_total.Add(x + (y * width_) + width_ + 1);		//current vertex + row + one right

			m_tris_total.Add(x + (y * width_));					//current vertex
			m_tris_total.Add(x + (y * width_) + width_ + 1);		//current vertex + row + one right
			m_tris_total.Add(x + (y * width_) + 1);				//current vertex + one right

			vec_m_tris[index_].Add(x + (y * width_));					//current vertex
			vec_m_tris[index_].Add(x + (y * width_) + width_);			//current vertex + row
			vec_m_tris[index_].Add(x + (y * width_) + width_ + 1);		//current vertex + row + one right

			vec_m_tris[index_].Add(x + (y * width_));					//current vertex
			vec_m_tris[index_].Add(x + (y * width_) + width_ + 1);		//current vertex + row + one right
			vec_m_tris[index_].Add(x + (y * width_) + 1);				//current vertex + one right
		}
	}
}

void AMyProceduralMesh::TestFinal() {
	//m_verts_total.Empty();
	z_axis.SetNum(640000);
//	m_verts_total.SetNum(640000);
	temp_colors.SetNum(640000);
	//adds verts etc into total verts and also z_axis;
	int local_grid_size = height_ * 2;
	ParallelFor(local_grid_size, [&](int32 j) {
		//for (int j = 0; j < (local_grid_size); j++) {
		for (int32 i = 0; i < (local_grid_size); i++) {
			if (i < height_ && j < height_) {
			//	m_verts_total[(j * (local_grid_size)) + (i)] = vec_m_verts[0][(j * (height_)) + (i)];
				z_axis[(j * (local_grid_size)) + (i)] = vec_m_verts[0][(j * (height_)) + (i)].Z;
				temp_colors[(j * (local_grid_size)) + (i)] = vec_m_vert_colors[0][(j * (height_)) + (i)];
			}
			else if (i >= height_ && j < height_) {
				//m_verts_total[(j * (local_grid_size)) + (i)] = vec_m_verts[1][(j * height_) + (i - height_)];
				z_axis[(j * (local_grid_size)) + (i)] = vec_m_verts[1][(j * height_) + (i - height_)].Z;
				temp_colors[(j * (local_grid_size)) + (i)] = vec_m_vert_colors[1][(j * height_) + (i - height_)];
			}
			else if (i < height_ && j >= height_) {
			//	m_verts_total[(j * (local_grid_size)) + (i)] = vec_m_verts[2][((j - height_) * height_) + i];
				z_axis[(j * (local_grid_size)) + (i)] = vec_m_verts[2][((j - height_) * height_) + i].Z;
				temp_colors[(j * (local_grid_size)) + (i)] = vec_m_vert_colors[2][((j - height_) * height_) + i];
			}
			else if (i >= height_ && j >= height_) {
			//	m_verts_total[(j * (local_grid_size)) + (i)] = vec_m_verts[3][((j - height_) * height_) + (i - height_)];
				z_axis[(j * (local_grid_size)) + (i)] = vec_m_verts[3][((j - height_) * height_) + (i - height_)].Z;
				temp_colors[(j * (local_grid_size)) + (i)] = vec_m_vert_colors[3][((j - height_) * height_) + (i - height_)];
			}
		}
		});
	//smoths
	SmoothTerrain(z_axis);
	SmoothTerrain(z_axis);
	//m_verts_total.Empty();
	//m_verts_total.SetNum(640000);
	int ind = 0;
	//adds verts back into total
	//ParallelFor(local_grid_size, [&](int32 j) {
	//	//for (int j = 0; j < (local_grid_size); j++) {
	//	for (int32 i = 0; i < (local_grid_size); i++) {
	//		if (i < height_ && j < height_) {
	//			m_verts_total[(j * (local_grid_size)) + (i)] = FVector(vec_m_verts[0][(j * (height_)) + (i)].X, vec_m_verts[0][(j * (height_)) + (i)].Y, z_axis[(j * (local_grid_size)) + (i)]);
	//		}
	//		else if (i >= height_ && j < height_) {
	//			m_verts_total[(j * (local_grid_size)) + (i)] = FVector(vec_m_verts[1][(j * height_) + (i - height_)].X, vec_m_verts[1][(j * height_) + (i - height_)].Y, z_axis[(j * (local_grid_size)) + (i)]);
	//		}
	//		else if (i < height_ && j >= height_) {
	//			m_verts_total[(j * (local_grid_size)) + (i)] = FVector(vec_m_verts[2][((j - height_) * height_) + i].X, vec_m_verts[2][((j - height_) * height_) + i].Y, z_axis[(j * (local_grid_size)) + (i)]);
	//		}
	//		else if (i >= height_ && j >= height_) {
	//			m_verts_total[(j * (local_grid_size)) + (i)] = FVector(vec_m_verts[3][((j - height_) * height_) + (i - height_)].X, vec_m_verts[3][((j - height_) * height_) + (i - height_)].Y, z_axis[(j * (local_grid_size)) + (i)]);
	//		}
	//	}
	//	});

	//into correct places
	ParallelFor(local_grid_size, [&](int32 j) {
		//for (int32 j = 0; j < local_grid_size; j++) {
		for (int32 i = 0; i < local_grid_size; i++) {
			if (i < height_ && j < height_) {
				vec_m_verts[0][(j * height_) + i].Z = z_axis[(j * local_grid_size) + i];
			}
			else if (i >= height_ && j < height_) {
				vec_m_verts[1][(j * height_) + (i - height_)].Z = z_axis[(j * local_grid_size) + i];
			}
			else if (i < height_ && j >= height_) {
				vec_m_verts[2][((j - height_) * height_) + i].Z = z_axis[(j * local_grid_size) + i];
			}
			else if (i >= height_ && j >= height_) {
				vec_m_verts[3][((j - height_) * height_) + (i - height_)].Z = z_axis[(j * local_grid_size) + i];
			}
		}
		});

	ParallelFor(4, [&](int32 i) {
		procedural_mesh_comp->UpdateMeshSection_LinearColor(i, vec_m_verts[i], vec_m_norms[i], m_u_vs, vec_m_vert_colors[i], m_tangents);
		});
}


void AMyProceduralMesh::CreateMesh(int& d_height_, int& d_width_, float& d_spacing_, const int& index_) {
	height_ = d_height_;
	width_ = d_width_;
	spacing_ = 20.0f;
	if (!is_created) {
		ClearMeshData();
		for (int i = 0; i < vec_m_verts.Num(); i++) {
			vec_m_verts[i].SetNum(height_ * height_);
			vec_m_norms[i].SetNum(height_ * height_);
			vec_m_vert_colors[i].SetNum(height_ * height_);
			if (is_temp){
				i = 3;
			}
		}
		is_created = true;
	}
	GenerateVerts(index_);
	GenerateTris(index_);
	//Function that creates mesh section

	procedural_mesh_comp->CreateMeshSection_LinearColor(index_, vec_m_verts[index_], vec_m_tris[index_], vec_m_norms[index_], m_u_vs, vec_m_vert_colors[index_], m_tangents, is_temp);
}


void AMyProceduralMesh::ModiVerts(TArray<float>& c_, const int& m_, const int& index_) {
	if (m_ == 0) {
		SmoothTerrain(c_);
		SmoothTerrain(c_);
		SmoothTerrain(c_);
		SmoothTerrain(c_);
	}
	//FCriticalSection Mutex;
	ParallelFor(width_, [&](int32 y) {

		//for (int32 y = 0; y < height_; y++) {
		for (int32 x = 0; x < width_; x++) {
			if (height_ == 400) {
				vec_m_verts[index_][y * height_ + x].Z = (c_[y * height_ + x] * spacing_) / m_;
			}
			else {
				vec_m_verts[index_][y * height_ + x].Z = (c_[y * height_ + x]);
			}
		}
		});

	CalculateNormals(index_);

	procedural_mesh_comp->bCastDynamicShadow = false;
	procedural_mesh_comp->UpdateMeshSection_LinearColor(index_, vec_m_verts[index_], vec_m_norms[index_], m_u_vs, vec_m_vert_colors[index_], m_tangents);

	material_interface = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/Materials/TerrainMaterial.TerrainMaterial'"));
	material_instance = UMaterialInstanceDynamic::Create(material_interface, this);
	procedural_mesh_comp->SetMaterial(index_, material_instance);
}


float Lerp(const float& p1, const float& p2, const float& t)
{
	auto c = (1.0f - t) * p1 + t * p2;
	return c;
}

FVector LerpV(const FVector& p1, const FVector& p2, const float& t)
{
	auto c = (1.0f - t) * p1 + t * p2;
	return c;
}


float Distance(const FVector& p1, const FVector& p2) {
	float xd = p2.X - p1.X;
	float yd = p2.Y - p1.Y;
	auto c = sqrt((xd * xd) + (yd * yd));
	return c;
}

void AMyProceduralMesh::ChangePos(int& pos, const int& max, bool& local_is_changed) {
	if (pos < 0) {
		pos = 0;
		local_is_changed = true;
	}
	if (pos > 399) {
		pos = 399;
		local_is_changed = true;
	}
}

void AMyProceduralMesh::ChangeVert(const float& x_pos, const float& y_pos, const float& z_pos, const int& index_) {
	bool local_is_changed = false;//bool to catch if point changed
	int y_po = ((static_cast<int>(y_pos) / spacing_));		//getting point in local coord system
	ChangePos(y_po, vec_m_verts[index_].Num(), local_is_changed);		//if point is less than or greater than bounds change point.

	int x_po = ((static_cast<int>(x_pos) / spacing_));		//samas as above
	ChangePos(x_po, vec_m_verts[index_].Num(), local_is_changed);

	vec_m_verts[index_][(y_po)*height_ + (x_po)].Z = z_pos;			//set the vert to the height
	if (local_is_changed) {									//if changed used different mechanic
		vec_m_vert_colors[index_][(static_cast<int>(y_po)) * height_ + (static_cast<int>(x_po))] = FLinearColor::Black;
	}
	else {
		vec_m_vert_colors[index_][(static_cast<int>(y_pos / spacing_)) * height_ + (static_cast<int>(x_pos / spacing_))] = FLinearColor::Black;
	}
}

void AMyProceduralMesh::ReplaceC(TArray<float>& c_, const int& index_) {
	TArray<float> temp;
	for (int i = 0; i < vec_m_verts[index_].Num(); i++) {
		temp.Add(vec_m_verts[index_][i].Z);
	}
	if (!is_chunking) {
		SmoothTerrain(temp);
		SmoothTerrain(temp);
	}
	for (int32 y = 0; y < height_; y++) {
		for (int32 x = 0; x < width_; x++) {
			vec_m_verts[index_][y * height_ + x].Z = temp[y * height_ + x];
		}
	}
	CalculateNormals(index_);

	procedural_mesh_comp->UpdateMeshSection_LinearColor(index_, vec_m_verts[index_], vec_m_norms[index_], m_u_vs, vec_m_vert_colors[index_], m_tangents);
}


void AMyProceduralMesh::SetHeightProper(const TArray<FVector>& points_, const TArray<FVector>& verts_, const int& index_) {
	int index_tracker_verts = 0;
	int right, left;
	left = 3;
	right = 2;
	float inner_count_size = 100.0f;
	for (int i = 0; i < points_.Num(); i += 2) {
		float dist = Distance(points_[i], points_[i + 1]);
		int int_dist = round(dist);
		int_dist *= 20;
		for (int j = 0; j < (int)int_dist; j++) {
			float t = (float)(j / (float)int_dist);
			auto left_pos = LerpV(verts_[index_tracker_verts + left], verts_[index_tracker_verts + (left - 2)], t);	//gives pos on left
			auto right_pos = LerpV(verts_[index_tracker_verts + right], verts_[index_tracker_verts + (right - 2)], t);	//gives pos on right
			auto centre_pos = LerpV(points_[i], points_[i + 1], t);
			ChangeVert(centre_pos.X, centre_pos.Y, left_pos.Z, index_);
			ChangeVert(left_pos.X, left_pos.Y, left_pos.Z, index_);
			ChangeVert(right_pos.X, right_pos.Y, left_pos.Z, index_);
			for (int k = 0; k < (int)inner_count_size; k++) {
				float t_inner = (float)(k / inner_count_size);
				auto a = LerpV(left_pos, right_pos, t_inner);
				ChangeVert(a.X, a.Y, left_pos.Z, index_);
			}
		}
		index_tracker_verts += 4;
	}
}
void AMyProceduralMesh::NearestNeighbourSample(const int& grid_size, const int& new_size, const TArray<FVector>& m_verts_, TArray<FVector>& temp_vec, const int& scale, const TArray<FLinearColor>& temp_colour, TArray<FLinearColor>& new_c) {
	for (int i = 0; i < (grid_size); i++) {
		for (int j = 0; j < (grid_size); j++) {
			int x_dash = j * new_size / grid_size;
			int y_dash = i * new_size / grid_size;
			temp_vec[i * new_size + j].Z = m_verts_[i * grid_size + j].Z;
			new_c[y_dash * new_size + x_dash] = temp_colour[i * grid_size + j];
		}
	}
	//coloums
	for (int i = 0; i < (new_size); i++) {
		for (int j = 0; j < (new_size - 1); j += scale) {
			auto c = m_verts_[(i / scale) * grid_size + (j / scale)].Z;
			auto c_ = temp_colour[(i / scale) * grid_size + (j / scale)];
			for (int g = 0; g < scale; g++) {
				temp_vec[(i * new_size) + (j + g)].Z = (c);
				new_c[(i * new_size) + (j + g)] = c_;
			}
		}
	}
	//////rows - same for rows
	for (int i = 0; i < (new_size - 1); i += scale) {
		for (int j = 0; j < (new_size); j++) {
			auto c = m_verts_[(i / scale) * grid_size + (j / scale)].Z;
			auto c_ = temp_colour[(i / scale) * grid_size + (j / scale)];
			for (int g = 0; g < scale; g++) {
				temp_vec[(i + g) * new_size + j].Z = c;
				new_c[(i + g) * new_size + j] = c_;
			}
		}
	}
}

void AMyProceduralMesh::Resize(const TArray<FVector>& m_verts_, const int& scale_, const TArray<FLinearColor>& temp_colour, const int& index_) {
	int grid_size;
	if (is_chunking) {
		grid_size = 800;
	}
	else {
		grid_size = 400;
	}

	float spacing = 0.f;//just for passing in
	int new_size = grid_size * scale_;
	TArray<FVector> new_z;
	TArray<FLinearColor> new_c;
	new_z.SetNum(new_size * new_size);
	new_c.SetNum(new_size * new_size);
	height_ = new_size;
	width_ = new_size;
	//inital vals

	NearestNeighbourSample(grid_size, new_size, m_verts_, new_z, scale_, temp_colour, new_c);
	CreateMesh(height_, width_, spacing, index_);

	//gaining temp verts
	TArray<float> temp_c;
	temp_c.SetNum(new_size * new_size);
	for (int32 y = 0; y < height_; y++) {
		for (int32 x = 0; x < width_; x++) {
			vec_m_verts[0][y * height_ + x].Z = new_z[y * height_ + x].Z;
			vec_m_vert_colors[0][y * height_ + x] = new_c[y * height_ + x];
			temp_c[y * height_ + x] = new_z[y * height_ + x].Z;
		}
	}
	ModiVerts(temp_c, 0, index_);//smoothing and setting verts plus regen.
}

void AMyProceduralMesh::Save(TArray<FVector>& temp_, TArray<FLinearColor>& temp_colours) {
	for (int i = 0; i < vec_m_verts[0].Num(); i++) {
		temp_.Add(vec_m_verts[0][i]);
		temp_colours.Add(vec_m_vert_colors[0][i]);
	}
}
