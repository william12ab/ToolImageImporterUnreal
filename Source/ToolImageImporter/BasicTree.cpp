// Fill out your copyright notice in the Description page of Project Settings.
#include "BasicTree.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "PhysicsEngine/BodySetup.h"
// Sets default values
ABasicTree::ABasicTree(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ScnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene"));
	SetRootComponent(ScnComponent);
	h_instanced = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("BasicInstancedTree"));
	SetRootComponent(h_instanced);
	spacing_=20;
	division_=5.0f;
	is_track_tree = false;
}
// Called when the game starts or when spawned
void ABasicTree::BeginPlay(){
	Super::BeginPlay();
}

void ABasicTree::NameChoiceTree(FString& mesh_name, int& tree_select){
	int rand_name = FMath::RandRange(0, 1);
	int range_start = 1;
	int range_end = 4;

	if (rand_name == 1) {
		mesh_name = "SM_Pine_Tree_";
		range_start = 1;
		range_end = 2;
	}
	else {
		mesh_name = "SM_Common_Tree_";
		range_start = 1;
		range_end = 11;
	}
	tree_select = FMath::RandRange(range_start, range_end);
	if (tree_select==7|| tree_select == 8|| tree_select==4){
		tree_select = 10;
	}
}

void ABasicTree::NameChoicePlant(FString& mesh_name, float& z_alter){
	int ran_name = FMath::RandRange(0, 1);
	switch (ran_name){
	case 0:{
		mesh_name = "SM_Bush";
		z_alter = 8.0f;
		break;
	}
	case 1:{
		mesh_name = "SM_Fern";
		z_alter = 2.0f;
		break;
	}
	}
}

bool ABasicTree::CheckTrackTree(int& point_x, int& point_y){
	int pos_ = 1;
	for (int i = 0; i < track_tree_points.Num(); i++) {
		if ((int)point_x != (int)track_tree_points[i].X && (int)point_y != (int)track_tree_points[i].Y) {
			//if not on track point, do nothing and continue to see if it is on a track point.
		}
		else {
			for (int switcher_ = 0; switcher_ < 2; switcher_++) {
				for (int x = 0; x < 5; x++) {
					for (int y = 0; y < 5; y++) {
						if ((int)point_x == (int)track_tree_points[i].X + x * pos_ && (int)point_y == (int)track_tree_points[i].Y + y * pos_) {
							index_tracker = i;
							return false;
						}
					}
				}
				pos_ *= -1;//if equals point, or right, left, up, down 
			}
		}
	}
	return true;
}

void ABasicTree::CheckDistance(UStaticMesh& mesh_) {
	if (is_track_tree){
		mesh_.BodySetup->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseComplexAsSimple;
	}
}


void ABasicTree::AddTreeNearTrack(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts, const int& max_, const int& min_) {
	float yaw_rot = 0.0f;//gives random yaw
	FString mesh_name;
	int loop_range = 16;
	int tree_select = 0;
	float max_m = max_;
	float min_m = min_;//min and max terrain mesh points
	auto temp_vector = track_point;
	NameChoiceTree(mesh_name, tree_select);
	is_track_tree = true;
	yaw_rot = FMath::RandRange(-360, 360);
	for (int i = 0; i < loop_range; i++) {
		int rand_point = FMath::RandRange(5, temp_vector.Num() - 1);
		int pos_y = temp_vector[rand_point].Y;
		int pos_x = temp_vector[rand_point].X;
		float z_pos = m_verts[pos_y * 400 + pos_x].Z;
		pos_x += FMath::RandRange(-4, 8);
		pos_y += FMath::RandRange(-8, 8);
		if (pos_x <= 0 || pos_x >= 400 || pos_y >= 400 || pos_y <= 0) {
			pos_y = temp_vector[rand_point].Y;
			pos_x = temp_vector[rand_point].X;
		}
		z_pos = m_verts[pos_y * 400 + pos_x].Z;
		FTransform A{
			   FRotator{0,yaw_rot,0},
			   FVector{pos_x * spacing_, pos_y * spacing_, (z_pos) },
			   FVector{0.250f, 0.250f, 0.250f} };	//Scale		
		AddBasicTree(A, tree_select, mesh_name, temp_vector, pos_x, pos_y);
		h_instanced->SetMobility(EComponentMobility::Movable);
		h_instanced->bCastDynamicShadow = true;
		h_instanced->CastShadow = true;
		h_instanced->SetMassOverrideInKg(NAME_None, 10000.f);
		h_instanced->SetCullDistances(750, 3000);
		h_instanced->SetMobility(EComponentMobility::Static);
		track_tree_points.Add(FVector2D(pos_x, pos_y));
		temp_vector.RemoveAt(rand_point);
		}
	index_tracker = 0;

	for (int j = 0; j < 2; j++) {
		for (int i = h_instanced->GetInstanceCount() - 1; i > 0; i--) {
			int x = track_tree_points[i].X;
			int y = track_tree_points[i].Y;
			if (!CheckBounds(track_point, x, y)) {
				h_instanced->RemoveInstance(i);
				track_tree_points.RemoveAt(i);
			}
		}
	}
	if (!CheckBounds(track_point, track_tree_points[0].X, track_tree_points[0].Y)) {
		h_instanced->RemoveInstance(0);
		track_tree_points.RemoveAt(0);
	}
	h_instanced->UpdateBounds();
}

