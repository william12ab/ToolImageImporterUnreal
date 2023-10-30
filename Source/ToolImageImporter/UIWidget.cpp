#include "UIWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Components/CircularThrobber.h"
#include "DrawDebugHelpers.h"
#include <Runtime/Engine/Public/ImageUtils.h>
#include <chrono>
using namespace std::chrono;
void UUIWidget::NativeConstruct() {
	Super::NativeConstruct();
	w_ = 4;
	h_ = 4;
	s_ = 20.0f;		//distance between verts in proc mesh
	m_ = 7;			//division of height
	scaling_down_ = 10.0f;		//scale factor of everything, so 8 times what it is now
	test_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::ResizeMesh);
	vehicle_pawn = Cast<AVehicleController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));//casting to pawn
	pace_notes_actor = Cast<APaceNotesActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APaceNotesActor::StaticClass()));
	
	is_decal_spawn = false;
	point_type = false;
	is_chunking = level_loader.ReadMetaFile();
	int loop_index = 1;
	vec_water_mesh.Add(w_mesh);
	vec_water_mesh.Add(w_mesh1);
	vec_water_mesh.Add(w_mesh2);
	vec_water_mesh.Add(w_mesh3);
	is_start_done = false;
	is_created = false;
	TArray<int>temp_l; TArray<int>temp_a; TArray<int>temp_i; TArray<int>temp_w; TArray<int>temp_d; 
	level_loader.ReadPaceNoteFile(temp_l, temp_a, temp_i, temp_w,temp_d, temp_cp_w);
	pace_notes_actor->SetLengths(temp_l); pace_notes_actor->SetAngles(temp_a); pace_notes_actor->SetInclines(temp_i); pace_notes_actor->SetWidths(temp_w); pace_notes_actor->SetDirections(temp_d);
	pace_notes_actor->FindOrder();
	control_points_multi.SetNum(4);
	if (is_chunking) {
		loop_index = 4;
	}
	for (int i = 0; i < loop_index; i++) {
		m_colors.Empty();
		track_points.Empty();
		control_points.Empty();
		m_colors = level_loader.ReadFileInfo(h_, w_, i);
		GeneratePlane(i);
		if (is_chunking){
			original_plane->SetIsChunking(true);
		}
		else {
			original_plane->SetIsChunking(false);
		}
		point_type = level_loader.ReadTrackPoints(track_points, control_points, i);
		
		if (track_points.IsValidIndex(0)){
			CreateTrack(i);
			FixScales(i);
		}
		is_start_done = true;
		for (int32 p = 0; p < track_points.Num(); p++){
			if (i == 0) {
				total_track_points.Add(track_points[p]);
			}
			else if (i==1){
				auto total_point = track_points[p];
				total_point.X += 400;
				total_track_points.Add(total_point);
			}
			else if (i	 == 2) {
				auto total_point = track_points[p];
				total_point.Y += 400;
				total_track_points.Add(total_point);
			}
			else if (i == 3) {
				auto total_point = track_points[p];
				total_point.Y += 400;	total_point.X += 400;
				total_track_points.Add(total_point);
			}
		}
		control_points_multi[i] = control_points;
	}
	GetOrderOfControlPoints();
	if (is_chunking){
		original_plane->SetEdges();
	}
	if (is_chunking){//at this point youre merging them to create a full big one
		original_plane->TestFinal();		//smooths each section uniformly
		original_plane->SetEdges();
		original_plane->FullSize();			//adds all together
	}
	original_plane->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	ResizeMesh();//ading more verts
	SpawnStartEndFlags();
	//starting
	auto loc_ = start_decal->GetActorLocation();
	loc_.Z += 25;
	starting_angle.Yaw += 180;
	while (!vehicle_pawn->TeleportTo(loc_, starting_angle, false, false)) {
		loc_.Z += 0.5f;
	}
	starting_position = loc_;
	FVector first_pace = FVector(total_control_points[0].X*s_*scaling_down_, total_control_points[0].Y * s_ * scaling_down_, z_height[0]);
	first_pace *= scaling_down_;
	vehicle_pawn->SetPaceOne(first_pace);
	FVector2D first_paced= FVector2D(total_control_points[1].X * s_ * scaling_down_, total_control_points[1].Y * s_ * scaling_down_);
	first_paced *= scaling_down_;
	pacetwo = first_paced;
	
	counter_ = 0.0f;//for resetting postion
	is_level_spawnned = true;//see .h

	//ui images
	images_.Empty();
	images_.Add(image_slot_1);
	images_.Add(image_slot_2);
	images_.Add(image_slot_3);
	images_.Add(image_slot_4);
	images_.Add(image_slot_5);
	//for timer
	minutes = 0;
	seconds = 0;
	point_seconds = 0.0f;
	counter_countdown = 0.0f;
	index_image = 0;
	is_images_off = false;
	is_system_on = false;
	give_time_penalty = false;
	is_spinner_enabled = false;
	is_generated_boxes = false;
	for (size_t i = 0; i < total_control_points.Num(); i++){
		pacenote_c_p.Add(total_control_points[i]);
	}
	vehicle_pawn->GetMesh()->SetPhysicsAngularVelocity(FVector(0, 0, 0));
	vehicle_pawn->GetMesh()->SetAllPhysicsLinearVelocity(FVector(0, 0, 0));

	UE_LOG(LogTemp, Warning, TEXT("ui constructor"));
}

void UUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);
	//spawning boxes
	if (!is_generated_boxes){
		is_generated_boxes = true;
		SetControlPointTriggerBoxes();	
	}
	if (pacenote_c_p[0]== total_control_points[0]){
		pace_notes_actor->PlayFirstNote();
	}

	//UI FUNCS
	StartTextFunction();
	HandBreakTextFunction();
	CountdownImageFunction(InDeltaTime);
	LapTimerFunction(InDeltaTime);
	RenderTimer();
	//RESTARTING lap
	RestartLap();
	CheckForControlPointChange();
	pace_notes_actor->SetTurnCounter(vehicle_pawn->GetCPCounter());

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
		if (!is_spinner_enabled){
			triangle_button->SetVisibility(ESlateVisibility::Visible);
			is_spinner_enabled = true;
		}
		counter_ += InDeltaTime;
		vehicle_pawn->SetCounter(counter_);
		if (vehicle_pawn->GetCounter() >= 1.5f) {
			if (vehicle_pawn->GetVelocityFromComp() < 500.f) {
				vehicle_pawn->SetPressed(false);
				float angle = atan2(track_points[index_recorder + 1].Y - last_point.Y, track_points[index_recorder + 1].X - last_point.X) * 180.0f / PI;
				last_point *= scaling_down_;
				last_point *= s_;
				last_point.Z += 25;

				vehicle_pawn->GetMesh()->SetPhysicsAngularVelocity(FVector(0, 0, 0));
				vehicle_pawn->GetMesh()->SetAllPhysicsLinearVelocity(FVector(0, 0, 0));
				while (!vehicle_pawn->TeleportTo(last_point, FRotator(0.0f, angle, 0.0f), false, false)) {
					last_point.Z += 0.1f;
				}
				counter_ = 0.0f;
				triangle_button->SetVisibility(ESlateVisibility::Hidden);
				is_spinner_enabled = false;
			}
		}
	}
	else {
		triangle_button->SetVisibility(ESlateVisibility::Hidden);
		is_spinner_enabled = false;
	}
	if (vehicle_pawn->GetBoolEnd()) {
		SetLapTimeFinal();
	}
}

void UUIWidget::GeneratePlane(const int& loop_index) {
	if (!is_created){
		FActorSpawnParameters SpawnInfo;
		FRotator myRot(0, 0, 0);
		FVector myLoc = FVector(0, 0, 0);
		//original_plane = GetWorld()->SpawnActor<AMyProceduralMesh>(myLoc, myRot, SpawnInfo);
		original_plane = GetWorld()->SpawnActor<ARuntimeMeshPlane>(myLoc, myRot, SpawnInfo);
		is_created = true;
	}
	original_plane->CreateMesh(h_, w_, s_, loop_index);
}

void UUIWidget::CreateTrack(const int& loop_index) {
	original_plane->ModiVerts(m_colors, m_,loop_index);
	CreateFoilage(loop_index);
}


void UUIWidget::FixControlPoints(const int& index_, TArray<FVector2D>& points_to_fix) {
	switch (index_)
	{
	case 0: {
		break;
	}
	case 1: {
		for (size_t i = 0; i < points_to_fix.Num(); i++){
			points_to_fix[i].X += 400;
		}
		break;
	}
	case 2: {
		for (size_t i = 0; i < points_to_fix.Num(); i++) {
			points_to_fix[i].Y += 400;
		}
		break;
	}
	case 3: {
		for (size_t i = 0; i < points_to_fix.Num(); i++) {
			points_to_fix[i].Y += 400;
			points_to_fix[i].X += 400;
		}
		break;
	}
	}
}
void UUIWidget::LerpCalculation(TArray<FVector2D>& temp_arr, const int& index_saftey_p, const int& index_t_p) {
	auto safet_p = track_spline->GetSafetyPoints();
	for (int t = 0; t < 100; t++) {
		float t_val = t / 100.f;
		if (index_saftey_p == 1) {
			temp_arr.Add(track_spline->LerpV2D(track_points[index_t_p], (safet_p[index_saftey_p] / s_), t_val));
		}
		else {
			temp_arr.Add(track_spline->LerpV2D((safet_p[index_saftey_p] / s_), track_points[index_t_p], t_val));
		}
	}
	track_points.Insert(temp_arr, index_t_p);
}

