// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTree.h"

// Sets default values
ABasicTree::ABasicTree(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ScnComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene"));
	SetRootComponent(ScnComponent);
	instanced_basic_tree = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BasicInstancedTree"));
	SetRootComponent(instanced_basic_tree);
}
// Called when the game starts or when spawned
void ABasicTree::BeginPlay(){
	Super::BeginPlay();
	
}

void ABasicTree::NameChoiceTree(FString& mesh_name, int& tree_select)
{
	int rand_name = FMath::RandRange(0, 1);
	int range_start = 1;
	int range_end = 4;

	if (rand_name == 1) {
		mesh_name = "SM_Pine_Tree_";
		range_start = 1;
		range_end = 4;
	}
	else {
		mesh_name = "SM_Common_Tree_";
		range_start = 1;
		range_end = 11;
	}
	tree_select = FMath::RandRange(range_start, range_end);
}

void ABasicTree::NameChoicePlant(FString& mesh_name, float& z_alter)
{
	int ran_name = FMath::RandRange(0, 1);
	switch (ran_name)
	{
	case 0:
	{
		mesh_name = "SM_Bush";
		z_alter = 8.0f;
		break;
	}
	case 1:
	{
		mesh_name = "SM_Fern";
		z_alter = 2.0f;
		break;
	}
	}
}


