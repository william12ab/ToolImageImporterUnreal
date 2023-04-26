 // Fill out your copyright notice in the Description page of Project Settings.


#include "UIWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include <Runtime/Engine/Public/ImageUtils.h>

void UUIWidget::NativeConstruct()
{
	Super::NativeConstruct();
	w_ = 4;
	h_ = 4;
	s_ = 20.0f;		//distance between verts in proc mesh
	m_ = 7;			//division of height
	scaling_down_ = 10.0f;		//scale factor of everything, so 8 times what it is now
	Label->SetText(FText::FromString("Plane Generator"));

	delete_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickDelete);
	file_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickLoadNewTrack);
	test_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnTest);
	vehicle_pawn = Cast<AVehicleController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	counter_ = 0.0f;
	point_type = false;
	is_level_spawnned = false;

	images_.Add(image_slot_1);
	images_.Add(image_slot_2);
	images_.Add(image_slot_3);
	images_.Add(image_slot_4);
	images_.Add(image_slot_5);
}

bool ReadFileInfoA(const FString& dialog_name_, FString &file_name)
{
	FString default_path = "";
	FString dialog_name = dialog_name_;
	FString default_file = "";
	FString file_types = "";
	TArray<FString> outfile_names;			//stores the file
	uint32 flags_ = 1;
	IDesktopPlatform* fpl = FDesktopPlatformModule::Get();
	if (fpl->OpenFileDialog(0, dialog_name, default_path, default_file, file_types, flags_, outfile_names)) {
		file_name = outfile_names[0];
		return true;
	}
	else {
		return false;
	}
}

void UUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime){
	Super::NativeTick(MyGeometry, InDeltaTime);
	

	//UI FUNCS
	StartTextFunction();
	HandBreakTextFunction();
	CountdownImageFunction(InDeltaTime);

	//collisons for restarting position
	auto l = vehicle_pawn->GetActorLocation();
	l /= s_;
	l /= scaling_down_;
	FVector2D current_point = FVector2D(l.X, l.Y);
	for (int i = 0; i < track_points.Num(); i++) {
		if (static_cast<int>(current_point.X) == static_cast<int>(track_points[i].X) && static_cast<int>(current_point.Y) == static_cast<int>(track_points[i].Y)) {
			last_point = FVector(track_points[i].X, track_points[i].Y, l.Z);
			index_recorder = i;
		}
	}
	
	//if pressed, start timer, if held for 1.5sec, and v <5, put back to correct place.
	if (vehicle_pawn->GetPressed()) {
		counter_ += InDeltaTime;
		vehicle_pawn->SetCounter(counter_);
		if (vehicle_pawn->GetCounter()>= 1.5f) {
			if (vehicle_pawn->GetVelocityFromComp() < 5.f) {
				vehicle_pawn->SetPressed(false);
				float angle = atan2(track_points[index_recorder + 1].Y - last_point.Y, track_points[index_recorder + 1].X - last_point.X) * 180.0f / PI;
				last_point *= scaling_down_;
				last_point *= s_;
				last_point.Z += 5;
				while (!vehicle_pawn->TeleportTo(last_point, FRotator(0.0f, angle, 0.0f), false, false)){
					last_point.Z += 1.f;
				}
				counter_ = 0.0f;
			}
		}
	}
}

void UUIWidget::OnClickLoadNewTrack(){
	FString dialog_name = "Open Heightmap";
	FString return_name;
	if (ReadFileInfoA(dialog_name, return_name)){
		name_ = return_name;
		OnClickHeightmapButton();
			LoadTrackPointsIn();
	}
}

void UUIWidget::GeneratePlane(){
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = FVector(0,0,0);
	p_mesh = GetWorld()->SpawnActor<AMyProceduralMesh>(myLoc, myRot, SpawnInfo);
	p_mesh->CreateMesh(h_,w_,s_);
}

void UUIWidget::OnClickDelete(){
	DeletePlane();
}