//selects a tree type randomly, selects a position randomly, checks if in height limitations, spawns tree if in bounds, otherwise -1 on the index from the loop.
void ABasicTree::AddClusterTrees(const TArray<FVector>& m_verts, const int&max_, const int&min_, const TArray<FVector2D>& track_point, const bool& is_foilage){
	//default values, holder for name of the mesh, ranges for how many types of that mesh there are. min and max of the current terrain mesh, ttree sleect is for sleecting a tree. z_alter is for fixing the position of foilage.
	FString mesh_name;
	int loop_range = 250;
	int tree_select = 0;
	float max_m = max_;
	float min_m = min_;//min and max terrain mesh points
	float z_alter = 0.0f;//making sure placed on plane
	float yaw_rot = 0.0f;//gives random yaw
	float min_height_modi = 0.3f;//for the height check, different for trees and bushes
	float max_height_modi =0.15f; //same above
	max_height_modi = FMath::RandRange(0.1f,0.2f);
	is_track_tree = false;
	if (is_foilage){
		loop_range =  500;
		NameChoicePlant(mesh_name,z_alter);
		//yaw_rot = 270.0f;
		yaw_rot = FMath::RandRange(-360, 360);
		max_height_modi = 0.05f;
	}
	else{
		NameChoiceTree(mesh_name, tree_select);
		yaw_rot = FMath::RandRange(-360, 360);
	}
	for (int i = 0; i < loop_range; i++) {
		int pos_y = FMath::RandRange(10, 380);
		int pos_x = FMath::RandRange(10, 380);
		float z_pos = m_verts[pos_y * 400 + pos_x].Z;
		bool is_found = false;
		while (!is_found){
			if (z_pos<(max_m - (max_m * min_height_modi)) && z_pos>(min_m + (max_m * max_height_modi))){
				if (CheckBounds(track_point, pos_x, pos_y)) {
					is_found = true;
					if (is_foilage){
						FTransform A{
						FRotator{0,yaw_rot,0},
						FVector{pos_x * spacing_, pos_y * spacing_, (z_pos - z_alter) },
						FVector{0.150f, 0.150f, 0.150f} };	//Scale
						AddBasicTree(A, tree_select, mesh_name, track_point, pos_x, pos_y);
					}
					else{
						FTransform A{
					   FRotator{0,yaw_rot,0},
					   FVector{pos_x * spacing_, pos_y * spacing_, (z_pos - z_alter) },
					   FVector{0.250f, 0.250f, 0.250f} };	//Scale		
						AddBasicTree(A, tree_select, mesh_name,track_point,pos_x,pos_y);
					}
					h_instanced->bCastDynamicShadow = false;
					h_instanced->SetCullDistances(750, 3000);
					h_instanced->SetMobility(EComponentMobility::Static);
					if (is_foilage) {
						h_instanced->bCastDynamicShadow = false;
						h_instanced->CastShadow = false;
						h_instanced->BodyInstance.bSimulatePhysics = false;
						h_instanced->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
					}
				}
				else{
					bool is_found_pos = false;
					while (!is_found_pos){
						pos_x += FMath::RandRange(-5, 5);
						pos_y += FMath::RandRange(-5, 5);
						if (pos_y >= 0 && pos_y < 400 && pos_x>=0 && pos_x < 400) {
							z_pos = m_verts[pos_y * 400 + pos_x].Z;
							is_found_pos = true;
						}
					}
				}
			}
			else{
				pos_y = FMath::RandRange(10, 380);
				pos_x = FMath::RandRange(10, 380);
				z_pos = m_verts[pos_y * 400 + pos_x].Z;
			}
		}
	}
}

