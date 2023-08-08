// Fill out your copyright notice in the Description page of Project Settings.
#include "BasicTree.h"
#include "PhysicsEngine/BodySetup.h"
// Sets default values
ABasicTree::ABasicTree(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ScnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene"));
	SetRootComponent(ScnComponent);
	instanced_basic_tree = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BasicInstancedTree"));
	SetRootComponent(instanced_basic_tree);
	spacing_=20;
	division_=5.0f;
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
	if (tree_select==7|| tree_select == 8){
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
			for (size_t switcher_ = 0; switcher_ < 2; switcher_++) {
				for (size_t x = 0; x < 5; x++) {
					for (size_t y = 0; y < 5; y++) {
						if ((int)point_x == (int)track_tree_points[i].X + x * pos_ && (int)point_y == (int)track_tree_points[i].Y + y * pos_) {
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

void ABasicTree::CheckDistance(const TArray<FVector2D>& track_point_arr, const int& x_pos, const int& y_pos, UStaticMesh& mesh_) {
	for (int i = 0; i < track_point_arr.Num(); i++){
		auto dist = FVector2D::Distance(track_point_arr[i], FVector2D(x_pos, y_pos));
		if (dist<10){
			mesh_.BodySetup->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseComplexAsSimple;
			i = track_point_arr.Num() - 1;
		}
		else {
			mesh_.BodySetup->CollisionTraceFlag = ECollisionTraceFlag::CTF_UseSimpleAsComplex;
		}
	}
}

void ABasicTree::AddTreeNearTrack(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts, const int& max_, const int& min_) {
	float yaw_rot = 0.0f;//gives random yaw
	FString mesh_name;
	int loop_range = FMath::RandRange(250, 750);
	int tree_select = 0;
	int pos_y = 0;
	int pos_x = 0;
	float z_pos = 0.f;
	float max_m = max_;
	float min_m = min_;//min and max terrain mesh points

	NameChoiceTree(mesh_name, tree_select);
	yaw_rot = FMath::RandRange(-360, 360);
	for (int i = 0; i < loop_range; i++) {
		int rand_point = FMath::RandRange(0, track_point.Num() - 1);
		pos_y = track_point[rand_point].Y;
		pos_x = track_point[rand_point].X;
		z_pos = m_verts[pos_y * 400 + pos_x].Z;
		bool is_found = false;
		while (!is_found) {
			if (CheckBounds(track_point, pos_x, pos_y)) {
				is_found = true;
				float rand_scale = FMath::RandRange(0.01f, 0.2f);
				float rand_yaw = FMath::RandRange(0.0f, 180.f);
				FTransform A{
				   FRotator{0,yaw_rot,0},
				   FVector{pos_x * spacing_, pos_y * spacing_, (z_pos) },
				   FVector{0.250f, 0.250f, 0.250f} };	//Scale		
				AddBasicTree(A, tree_select, mesh_name, track_point, pos_x, pos_y);
				instanced_basic_tree->SetMobility(EComponentMobility::Static);
				instanced_basic_tree->bCastDynamicShadow = true;
				instanced_basic_tree->CastShadow = true;
				track_tree_points.Add(FVector2D(pos_x, pos_y));
			}
			else {
				pos_x += FMath::RandRange(-4, 8);
				pos_y += FMath::RandRange(-8, 8);
				if (pos_x <= 0 || pos_x >= 400 || pos_y >= 400 || pos_y <= 0) {
					pos_y = track_point[rand_point].Y;
					pos_x = track_point[rand_point].X;
				}
				z_pos = m_verts[pos_y * 400 + pos_x].Z;
			}
		}
	}
}

//selects a tree type randomly, selects a position randomly, checks if in height limitations, spawns tree if in bounds, otherwise -1 on the index from the loop.
void ABasicTree::AddClusterTrees(const TArray<FVector>& m_verts, const int&max_, const int&min_, const TArray<FVector2D>& track_point, const bool& is_foilage){
	//default values, holder for name of the mesh, ranges for how many types of that mesh there are. min and max of the current terrain mesh, ttree sleect is for sleecting a tree. z_alter is for fixing the position of foilage.
	FString mesh_name;
	int loop_range = FMath::RandRange(250, 1000);
	int tree_select = 0;
	float max_m = max_;
	float min_m = min_;//min and max terrain mesh points
	float z_alter = 0.0f;//making sure placed on plane
	float yaw_rot = 0.0f;//gives random yaw
	float min_height_modi = 0.3f;//for the height check, different for trees and bushes
	float max_height_modi =0.15f; //same above
	max_height_modi = FMath::RandRange(0.1f,0.2f);
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
					instanced_basic_tree->bCastDynamicShadow = false;
					instanced_basic_tree->SetMobility(EComponentMobility::Static);
					if (is_foilage) {
						instanced_basic_tree->bCastDynamicShadow = false;
						instanced_basic_tree->CastShadow = false;
						instanced_basic_tree->BodyInstance.bSimulatePhysics = false;
						instanced_basic_tree->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
					}
				}
				else{
					bool is_found_pos = false;
					while (!is_found_pos){
						pos_x += FMath::RandRange(-5, 5);
						pos_y += FMath::RandRange(-5, 5);
						if (pos_y > 0 && pos_y < 400 && pos_x>0 && pos_x < 400) {
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
		CheckDistance(track_point_arr, x_pos, y_pos, *meshToUse);
	}
	if (meshToUse && instanced_basic_tree){
		instanced_basic_tree->SetStaticMesh(meshToUse);
	}
	instanced_basic_tree->AddInstance(transform_);
}

bool ABasicTree::CheckBounds(const TArray<FVector2D>& track_point, int&point_x, int&point_y){
	int pos_ = 1;
	for (int i = 0; i < track_point.Num(); i++){
		if ((int)point_x != (int)track_point[i].X && (int)point_y != (int)track_point[i].Y ) {
			//if not on track point, do nothing and continue to see if it is on a track point.
		}
		else {
			for (size_t switcher_ = 0; switcher_ < 2; switcher_++){
			for (size_t x = 0; x < 5; x++){
				for (size_t y = 0; y < 5; y++){
					if ((int)point_x == (int)track_point[i].X + x*pos_ && (int)point_y == (int)track_point[i].Y + y*pos_) {
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

void ABasicTree::AddRockClusters(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts){
	auto d = FVector2D::Distance(track_point[0], track_point.Last());
	float rand_percent = FMath::RandRange(0.0f, spacing_);
	float rocks_to_spawn_float = d * (rand_percent/100.0f);
	int rocks_to_spawn = round(rocks_to_spawn_float);
	rocks_to_spawn *= 8;
	//above calculates the number of rocks to spawn. by finding distance of track, geting a number between 0 and 20, turning that to a percentage and rounding to int.
	for (int i = 0; i < rocks_to_spawn; i++){
		int rand_point = FMath::RandRange(0, track_point.Num()-1);
		int pos_y = track_point[rand_point].Y;
		int pos_x = track_point[rand_point].X;
		bool is_found = false;
		while (!is_found){
			if (CheckBounds(track_point, pos_x, pos_y)&& CheckTrackTree(pos_x,pos_y)) {
				is_found = true;
				float z_pos = m_verts[pos_y * 400 + pos_x].Z;
				float rand_scale = FMath::RandRange(0.01f, 0.2f);
				float rand_yaw = FMath::RandRange(0.0f, 180.f);
				UStaticMesh* meshToUse = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Game/Stylized_PBR_Nature/Rocks/Assets/SM_R_Rock_02.SM_R_Rock_02'")));
				if (meshToUse && instanced_basic_tree) {
					instanced_basic_tree->SetStaticMesh(meshToUse);
				}
				FTransform A{
				FRotator{0,rand_yaw,0},
				FVector{pos_x * spacing_, pos_y * spacing_, (z_pos) },
				FVector{rand_scale, rand_scale, rand_scale} };	//Scale
				instanced_basic_tree->AddInstance(A);
				instanced_basic_tree->SetMobility(EComponentMobility::Static);
				
			}
			else {
				pos_x += FMath::RandRange(-4, 8);
				pos_y += FMath::RandRange(-8, 8);
				if (pos_x<=0 ||pos_x>=400 || pos_y>=400 || pos_y<=0){
					pos_y = track_point[rand_point].Y;
					pos_x = track_point[rand_point].X;
				}
			}
		}
	}
}
void ABasicTree::AddGrass(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts, const float&max, const float&min){
	float max_m = max;
	float min_m = min;
	for (int i = 0; i < 1000; i++) {
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
				instanced_basic_tree->SetMobility(EComponentMobility::Static);
				instanced_basic_tree->bCastDynamicShadow = true;
				instanced_basic_tree->CastShadow = true;
				instanced_basic_tree->BodyInstance.bSimulatePhysics = false;
				instanced_basic_tree->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
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