void UUIWidget::FillInGaps() {
	if (track_spline->GetIsOutsideBounds()) {
		TArray<FVector2D> temp_vec_first_pos;
		LerpCalculation(temp_vec_first_pos, 0, 0);//between start and start respectively
		temp_vec_first_pos.Empty();
		LerpCalculation(temp_vec_first_pos, 1, track_points.Num() - 1);//end and end respectively
	}
	else {
		TArray<FVector2D> temp_vec_first_pos;
		LerpCalculation(temp_vec_first_pos, 0, 0);//between start and start respectively
		temp_vec_first_pos.Empty();
		LerpCalculation(temp_vec_first_pos, 1, track_points.Num() - 1);//end and end respectively
	}
}

void UUIWidget::CheckForChunking(const int& index_, ABasicTree* instance_) {
	if (is_chunking){
		if (index_==0){
			//do nada
		}
		else if (index_ == 1) {
			instance_->GetRootComponent()->SetMobility(EComponentMobility::Movable);
			auto loc_ = instance_->GetActorLocation();
			loc_.X += 80000;
			instance_->SetActorLocation(loc_);
		}
		else if (index_ ==2) {
			instance_->GetRootComponent()->SetMobility(EComponentMobility::Movable);
			auto loc_ = instance_->GetActorLocation();
			loc_.Y += 80000;
			instance_->SetActorLocation(loc_);
		}
		else if (index_ == 3) {
			instance_->GetRootComponent()->SetMobility(EComponentMobility::Movable);
			auto loc_ = instance_->GetActorLocation();
			loc_.X += 80000;
			loc_.Y += 80000;
			instance_->SetActorLocation(loc_);
		}
	}
}

void UUIWidget::CreateFoilage(const int& loop_index) {
	int max = 0;
	int min = 100000000;
	int index = 0;
	for (int i = 0; i < original_plane->vec_m_verts[loop_index].Num(); i++) {
		if (original_plane->vec_m_verts[loop_index][i].Z > max) {
			max = original_plane->vec_m_verts[loop_index][i].Z;
		}
		if (original_plane->vec_m_verts[loop_index][i].Z < min) {
			min = original_plane->vec_m_verts[loop_index][i].Z;
			index = i;
		}
	}
	CreateSpline(loop_index);
	if (track_points.IsValidIndex(0)) {
		FActorSpawnParameters SpawnInfoTree;
		FRotator myRotTree(0, 0, 0);
		FVector myLocTree = FVector(0, 0, 0);
		TArray<FVector2D>temp_array_half;
		TArray<FVector2D>temp_array_two;
		int split_size = track_points.Num() / 2;
		temp_array_half.SetNum(split_size);
		temp_array_two.SetNum(split_size);
		for (int i = 0; i < split_size; i++) {
			temp_array_half[i] = track_points[i];
			temp_array_two[i] = track_points[i + split_size];
		}

		for (int i = 0; i < 94; i++) {//tree near track
			ABasicTree* tree_instancea;
			tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
			if (i<=47){
				tree_instancea->AddTreeNearTrack(temp_array_half, original_plane->vec_m_verts[loop_index], max, min);
			}
			else {
				tree_instancea->AddTreeNearTrack(temp_array_two, original_plane->vec_m_verts[loop_index], max, min);
			}
			
			tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
			CheckForChunking(loop_index, tree_instancea);
		}
		for (int i = 0; i < 6; i++) {//tree in general
			ABasicTree* tree_instancea;
			tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
			tree_instancea->AddClusterTrees(original_plane->vec_m_verts[loop_index], max, min, track_points, false);
			tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
			CheckForChunking(loop_index, tree_instancea);
		}
		for (int i = 0; i < 6; i++) {//ferns bushes
			ABasicTree* tree_instancea;
			tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
			tree_instancea->AddClusterTrees(original_plane->vec_m_verts[loop_index], max, min, track_points, true);
			tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
			CheckForChunking(loop_index, tree_instancea);
		}
		for (int i = 0; i < 24; i++) {//rocks
			ABasicTree* tree_instancea;
			tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
			tree_instancea->AddRockClusters(track_points, original_plane->vec_m_verts[loop_index]);
			tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
			CheckForChunking(loop_index, tree_instancea);
		}
		for (int i = 0; i < 6; i++) {//grass
			ABasicTree* tree_instancea;
			tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
			tree_instancea->AddGrass(track_points, original_plane->vec_m_verts[loop_index], max, min);
			tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
			CheckForChunking(loop_index, tree_instancea);
		}
		for (int i = 0; i < 2; i++) {//grass verge
			ABasicTree* tree_instancea;
			tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
			tree_instancea->SetItem(i);
			tree_instancea->AddGrassAtEdge(original_plane->vec_m_verts[loop_index], original_plane->vec_m_vert_colors[loop_index], original_plane->GetHeight());
			tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
			CheckForChunking(loop_index, tree_instancea);
		}
		////water
		float water_height = (min + (max * 0.05f));
		if (water_height >= track_spline->GetMinHeight()) {
			water_height = track_spline->GetMinHeight();
		}
		myLocTree = FVector(original_plane->vec_m_verts[loop_index][index].X, original_plane->vec_m_verts[loop_index][index].Y, water_height + track_spline->GetHeightChange());
		vec_water_mesh[loop_index] = GetWorld()->SpawnActor<AWaterMesh>(myLocTree, myRotTree, SpawnInfoTree);
		vec_water_mesh[loop_index]->SetActorScale3D(FVector(30, 30, 30));
	}
}