void UUIWidget::DeletePlane(){
	if (p_mesh){
		p_mesh->Destroy();
	}
	name_.Empty();
	m_colors.Empty();
	track_points.Empty();
}


void UUIWidget::OnClickHeightmapButton(){
	ReadFileInfo(name_);
}

void UUIWidget::LoadTrackPointsIn()
{
	FString n = "Open New Track.";
	FString outfile_names;
	auto b= ReadFileInfoA(n, outfile_names);
	IPlatformFile& file_manager= FPlatformFileManager::Get().GetPlatformFile();
	TArray<FString> array_;
	if (file_manager.FileExists(*outfile_names)){
		if (FFileHelper::LoadFileToStringArray(array_, *outfile_names)){
		}
		else{
		}
	}
	bool found_ = false;
	for (int i = 0; i < array_.Num(); i++){
		if (i > 0) {
			if (array_[i].Contains(FString("end"))) {
				found_ = true;
			}
			else {
				if (!found_) {
					auto index_ = array_[1].Find(" ");
					control_points.Add(FVector2D(FCString::Atoi(*array_[i]), FCString::Atoi(*array_[i].Right(index_ + 1))));
				}
				else {
					auto index_ = array_[1].Find(" ");
					track_points.Add(FVector2D(FCString::Atoi(*array_[i]), FCString::Atoi(*array_[i].Right(index_ + 1))));
				}
			}
		}
		else{
			if (array_[0] == "t.p") {
				point_type = true;
			}
			else {
				point_type = false;
			}
		}
	}
	CreateTrack();
}

void UUIWidget::CreateTrack(){
	p_mesh->ModiVerts(m_colors, m_);
	CreateFoilage();
}

void UUIWidget::ReadFileInfo(const FString& name__)
{
	UTexture2D* texture_ = FImageUtils::ImportFileAsTexture2D(name__);
	texture_->AddToRoot();
	
	//since grey scale already. each rgb component should be greyscale value. therefore no need to add up and divide by 3.
	//rgb comp is uint8 value. so, use this as the height and modifiy the height of terrain.
	const FColor* formated_image_data = static_cast<const FColor*>(texture_->PlatformData->Mips[0].BulkData.LockReadOnly());
	h_ = texture_->PlatformData->Mips[0].SizeY;
	w_ = texture_->PlatformData->Mips[0].SizeX;
	for (int32 y_ = 0; y_ < h_; y_++) {
		for (int32 x_ = 0; x_ < w_; x_++) {
			FColor pixel_color = formated_image_data[y_ * texture_->GetSizeX() + x_]; // Do the job with the pixel
			float pixel_colour_float = pixel_color.R;
			m_colors.Add(pixel_colour_float);
		}
	}
	texture_->PlatformData->Mips[0].BulkData.Unlock();
	texture_->UpdateResource();
	GeneratePlane();
}

void UUIWidget::LerpCalculation(TArray<FVector2D> &temp_arr, const int& index_saftey_p, const int& index_t_p){
	auto safet_p = track_spline->GetSafetyPoints();
	for (int t = 0; t < 100; t++) {
		float t_val = t / 100.f;
		if (index_saftey_p==1){
			temp_arr.Add(track_spline->LerpV2D(track_points[index_t_p], (safet_p[index_saftey_p]/ s_), t_val));
		}
		else{
			temp_arr.Add(track_spline->LerpV2D((safet_p[index_saftey_p]/ s_), track_points[index_t_p], t_val));
		}	
	}
	track_points.Insert(temp_arr, index_t_p);
}

void UUIWidget::FillInGaps(){
	TArray<FVector2D> temp_vec_first_pos;
	LerpCalculation(temp_vec_first_pos, 0, 0);//between start and start respectively
	temp_vec_first_pos.Empty();
	LerpCalculation(temp_vec_first_pos, 1, track_points.Num()-1);//end and end respectively
}