void ABasicTree::AddName(const FString& name_attachment_,const int&tree_, FString& name_){
	if (name_attachment_ == "SM_Common_Tree_") {
		name_.Append(name_attachment_);
		if (tree_ < 10) {
			name_.AppendInt(0);
			name_.AppendInt(tree_);
		}
		else if (tree_==0){
			
		}
		else {
			name_.AppendInt(tree_);
		}
	}
	else {
		name_.Append(name_attachment_);
		if (tree_!=0){
			name_.AppendInt(tree_);
		}
	}
}

void ABasicTree::AddBasicTree(const FTransform& transform_, const int& tree_, const FString& name_attachment_,const TArray<FVector2D>& track_point_arr, const int& x_pos, const int& y_pos){
	FString name_;
	name_.Append("StaticMesh'/Game/Stylized_PBR_Nature/Foliage/Assets/");
	AddName(name_attachment_, tree_, name_);
	name_.Append(".");
	AddName(name_attachment_, tree_, name_);
	name_.Append("'");
	UStaticMesh* meshToUse = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *name_));
	if (name_attachment_=="SM_Pine_Tree_"|| name_attachment_ == "SM_Common_Tree_"){
		CheckDistance(*meshToUse);
	}
	if (meshToUse && h_instanced){
		h_instanced->SetStaticMesh(meshToUse);
	}
	h_instanced->AddInstance(transform_);
}

bool ABasicTree::CheckBounds(const TArray<FVector2D>& track_point, const int&point_x, const int&point_y){
	for (int i = 0; i < track_point.Num(); i++){
		int pos_ = 1;
		if ((int)point_x != (int)track_point[i].X && (int)point_y != (int)track_point[i].Y ) {
			//if not on track point, do nothing and continue to see if it is on a track point.
		}
		else {
			FVector2D loc_t_point = track_point[i];
			for (int x = -5; x < 5; x++){
				for (int y = -5; y < 5; y++){
					if ((int)point_x == loc_t_point.X + x*pos_ && (int)point_y == loc_t_point.Y + y*pos_) {
						return false;
					}
				}
			}
			
		}
	}
	return true;
}