void UUIWidget::FixMultipleSpline(TArray<FVector2D> &temp_arr, TArray<FVector2D>&second_spline_array, bool & is_new_spline_needed) {
	int split_index = 0;
	for (size_t i = 0; i < temp_arr.Num() - 1; i++) {
		auto dist = FVector2D::Distance(temp_arr[i], temp_arr[i + 1]);
		if (dist > 10) {
			is_new_spline_needed = true;
			split_index = i + 1;
			i = temp_arr.Num() - 1;
		}
	}

	if (is_new_spline_needed) {
		auto num_ = temp_arr.Num();
		for (size_t i = split_index; i < num_; i++) {
			second_spline_array.Add(temp_arr[i]);
		}
		temp_arr.RemoveAt(split_index, second_spline_array.Num());
	}

}

void UUIWidget::FixPoints(TArray<FVector2D>& array_) {
	for (auto& point : array_) {
		point.X *= s_;
		point.Y *= s_;
	}
}
void UUIWidget::DividingPoinfts(TArray<FVector2D>& array_) {
	for (auto& point : array_) {
		point.X /= s_;
		point.Y /= s_;
	}
}

void UUIWidget::SplineCreation(ATrackSpline* spline_obj,const TArray<FVector2D>& points_array, const int& loop_index, const FTransform& tranform_) {

	spline_obj->SetSpacing(s_);
	spline_obj->SetDivision(m_);//setters
	spline_obj->SetControlPoints(points_array);//setting array in class to the points
	spline_obj->SetHeightArray(m_colors);//setting array as well
	spline_obj->OnConstruction(tranform_);//consttruction
	original_plane->SetHeightProper(spline_obj->GetSEPoints(), spline_obj->GetVerts(), loop_index);//changing height of mesh
	original_plane->ReplaceC(loop_index);//replacing heightmap to match new mesh, also normals and smoothing
	spline_obj->SetActorLocation(FVector(spline_obj->GetActorLocation().X, spline_obj->GetActorLocation().Y, spline_obj->GetActorLocation().Z));
	spline_obj->SetActorEnableCollision(false);
}

void UUIWidget::CreateSpline(const int&loop_index) {
	TArray<FVector2D> temp_arr;
	TArray<FVector2D> second_spline_array;
	if (track_points.IsValidIndex(0)) {
		if (point_type) {
			temp_arr = track_points;
		}
		else {
			temp_arr = track_points;
		}

		bool is_new_spline_needed = false;
		FixMultipleSpline(temp_arr, second_spline_array, is_new_spline_needed);

		FixPoints(temp_arr);
		if (second_spline_array.IsValidIndex(0)){
			FixPoints(second_spline_array);
		}
		
		FActorSpawnParameters SpawnInfoTree;
		FRotator myRotTree(0, 0, 0);
		FVector myLocTree = FVector(0, 0, 0);
		FTransform t_transform_{
						FRotator{0,0,0},
						FVector{0, 0, 0},
						FVector{1, 1, 1} };
		track_spline = GetWorld()->SpawnActor<ATrackSpline>(myLocTree, myRotTree, SpawnInfoTree);//above to here just spawning actor
		SplineCreation(track_spline, temp_arr, loop_index,t_transform_);
		if (is_new_spline_needed){
			ATrackSpline* second_spline= GetWorld()->SpawnActor<ATrackSpline>(myLocTree, myRotTree, SpawnInfoTree);//above to here just spawning actor
			SplineCreation(second_spline, second_spline_array, loop_index, t_transform_);
		}

		DividingPoinfts(temp_arr);
		track_points = temp_arr;
		if (is_new_spline_needed){
			DividingPoinfts(second_spline_array);
		}
	}
}