void UUIWidget::CreateFoilage(){
	int max = 0;
	int min = 100000000;
	int index = 0;
	for (int i = 0; i < p_mesh->m_verts.Num(); i++) {
		if (p_mesh->m_verts[i].Z > max) {
			max = p_mesh->m_verts[i].Z;
		}
		if (p_mesh->m_verts[i].Z < min) {
			min = p_mesh->m_verts[i].Z;
			index = i;
		}
	}
	CreateSpline();
	FillInGaps();

	FActorSpawnParameters SpawnInfoTree;
	FRotator myRotTree(0, 0, 0);
	FVector myLocTree = FVector(0, 0, 0);
	for (int i = 0; i < 4; i++) {//tree
		ABasicTree* tree_instancea;
		tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
		tree_instancea->AddClusterTrees(p_mesh->m_verts, max, min, track_points, false);
		tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	}
	for (int i = 0; i < 2; i++) {//ferns bushes
		ABasicTree* tree_instancea;
		tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
		tree_instancea->AddClusterTrees(p_mesh->m_verts, max, min, track_points, true);
		tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	}
	for (int i = 0; i < 1; i++) {//rocks
		ABasicTree* tree_instancea;
		tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
		tree_instancea->AddRockClusters(track_points, p_mesh->m_verts);
		tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	}
	for (int i = 0; i < 1; i++) {//grass
		ABasicTree* tree_instancea;
		tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
		tree_instancea->AddGrass(track_points, p_mesh->m_verts, max, min);
		tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	}
	//water
	float water_height = (min + (max * 0.05f));
	if (water_height>=track_spline->GetMinHeight()){
		water_height = track_spline->GetMinHeight() ;
	}
	myLocTree = FVector(p_mesh->m_verts[index].X, p_mesh->m_verts[index].Y, water_height);
	w_mesh = GetWorld()->SpawnActor<AWaterMesh>(myLocTree, myRotTree, SpawnInfoTree);
	w_mesh->SetActorScale3D(FVector(30, 30, 30));
	FixScales();
}
void UUIWidget::CreateSpline(){
	TArray<FVector2D> temp_arr;
	if (point_type){
		temp_arr = track_points;
	}
	else {
		temp_arr = control_points;
	}

	for (size_t i = 0; i < temp_arr.Num(); i++){
		(temp_arr[i].X) *= s_;
		(temp_arr[i].Y) *= s_;
	}
	FActorSpawnParameters SpawnInfoTree;
	FRotator myRotTree(0, 0, 0);
	FVector myLocTree = FVector(0, 0, 0);
	FTransform t_transform_{
					FRotator{0,0,0},
					FVector{0, 0, 0},
					FVector{1, 1, 1} };
	track_spline = GetWorld()->SpawnActor<ATrackSpline>(myLocTree, myRotTree, SpawnInfoTree);//above to here just spawning actor
	track_spline->SetSpacing(s_);
	track_spline->SetDivision(m_);//setters
	track_spline->SetControlPoints(temp_arr);//setting array in class to the points
	track_spline->SetHeightArray(m_colors);//setting array as well
	track_spline->OnConstruction(t_transform_);//consttruction
	p_mesh->SetHeightProper(track_spline->GetSEPoints(), track_spline->GetVerts());//changing height of mesh
	p_mesh->ReplaceC(m_colors);//replacing heightmap to match new mesh, also normals and smoothing
	track_spline->SetActorLocation(FVector(track_spline->GetActorLocation().X, track_spline->GetActorLocation().Y, track_spline->GetActorLocation().Z + 27.f));
	track_spline->SetActorEnableCollision(false);
	if (point_type) {
		for (size_t i = 0; i < temp_arr.Num(); i++) {
			temp_arr[i].X /= s_;
			temp_arr[i].Y /= s_;
		}
		track_points = temp_arr;
	}
	else {
		control_points = temp_arr;
	}
}