void ABasicTree::AddRockClusters(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts){
	int rocks_to_spawn = 100;
	//above calculates the number of rocks to spawn. by finding distance of track, geting a number between 0 and 20, turning that to a percentage and rounding to int.
	for (int i = 0; i < rocks_to_spawn; i++) {
		int rand_point = FMath::RandRange(0, track_point.Num() - 1);
		int pos_y = track_point[rand_point].Y;
		int pos_x = track_point[rand_point].X;
		pos_x += FMath::RandRange(-4, 8);
		pos_y += FMath::RandRange(-8, 8);
		if (pos_x <= 0 || pos_x >= 400 || pos_y >= 400 || pos_y <= 0) {
			pos_y = track_point[rand_point].Y;
			pos_x = track_point[rand_point].X;
		}
		float z_pos = m_verts[pos_y * 400 + pos_x].Z;
		float rand_scale = FMath::RandRange(0.01f, 0.2f);
		float rand_yaw = FMath::RandRange(0.0f, 180.f);
		UStaticMesh* meshToUse = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/Stylized_PBR_Nature/Rocks/Assets/SM_R_Rock_02.SM_R_Rock_02'")));
		if (meshToUse && h_instanced) {
			h_instanced->SetStaticMesh(meshToUse);
		}
		FTransform A{
		FRotator{0,rand_yaw,0},
		FVector{pos_x * spacing_, pos_y * spacing_, (z_pos) },
		FVector{rand_scale, rand_scale, rand_scale} };	//Scale
		h_instanced->AddInstance(A);
		h_instanced->SetMobility(EComponentMobility::Movable);
		h_instanced->SetCullDistances(750, 3000);
		h_instanced->SetMassOverrideInKg(NAME_None, 10000.f);
		h_instanced->SetMobility(EComponentMobility::Static);
		h_instanced->bReceivesDecals = false;
		track_rock_points.Add(FVector2D(pos_x, pos_y));
	}

	for (int i = h_instanced->GetInstanceCount() - 1; i > 0; i--) {
		FTransform f;
		int x = track_rock_points[i].X;
		int y = track_rock_points[i].Y;
		if (!CheckBounds(track_point, x, y)&& CheckTrackTree(x,y)) {
			h_instanced->RemoveInstance(i);
			h_instanced->GetInstanceTransform(i, f);
		}
	}
	h_instanced->UpdateBounds();
}
void ABasicTree::AddGrassInstance(const int&x, const int&y, const float&z_pos, const int& item) {
	float rand_rot_yaw = FMath::RandRange(-360, 360);
	float rand_scale = FMath::RandRange(0.1f, 0.251f);
	float min_range = FMath::RandRange(0.250f, 0.350f);
	float max_range = FMath::RandRange(0.050f, 0.10f);
	float x_add = FMath::RandRange(-0.52f, 0.52f);
	float y_add = FMath::RandRange(-.52f, 0.52f);
	float total_x = x + x_add;
	float total_y = y + y_add;
	FTransform A{
	FRotator{0,rand_rot_yaw,0},
	FVector{(total_x) * spacing_, (total_y) * spacing_, (z_pos) },
	FVector{rand_scale, rand_scale, rand_scale} };	//Scale
	TArray<FVector2D>a;
	if (item==0){
		AddBasicTree(A, 0, "SM_Grass", a, 0, 0);
	}
	else {
		AddBasicTree(A, 0, "SM_Grass", a, 0, 0);
	}
	h_instanced->SetCullDistances(750, 3000);
	h_instanced->SetMobility(EComponentMobility::Static);
	h_instanced->bCastDynamicShadow = true;
	h_instanced->CastShadow = true;
	h_instanced->BodyInstance.bSimulatePhysics = false;
	h_instanced->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
	h_instanced->bReceivesDecals = false;
}
void ABasicTree::AddGrassAtEdge(const TArray<FVector>& m_verts, const TArray<FLinearColor>& m_colors, const int& height_) {
	for (int y = 0; y < height_; y++){
		for (int x = 0; x < height_; x++){
			if (x+1<height_){
				if (m_colors[y * height_ + x] != m_colors[y * height_ + (x + 1)]) {
					float z_pos = m_verts[y* height_+ x].Z;
					AddGrassInstance(x, y, z_pos,item_);
					z_pos = m_verts[y * height_ + (x + 1)].Z;
					AddGrassInstance((x+1), y, z_pos,item_);
				}
			}
			if (y + 1 < height_) {
				if (m_colors[y * height_ + x] != m_colors[(y + 1) * height_ + x]) {
					float z_pos = m_verts[(y+1) * height_ + x].Z;
					AddGrassInstance(x, (y+1), z_pos,item_);
					z_pos = m_verts[y * height_ + x].Z;
					AddGrassInstance(x, y, z_pos, item_);
				}
			}
		}
	}
}

void ABasicTree::AddGrass(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts, const float&max, const float&min){
	float max_m = max;
	float min_m = min;
	for (int i = 0; i < 250; i++) {
		int pos_y = FMath::RandRange(10, 380);
		int pos_x = FMath::RandRange(10, 380);
		float z_pos = m_verts[pos_y * 400 + pos_x].Z;
		float rand_rot_yaw = FMath::RandRange(-360, 360);
		float rand_scale = FMath::RandRange(0.125f, 0.2f);
		float min_range = FMath::RandRange(0.250f, 0.350f);
		float max_range = FMath::RandRange(0.050f, 0.10f);
		if (CheckBounds(track_point, pos_x, pos_y)) {
			if (z_pos<(max_m - (max_m * min_range)) && z_pos>(min_m + (max_m * max_range))) {
				FTransform A{
					FRotator{0,rand_rot_yaw,0},
					FVector{pos_x * spacing_, pos_y * spacing_, (z_pos) },
					FVector{rand_scale, rand_scale, rand_scale} };	//Scale
				TArray<FVector2D>a;
				AddBasicTree(A, 0, "SM_Grass",a,0,0);
				h_instanced->SetCullDistances(750, 3000);
				h_instanced->SetMobility(EComponentMobility::Static);
				h_instanced->bCastDynamicShadow = true;
				h_instanced->CastShadow = true;
				h_instanced->BodyInstance.bSimulatePhysics = false;
				h_instanced->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
				h_instanced->bReceivesDecals = false;
			}
			else {
				i--;
			}
		}
		else {
			i--;
		}
	}
}