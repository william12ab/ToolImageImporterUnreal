#include "UIWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include <Runtime/Engine/Public/ImageUtils.h>

void UUIWidget::NativeConstruct(){
	Super::NativeConstruct();
	w_ = 4;
	h_ = 4;
	s_ = 20.0f;		//distance between verts in proc mesh
	m_ = 7;			//division of height
	scaling_down_ = 10.0f;		//scale factor of everything, so 8 times what it is now
	test_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnTest);
	vehicle_pawn = Cast<AVehicleController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));//casting to pawn

	point_type = false;
	m_colors= level_loader.ReadFileInfo(h_, w_);
	GeneratePlane();
	point_type = level_loader.ReadTrackPoints(track_points,control_points);
	CreateTrack();

	counter_ = 0.0f;//for resetting postion
	is_level_spawnned = true;//see .h

	//ui images
	images_.Add(image_slot_1);
	images_.Add(image_slot_2);
	images_.Add(image_slot_3);
	images_.Add(image_slot_4);
	images_.Add(image_slot_5);
	//for timer
	minutes=0;
	seconds=0;
	point_seconds=0.0f;
	counter_countdown=0.0f;
	index_image=0;
}

void UUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime){
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	//UI FUNCS
	StartTextFunction();
	HandBreakTextFunction();
	CountdownImageFunction(InDeltaTime);
	LapTimerFunction(InDeltaTime);

	//RESTARTING lap
	RestartLap();

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

void UUIWidget::GeneratePlane(){
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = FVector(0,0,0);
	p_mesh = GetWorld()->SpawnActor<AMyProceduralMesh>(myLoc, myRot, SpawnInfo);
	p_mesh->CreateMesh(h_,w_,s_);
}

void UUIWidget::CreateTrack(){
	p_mesh->ModiVerts(m_colors, m_);
	CreateFoilage();
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
	for (int i = 0; i < 4; i++) {//tree near track
		ABasicTree* tree_instancea;
		tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
		tree_instancea->AddTreeNearTrack(track_points, p_mesh->m_verts,max,min);
		tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	}
	for (int i = 0; i < 4; i++) {//tree in general
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
	myLocTree = FVector(p_mesh->m_verts[index].X, p_mesh->m_verts[index].Y, water_height+track_spline->GetHeightChange());
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
	track_spline->SetActorLocation(FVector(track_spline->GetActorLocation().X, track_spline->GetActorLocation().Y, track_spline->GetActorLocation().Z ));
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
	//setting scales of terrain, spline, water.
	p_mesh->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	track_spline->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	w_mesh->SetActorScale3D(FVector(30* scaling_down_, 30* scaling_down_, 30* scaling_down_));
	auto t = w_mesh->GetActorLocation();
	w_mesh->SetActorLocation(t* scaling_down_);

	auto middle_point = FMath::Lerp(track_spline->GetSEPoints()[0], track_spline->GetSEPoints()[1], 0.5f);
	middle_point *= scaling_down_;
	float angle = atan2(track_spline->GetSEPoints()[1].Y - track_spline->GetSEPoints()[0].Y, track_spline->GetSEPoints()[1].X - track_spline->GetSEPoints()[0].X) * 180.0f / PI;
	starting_position = middle_point;
	starting_angle = FRotator(0.f, angle, 0.f);
	while (!vehicle_pawn->TeleportTo(middle_point, starting_angle, false, false)) {
		middle_point.Z += 1.f;
	}


	//trigger boxes used for end and start of the lap. - for starting and stopping timer etc...
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
	//removes spline and starts the level, bool used for triggering start ui 
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

void UUIWidget::LapTimerFunction(const float& dt) {
	if (vehicle_pawn->GetBoolBeginLap()) {
		float lap_time = vehicle_pawn->GetLapTimer();
		if (lap_time > 3.0f) {
			for (int i = 0; i < images_.Num(); i++) {
				images_[i]->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		if (lap_time>60){
			minutes = ((int)lap_time / 60);
			lap_time -=(60*minutes);
			seconds = (int)lap_time;
			lap_time -= seconds;
			point_seconds = lap_time;
			point_seconds *= 1000;
			point_sec_int = (int)point_seconds;
		}
		else{
			seconds = (int)lap_time;
			lap_time -= seconds;
			point_seconds = lap_time;
			point_seconds *= 1000;
			point_sec_int = (int)point_seconds;
		}
		FString timer_string = FString::FromInt(minutes);
		timer_string += ":";
		timer_string += FString::FromInt(seconds);
		timer_string += ":";
		timer_string += FString::FromInt(point_sec_int);
		lap_timer_text->SetText(FText::FromString(timer_string));
	}
}
void UUIWidget::RestartLap() {
	if (vehicle_pawn->GetIsRestartLevel()){
		auto temp_start = starting_position;
		while (!vehicle_pawn->TeleportTo(temp_start, starting_angle, false, false)) {
			temp_start.Z += 1.f;
		}
		vehicle_pawn->SetIsRestartLevel(); 
		FString timer_string = FString::FromInt(00);
		timer_string += ":";
		timer_string += FString::FromInt(00);
		timer_string += ":";
		timer_string += FString::FromInt(00);
		lap_timer_text->SetText(FText::FromString(timer_string));
		for (int i = 0; i < images_.Num(); i++) {
			images_[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
		counter_countdown = 0.0f;
		index_image = 0;
	}
}