void UUIWidget::FixScales(){
	p_mesh->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	track_spline->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	w_mesh->SetActorScale3D(FVector(30* scaling_down_, 30* scaling_down_, 30* scaling_down_));
	auto t = w_mesh->GetActorLocation();
	w_mesh->SetActorLocation(t* scaling_down_);

	auto middle_point = FMath::Lerp(track_spline->GetSEPoints()[0], track_spline->GetSEPoints()[1], 0.5f);
	middle_point *= scaling_down_;
	float angle = atan2(track_spline->GetSEPoints()[1].Y - track_spline->GetSEPoints()[0].Y, track_spline->GetSEPoints()[1].X - track_spline->GetSEPoints()[0].X) * 180.0f / PI;
	while (!vehicle_pawn->TeleportTo(middle_point, FRotator(0.0f, angle, 0.0f), false, false)) {
		middle_point.Z += 1.f;
	}
	auto ss = track_spline->GetSEPoints().Num();


	FActorSpawnParameters SpawnInfoDecal;
	FActorSpawnParameters SpawnInfoBox = FActorSpawnParameters();

	FRotator myRotD(0, 0, 0);
	FVector myLocD = FVector(track_spline->GetSEPoints()[1]);
	myLocD *= scaling_down_;
	myLocD.Z += 85.f;
	FName RightName = FName(TEXT("boxendtriggername"));
	SpawnInfoBox.Name = RightName;
	box_start = GetWorld()->SpawnActor<ATriggerBoxDecal>(myLocD, myRotD, SpawnInfoDecal);
	myLocD.Z -= 85.f;
	myLocD.Z -= 85.f;
	start_decal = GetWorld()->SpawnActor<AStartDecalActor>(myLocD, myRotD, SpawnInfoDecal);
	myLocD = track_spline->GetSEPoints()[ss- 10];
	myLocD *= scaling_down_;
	box_end = GetWorld()->SpawnActor<ATriggerBoxDecal>(myLocD, myRotD, SpawnInfoBox);
	myLocD.Z -= 85.f;
	end_decal= GetWorld()->SpawnActor<AStartDecalActor>(myLocD, myRotD, SpawnInfoDecal);


	track_spline->Destroy();
	is_level_spawnned = true;
}

void UUIWidget::OnTest(){
	TArray<FLinearColor> temp_color;
	p_mesh->Save(temp_vec, temp_color);
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = FVector(0, 0, 0);
	new_temp = GetWorld()->SpawnActor<AMyProceduralMesh>(myLoc, myRot, SpawnInfo);
	new_temp->Resize(temp_vec,2, temp_color);
	new_temp->SetActorScale3D(FVector(4, 4, 8));//scaling*(s/2)(8*10) =80
}

void UUIWidget::StartTextFunction() {
	if (vehicle_pawn) {
		if (vehicle_pawn->GetBoolStartText() == false && is_level_spawnned) {
			start_instruction_text->SetText(FText::FromString("Drive to the Start Line"));
		}
		else {
			start_instruction_text->SetText(FText::FromString(""));
		}
	}
}

void UUIWidget::HandBreakTextFunction() {
	if (vehicle_pawn->GetBoolStartText()){
		start_instruction_text->SetText(FText::FromString("Hold Down Handbreak to Start"));
	}
	if (vehicle_pawn->GetBoolCountdown()){
		start_instruction_text->SetText(FText::FromString(""));
	}
}

void UUIWidget::CountdownImageFunction(const float &dt) {
	static float counter_countdown = 0.0f;
	static int index_image = 0;
	if (vehicle_pawn->GetBoolCountdown()&&vehicle_pawn->GetBoolBeginLap()==false){
		counter_countdown += dt;
		if (counter_countdown>1.f){
			counter_countdown = 0.0f;
			images_[index_image]->SetVisibility(ESlateVisibility::Visible);
			index_image++;
		}
	}
	if (vehicle_pawn->GetBoolBeginLap()){
		images_[index_image]->SetVisibility(ESlateVisibility::Visible);
	}
}