void UUIWidget::FixScales(const int& loop_index) {
	//setting scales of terrain, spline, water.
	track_spline->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	vec_water_mesh[loop_index]->SetActorScale3D(FVector( scaling_down_, scaling_down_, scaling_down_));
	
	//removes spline and starts the level, bool used for triggering start ui 
	track_spline->Destroy();
	is_level_spawnned = true;
}

void UUIWidget::ResizeMesh() {
	TArray<FLinearColor> temp_color;
	TArray<FVector2D> temp_uvs;
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = FVector(0, 0, 0);
	new_plane = GetWorld()->SpawnActor<ARuntimeMeshPlane>(myLoc, myRot, SpawnInfo);
	track_plane = GetWorld()->SpawnActor<ARuntimeMeshPlane>(myLoc, myRot, SpawnInfo);

	original_plane->Save(temp_vec, temp_color, temp_uvs,0);
	new_plane->SetIsTemp(true);
	if (is_chunking) {
		new_plane->SetIsChunking(true);
	}
	new_plane->Resize(temp_vec, 2, temp_color, 0, temp_uvs);
	track_plane->CreateCollisionZone(total_track_points, new_plane->vec_m_verts[0], new_plane->GetHeight());
	track_plane->SetActorScale3D(FVector(5.f, 5.f, 10));
	new_plane->SetActorScale3D(FVector(5.f, 5.f, 10));//2.5 for 4 times increase, 5 times for 2. so scaling/increase
	track_plane->RuntimeMeshComponent->SetMassOverrideInKg(NAME_None, 1000000000.f);

	//getting z before destroyed
	for (int i = 0; i < total_control_points.Num(); i++) {
		int xp = total_control_points[i].X*2;
		int yp = total_control_points[i].Y*2;
		int resolution_ = 800;
		if (is_chunking){
			resolution_ = 1600;
		}
		float z_from_original_plane = new_plane->vec_m_verts[0][(yp) *resolution_ + (xp)].Z;
		z_height.Add(z_from_original_plane);
	}
	original_plane->Destroy();


	//water
	for (int32 i = 0; i < vec_water_mesh.Num(); i++){
		if (vec_water_mesh[i] != NULL) {
			auto loc_ = vec_water_mesh[i]->GetActorLocation();
			loc_ *= 10;
			vec_water_mesh[i]->SetActorLocation(loc_);
			vec_water_mesh[i]->SetActorScale3D(FVector(150.f, 150.f, 10.f));
		}
	}

}