//selects a tree type randomly, selects a position randomly, checks if in height limitations, spawns tree if in bounds, otherwise -1 on the index from the loop.
void ABasicTree::AddClusterTrees(const TArray<FVector>& m_verts, const int&max_, const int&min_, const TArray<FVector2D>& track_point, const bool& is_foilage){

	//default values, holder for name of the mesh, ranges for how many types of that mesh there are. min and max of the current terrain mesh, ttree sleect is for sleecting a tree. z_alter is for fixing the position of foilage.
	FString mesh_name;
	int loop_range = 85;
	int tree_select = 0;
	float max_m = max_;
	float min_m = min_;
	float z_alter = 0.0f;
	float yaw_rot = 0.0f;
	if (is_foilage){
		loop_range = 2000;
		NameChoicePlant(mesh_name,z_alter);
		yaw_rot = 270.0f;
	}
	else
	{
		NameChoiceTree(mesh_name, tree_select);
	}

	for (int i = 0; i < loop_range; i++) {
		int pos_y = FMath::RandRange(10, 380);
		int pos_x = FMath::RandRange(10, 380);
		float z_pos = m_verts[pos_y * 400 + pos_x].Z;
		bool is_found = false;
		while (!is_found)
		{
			if (z_pos<(max_m - (max_m * 0.30f)) && z_pos>(min_m + (max_m * 0.20f)))
			{
				if (CheckBounds(track_point, pos_x, pos_y)) 
				{
					is_found = true;
					FTransform A{
						FRotator{0,yaw_rot,0},
						FVector{pos_x * 20.0f, pos_y * 20.0f, (z_pos - z_alter) },
						FVector{0.250f, 0.250f, 0.250f} };	//Scale
					AddBasicTree(A, tree_select, mesh_name);
					instanced_basic_tree->SetMobility(EComponentMobility::Static);
					if (is_foilage) {
						instanced_basic_tree->bCastDynamicShadow = false;
						instanced_basic_tree->CastShadow = false;
						instanced_basic_tree->BodyInstance.bSimulatePhysics = false;
						instanced_basic_tree->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
					}
				}
				else{
					pos_x += FMath::RandRange(-5, 5);
					pos_y += FMath::RandRange(-5, 5);
					z_pos = m_verts[pos_y * 400 + pos_x].Z;
				}
			}
			else
			{
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

void ABasicTree::AddBasicTree(const FTransform& transform_, const int& tree_, const FString& name_attachment_){
	FString name_;
	name_.Append("StaticMesh'/Game/Stylized_PBR_Nature/Foliage/Assets/");
	AddName(name_attachment_, tree_, name_);
	name_.Append(".");
	AddName(name_attachment_, tree_, name_);
	name_.Append("'");
	UStaticMesh* meshToUse = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *name_));
	if (meshToUse && instanced_basic_tree){
		instanced_basic_tree->SetStaticMesh(meshToUse);
	}
	instanced_basic_tree->AddInstance(transform_);
}


bool ABasicTree::CheckBounds(const TArray<FVector2D>& track_point, int&point_x, int&point_y){
	for (int i = 0; i < track_point.Num(); i++){
		if ((int)point_x != (int)track_point[i].X && (int)point_y != (int)track_point[i].Y ) {
			//if not on track point, do nothing and continue to see if it is on a track point.
		}
		else {
			if ((int)point_x == (int)track_point[i].X&& (int)point_y == (int)track_point[i].Y){
				return false;
			}
			if ((int)point_x == (int)track_point[i].X+1 && (int)point_y == (int)track_point[i].Y) {
				return false;
			}
			if ((int)point_x == (int)track_point[i].X - 1 && (int)point_y == (int)track_point[i].Y) {
				return false;
			}
			if ((int)point_x == (int)track_point[i].X && (int)point_y == (int)track_point[i].Y+1) {
				return false;
			}
			if ((int)point_x == (int)track_point[i].X && (int)point_y == (int)track_point[i].Y-1) {
				return false;
			}
			if ((int)point_x == (int)track_point[i].X+1 && (int)point_y == (int)track_point[i].Y + 1) {
				return false;
			}
			if ((int)point_x == (int)track_point[i].X -1&& (int)point_y == (int)track_point[i].Y - 1) {
				return false;
			}
			//if equals point, or right, left, up, down 
		}
	}
	return true;
}

void ABasicTree::AddRockClusters(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts){

	auto d = FVector2D::Distance(track_point[0], track_point.Last());
	float rand_percent = FMath::RandRange(0.0f, 20.0f);
	float rocks_to_spawn_float = d * (rand_percent/100.0f);
	int rocks_to_spawn = round(rocks_to_spawn_float);
	for (int i = 0; i < rocks_to_spawn; i++){

		int rand_point = FMath::RandRange(0, track_point.Num());
		int pos_y = track_point[rand_point].Y;
		int pos_x = track_point[rand_point].X;
		
		bool is_found = false;
		while (!is_found)
		{
			if (CheckBounds(track_point, pos_x, pos_y)) {
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
				FVector{pos_x * 20.0f, pos_y * 20.0f, (z_pos) },
				FVector{rand_scale, rand_scale, rand_scale} };	//Scale
				instanced_basic_tree->AddInstance(A);
			}
			else {
				pos_x += FMath::RandRange(-4, 8);
				pos_y += FMath::RandRange(-8, 8);
			}
		}
	}
}


void ABasicTree::AddGrass(const TArray<FVector2D>& track_point, const TArray<FVector>& m_verts, const float&max, const float&min){

	float max_m = max;
	float min_m = min;


	for (int i = 0; i < 5000; i++) {
		int pos_y = FMath::RandRange(10, 380);
		int pos_x = FMath::RandRange(10, 380);
		float z_pos = m_verts[pos_y * 400 + pos_x].Z;

		if (CheckBounds(track_point, pos_x, pos_y)) {
			if (z_pos<(max_m - (max_m * 0.30f)) && z_pos>(min_m + (max_m * 0.20f))) {
				FTransform A{
					FRotator{0,270.0f,0},
					FVector{pos_x * 20.0f, pos_y * 20.0f, (z_pos) },
					FVector{0.250f, 0.250f, 0.250f} };	//Scale
				AddBasicTree(A, 0, "SM_Grass");
				instanced_basic_tree->SetMobility(EComponentMobility::Static);
				instanced_basic_tree->bCastDynamicShadow = false;
				instanced_basic_tree->CastShadow = false;
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