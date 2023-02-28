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
	int ran_name = FMath::RandRange(0, 2);
	switch (ran_name)
	{
	case 0:
	{
		mesh_name = "SM_Grass";
		z_alter = 0;
		break;
	}
	case 1:
	{
		mesh_name = "SM_Bush";
		z_alter = 8.0f;
		break;
	}
	case 2:
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
	if (is_foilage){
		loop_range = 1000;
		NameChoicePlant(mesh_name,z_alter);
	}
	else
	{
		NameChoiceTree(mesh_name, tree_select);
	}

	for (int i = 0; i < loop_range; i++) {
		int pos_y = FMath::RandRange(10, 380);
		int pos_x = FMath::RandRange(10, 380);
		float z_pos = m_verts[pos_y * 400 + pos_x].Z;
		
		if (CheckBounds(track_point,pos_x,pos_y)) {
			if (m_verts[pos_y * 400 + pos_x].Z<(max_m - (max_m * 0.30f)) && m_verts[pos_y * 400 + pos_x].Z>(min_m + (max_m * 0.20f))) {
				FTransform A{
					FRotator{0,0,0},
					FVector{(float)pos_x * 20.0f, (float)pos_y * 20.0f, (z_pos- z_alter) },
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
			else {
				i--;
			}
		}
		else{
			i--;
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
		if ((float)point_x != track_point[i].X && (float)point_y != track_point[i].Y) {
			//not allowed
		}
		else {
			if ((float)point_x == track_point[i].X&& (float)point_y == track_point[i].Y){
				return false;
			}
		}
	}
	return true;
}

void ABasicTree::AddRockClusters(){
	/*get the total length of the track.
	produce change to spawn rock out of the length: 
	so say at anywhere between 0-20% of the length, can spawn new rock

	1.find length
	2. find amount of rocks to spawn
	2.1 int rand_percent = rand_range(0,20)
	2.2 rand_percent % of length = amount of rocks to spawwn - so this gives how many.
	3. for amount of rocks
	3.1 spawn at random edge of track.
	*/

	//length
	//could take distance between start and end, or actual length...
}