void UUIWidget::StartTextFunction() {
	if (vehicle_pawn) {
		if (vehicle_pawn->GetBoolStartText() == false && is_level_spawnned) {
			start_image->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			start_image->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UUIWidget::HandBreakTextFunction() {
	if (vehicle_pawn->GetBoolStartText()) {
		instruction_image->SetVisibility(ESlateVisibility::Visible);
		if (!is_system_on) {
			light_system->SetVisibility(ESlateVisibility::Visible);
			is_system_on = true;
		}
	}
	if (vehicle_pawn->GetBoolCountdown()) {
		instruction_image->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUIWidget::CountdownImageFunction(const float& dt) {
	if (vehicle_pawn->GetBoolCountdown() && vehicle_pawn->GetBoolBeginLap() == false) {
		counter_countdown += dt;
		if (counter_countdown > 1.f) {
			counter_countdown = 0.0f;
			if (index_image<5){
				images_[index_image]->SetVisibility(ESlateVisibility::Visible);
				index_image++;
			}
		}
	}
	if (vehicle_pawn->GetBoolBeginLap()) {
	}
}

void UUIWidget::LapTimerFunction(const float& dt) {
	if (vehicle_pawn->GetBoolBeginLap() || vehicle_pawn->GetIsUnorthadox()) {
		SkipCountdown();
		float lap_time = vehicle_pawn->GetLapTimer();
		if (vehicle_pawn->GetIsUnorthadox()) {
			lap_time += 10.f;
		}
		if (!is_images_off && !vehicle_pawn->GetIsUnorthadox()) {
			if (index_image < 5) {
				images_[index_image]->SetVisibility(ESlateVisibility::Visible);
			}
			if (lap_time > 3.0f) {
				for (int i = 0; i < images_.Num(); i++) {
					images_[i]->SetVisibility(ESlateVisibility::Hidden);
				}
				light_system->SetVisibility(ESlateVisibility::Hidden);
				is_images_off = true;
			}
		}
		lap_time += vehicle_pawn->GetPenaltyTime();
		if (lap_time > 60) {
			minutes = ((int)lap_time / 60);
			lap_time -= (60 * minutes);
			seconds = (int)lap_time;
			lap_time -= seconds;
			point_seconds = lap_time;
			point_seconds *= 1000;
			point_sec_int = (int)point_seconds;
		}
		else {
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

void UUIWidget::SetLapTimeFinal() {
	auto from_disp = lap_timer_text->GetText();
	vehicle_pawn->SetFinalTime(from_disp);
	if (vehicle_pawn->GetIsUnorthadox()) {
		float pen = 10;
		pen += vehicle_pawn->GetPenaltyTime();

		FString string_ = "0:00:";
		string_ += FString::FromInt((int)pen);
		FText penalty_time = FText::FromString(string_);
		vehicle_pawn->SetPenTime(penalty_time);
	}
}

void UUIWidget::RestartLap() {
	if (vehicle_pawn->GetIsRestartLevel()) {

		auto temp_start = starting_position;
		vehicle_pawn->GetMesh()->SetPhysicsAngularVelocity(FVector(0, 0, 0));
		vehicle_pawn->GetMesh()->SetAllPhysicsLinearVelocity(FVector(0, 0, 0));
		temp_start.Z += 25;
		while (!vehicle_pawn->TeleportTo(temp_start, starting_angle, false, false)) {
			temp_start.Z += .1f;
		}
		vehicle_pawn->SetIsRestartLevel();
		FString timer_string = FString::FromInt(00);
		timer_string += ":";
		timer_string += FString::FromInt(00);
		timer_string += ":";
		timer_string += FString::FromInt(00);
		lap_timer_text->SetText(FText::FromString(timer_string));
		for (int i = 0; i < images_.Num(); i++) {
			images_[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		counter_countdown = 0.0f;
		index_image = 0;
		is_images_off = false;
		is_system_on = false;
	}
}
void UUIWidget::SkipCountdown() {
	if (vehicle_pawn->GetIsUnorthadox()) {
		for (int i = 0; i < images_.Num(); i++) {
			images_[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		light_system->SetVisibility(ESlateVisibility::Hidden);
		give_time_penalty = true;
	}
}
void UUIWidget::RenderTimer() {
	if (vehicle_pawn->GetIsRenderTimer()) {
		lap_timer_text->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		lap_timer_text->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUIWidget::EndFlag(const TArray<FVector>& point_arr, const int& loop_index) {//setting position 

	FActorSpawnParameters SpawnInfoDecal;
	FActorSpawnParameters SpawnInfoBox = FActorSpawnParameters();
	FName RightName = FName(TEXT("boxendtriggername"));
	SpawnInfoBox.Name = RightName;
	auto ss = point_arr.Num();

	FVector myLocD;
	myLocD = point_arr[ss - 2];
	myLocD = FMath::Lerp(point_arr[ss - 2], point_arr[ss - 1], 0.9f);
	myLocD.Z *= scaling_down_;
	FRotator myRotD(0, 0, 0);
	float end_f = atan2(point_arr[ss - 1].Y - point_arr[ss - 2].Y, point_arr[ss - 1].X - point_arr[ss - 2].X) * 180.0f / PI;
	myRotD = FRotator(0, end_f, 0);

	box_end = GetWorld()->SpawnActor<ATriggerBoxDecal>(myLocD, myRotD, SpawnInfoBox);
	end_decal = GetWorld()->SpawnActor<AStartDecalActor>(myLocD, myRotD, SpawnInfoDecal);
	//divide by 10,20 add 400
}

void UUIWidget::SpawnStartEndFlags() {
	TArray<FVector> control_points_with_z;
	for (int i = 0; i < total_control_points.Num(); i++) {
		int xp = total_control_points[i].X*2;
		int yp = total_control_points[i].Y*2;
		int resolution_ = 800;
		if (is_chunking) {
			resolution_ = 1600;
		}
		float z_from_original_plane = new_plane->vec_m_verts[0][(yp) *resolution_ + (xp)].Z;
		control_points_with_z.Add(FVector(total_control_points[i].X * s_ * scaling_down_, total_control_points[i].Y * s_ * scaling_down_, z_from_original_plane));
	}
	StartPlaces(0);
	if (!point_type) {
		EndFlag(control_points_with_z, 0);
	}
	else {
		EndFlag(control_points_with_z, 0);
	}
}
void UUIWidget::InnerStartPlaces(const TArray<FVector>& point_arr, const int& loop_index) {
	if (!is_decal_spawn) {
		auto ss = point_arr.Num();
		FActorSpawnParameters SpawnInfoDecal;
		FActorSpawnParameters SpawnInfoBox = FActorSpawnParameters();
		FRotator myRotD(0, 0, 0);
		FVector myLocD = point_arr[0];

		myLocD *= scaling_down_;
		FName RightName = FName(TEXT("boxendtriggername"));
		SpawnInfoBox.Name = RightName;
		float angle_f = atan2(point_arr[0].Y - point_arr[1].Y, point_arr[0].X - point_arr[1].X) * 180.0f / PI;
		starting_angle = FRotator(0.f, angle_f, 0.f);
		box_start = GetWorld()->SpawnActor<ATriggerBoxDecal>(myLocD, starting_angle, SpawnInfoDecal);
		start_decal = GetWorld()->SpawnActor<AStartDecalActor>(myLocD, starting_angle, SpawnInfoDecal);
		myLocD = point_arr[ss - 2];
		myLocD = FMath::Lerp(point_arr[ss - 2], point_arr[ss - 1], 0.9f);
		myLocD *= scaling_down_;
		is_decal_spawn = true;
	}
}

//point type is true when curved, with width, or both
void UUIWidget::StartPlaces(const int& loop_index) {
	TArray<FVector> control_points_with_z;
	for (int i = 0; i < total_control_points.Num(); i++) {
		int xp = total_control_points[i].X*2;
		int yp = total_control_points[i].Y*2;
		int resolution_ = 800;
		if (is_chunking) {
			resolution_ = 1600;
		}
		float z_from_original_plane = new_plane->vec_m_verts[0][(yp) *resolution_ + (xp)].Z;
		control_points_with_z.Add(FVector(total_control_points[i].X * s_, total_control_points[i].Y * s_, z_from_original_plane));
	}
	if (!point_type) {
		InnerStartPlaces(control_points_with_z, 0);
	}
	else {
		InnerStartPlaces(control_points_with_z, 0);
	}
}

void UUIWidget::SetControlPointTriggerBoxes() {
	for (size_t i = 0; i < total_control_points.Num(); i++){
		FActorSpawnParameters SpawnInfoDecal;
		FActorSpawnParameters SpawnInfoBox = FActorSpawnParameters();
		FVector myLocD = FVector(total_control_points[i].X * s_, total_control_points[i].Y * s_ , z_height[i]);
		myLocD *= scaling_down_;
		if (i==0){
			myLocD = box_start->GetActorLocation();
		}
		starting_angle = FRotator(0.f, 0, 0.f);
		if (i+1< total_control_points.Num()){
			float end_f = atan2(total_control_points[i].Y - total_control_points[i + 1].Y, total_control_points[i].X - total_control_points[i + 1].X) * 180.0f / PI;
			starting_angle = FRotator(0, end_f, 0);
		}
		AControlPointTriggerBox* control_point_trigger;
		control_point_trigger = GetWorld()->SpawnActor<AControlPointTriggerBox>(myLocD, starting_angle, SpawnInfoDecal);
		if (temp_cp_w.IsValidIndex(0)){
			auto scale = control_point_trigger->GetActorScale();
			control_point_trigger->SetActorScale3D(FVector(scale.X * temp_cp_w[i], scale.Y * temp_cp_w[i], scale.Z));
		}
		control_point_trigger->SetActorHiddenInGame(false);
	}
}
void UUIWidget::CheckForControlPointChange() {
	for (int i = 0; i < pacenote_c_p.Num(); i++){
		FVector2D pace1 = FVector2D(pacenote_c_p[i].X*s_, pacenote_c_p[i].Y*s_);
		pace1*=scaling_down_;
		FVector2D pacev1 = FVector2D(vehicle_pawn->GetPaceOne().X, vehicle_pawn->GetPaceOne().Y);
		if (pacev1== pace1){
			if (i + 1 < pacenote_c_p.Num()) {
				pacetwo = FVector2D(pacenote_c_p[i + 1].X * s_, pacenote_c_p[i + 1].Y * s_);
				pacetwo *= scaling_down_;
				FVector2D pacev3 = FVector2D(vehicle_pawn->GetPaceThree().X, vehicle_pawn->GetPaceThree().Y);
				pace_notes_actor->WhenToPlay(pacev1, pacetwo, pacev3);
			}
		}
		if (pace_notes_actor->GetIsPlayed()){
			pace_notes_actor->SetIsPlayed();
			pacenote_c_p.RemoveAt(i);
		}
	}
}

void UUIWidget::GetOrderOfControlPoints() {

	auto local_start = level_loader.GetStartEndPos(0);
	auto local_end = level_loader.GetStartEndPos(1);
	for (size_t i = 0; i < control_points_multi.Num(); i++) {
		FixControlPoints(i, control_points_multi[i]);
	}
	for (size_t i = 0; i < control_points_multi.Num(); i++){
		if (control_points_multi[i].IsValidIndex(0)) {
		}//do nothing
		else {
			control_points_multi.RemoveAt(i);
		}
	}
	int index_holder=0;
	for (size_t i = 0; i < control_points_multi.Num(); i++){
		if (control_points_multi[i].IsValidIndex(0)) {
			auto distance = FVector2D::Distance(control_points_multi[i][0], local_start);
			if (distance<10 ) {
				for (size_t j = 0; j < control_points_multi[i].Num(); j++) {
					total_control_points.Add(control_points_multi[i][j]);
					index_holder = i;
				}
			}
		}
	}

	control_points_multi.RemoveAt(index_holder);
	if (is_chunking) {
		if (control_points_multi.IsValidIndex(0)) {
			while (control_points_multi.Num() > 0) {
				for (size_t i = 0; i < control_points_multi.Num(); i++) {
					if (control_points_multi[i].IsValidIndex(0)) {
						if (control_points_multi.Num() == 3) {
							auto distance = FVector2D::Distance(total_control_points[total_control_points.Num() - 1], control_points_multi[i][0]);
							auto distance2 = FVector2D::Distance(total_control_points[total_control_points.Num() - 1], control_points_multi[i+1][0]);
							auto distance3 = FVector2D::Distance(total_control_points[total_control_points.Num() - 1], control_points_multi[i+2][0]);
							auto smallest = FMath::Min3(distance, distance2, distance3);
							int index_d = 0;
							if (smallest==distance){

							}
							else if (smallest == distance2) {
								index_d = 1;
							}
							else if (smallest == distance3) {
								index_d = 2;
							}
							for (size_t j = 0; j < control_points_multi[index_d].Num(); j++) {
								total_control_points.Add(control_points_multi[index_d][j]);
							}
							index_holder = index_d;
							control_points_multi.RemoveAt(index_holder);
						}
						else if (control_points_multi.Num() > 1&& (i+1)< control_points_multi.Num()) {
							auto distance = FVector2D::Distance(total_control_points[total_control_points.Num() - 1], control_points_multi[i][0]);
							float distance2;
							if (control_points_multi[i+1].IsValidIndex(0)){
								distance2 = FVector2D::Distance(total_control_points[total_control_points.Num() - 1], control_points_multi[i + 1][0]);
							}
							else {
								distance2 = 1000000000000;
								control_points_multi.RemoveAt(i + 1);
							}
							
							if (distance < distance2) {
								for (size_t j = 0; j < control_points_multi[i].Num(); j++) {
									total_control_points.Add(control_points_multi[i][j]);
								}
								index_holder = i;
								control_points_multi.RemoveAt(index_holder);
							}
							else {
								for (size_t j = 0; j < control_points_multi[i + 1].Num(); j++) {
									total_control_points.Add(control_points_multi[i + 1][j]);
								}
								index_holder = i + 1;
								control_points_multi.RemoveAt(index_holder);
							}
						}
						else if (control_points_multi.Num() == 1) {
							for (size_t j = 0; j < control_points_multi[i].Num(); j++) {
								total_control_points.Add(control_points_multi[i][j]);
							}
							control_points_multi.RemoveAt(i);
							//this should be a hopeful fix for if exists elsewhere
							auto local_index = total_control_points.Find(local_end);
							if (local_end !=total_control_points.Last()){
								total_control_points.RemoveAt(local_index);
								total_control_points.Add(local_end);
							}
						}
					}
				}
			}
		}
	}
}