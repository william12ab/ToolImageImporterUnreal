#include "UIWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include <Runtime/Engine/Public/ImageUtils.h>

void UUIWidget::NativeConstruct() {
	Super::NativeConstruct();
	w_ = 4;
	h_ = 4;
	s_ = 20.0f;		//distance between verts in proc mesh
	m_ = 7;			//division of height
	scaling_down_ = 10.0f;		//scale factor of everything, so 8 times what it is now
	test_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnTest);
	vehicle_pawn = Cast<AVehicleController>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));//casting to pawn

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
			p_mesh->SetIsChunking(true);
		}
		else {
			p_mesh->SetIsChunking(false);
		}
		point_type = level_loader.ReadTrackPoints(track_points, control_points, i);
		CreateTrack(i);
		FixScales(i);
		is_start_done = true;
	}
	if (is_chunking){
		p_mesh->TestFinal();
		p_mesh->FullSize();
	}
	p_mesh->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	OnTest();
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
}

void UUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	//UI FUNCS
	StartTextFunction();
	HandBreakTextFunction();
	CountdownImageFunction(InDeltaTime);
	LapTimerFunction(InDeltaTime);
	RenderTimer();

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
		if (vehicle_pawn->GetCounter() >= 1.5f) {
			if (vehicle_pawn->GetVelocityFromComp() < 5.f) {
				vehicle_pawn->SetPressed(false);
				float angle = atan2(track_points[index_recorder + 1].Y - last_point.Y, track_points[index_recorder + 1].X - last_point.X) * 180.0f / PI;
				last_point *= scaling_down_;
				last_point *= s_;
				last_point.Z += 5;
				while (!vehicle_pawn->TeleportTo(last_point, FRotator(0.0f, angle, 0.0f), false, false)) {
					last_point.Z += 0.1f;
				}
				counter_ = 0.0f;
			}
		}
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
		p_mesh = GetWorld()->SpawnActor<AMyProceduralMesh>(myLoc, myRot, SpawnInfo);
		is_created = true;
	}
	p_mesh->CreateMesh(h_, w_, s_, loop_index);
}

void UUIWidget::CreateTrack(const int& loop_index) {
	p_mesh->ModiVerts(m_colors, m_,loop_index);
	CreateFoilage(loop_index);
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

void UUIWidget::CreateFoilage(const int& loop_index) {
	int max = 0;
	int min = 100000000;
	int index = 0;
	for (int i = 0; i < p_mesh->vec_m_verts[loop_index].Num(); i++) {
		if (p_mesh->vec_m_verts[loop_index][i].Z > max) {
			max = p_mesh->vec_m_verts[loop_index][i].Z;
		}
		if (p_mesh->vec_m_verts[loop_index][i].Z < min) {
			min = p_mesh->vec_m_verts[loop_index][i].Z;
			index = i;
		}
	}
	CreateSpline(loop_index);
	FActorSpawnParameters SpawnInfoTree;
	FRotator myRotTree(0, 0, 0);
	FVector myLocTree = FVector(0, 0, 0);
	//for (int i = 0; i < 2; i++) {//tree near track
	//	ABasicTree* tree_instancea;
	//	tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
	//	tree_instancea->AddTreeNearTrack(track_points, vev_ground_meshes[loop_index]->m_verts, max, min);
	//	tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	//}
	//for (int i = 0; i < 4; i++) {//tree in general
	//	ABasicTree* tree_instancea;
	//	tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
	//	tree_instancea->AddClusterTrees(vev_ground_meshes[loop_index]->m_verts, max, min, track_points, false);
	//	tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	//}
	//for (int i = 0; i < 2; i++) {//ferns bushes
	//	ABasicTree* tree_instancea;
	//	tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
	//	tree_instancea->AddClusterTrees(vev_ground_meshes[loop_index]->m_verts, max, min, track_points, true);
	//	tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	//}

	//for (int i = 0; i < 1; i++) {//rocks
	//	ABasicTree* tree_instancea;
	//	tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
	//	tree_instancea->AddRockClusters(track_points, vev_ground_meshes[loop_index]->m_verts);
	//	tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	//}
	//for (int i = 0; i < 1; i++) {//grass
	//	ABasicTree* tree_instancea;
	//	tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
	//	tree_instancea->AddGrass(track_points, vev_ground_meshes[loop_index]->m_verts, max, min);
	//	tree_instancea->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	//}
	////water
	float water_height = (min + (max * 0.05f));
	if (water_height >= track_spline->GetMinHeight()) {
		water_height = track_spline->GetMinHeight();
	}
	myLocTree = FVector(p_mesh->vec_m_verts[loop_index][index].X, p_mesh->vec_m_verts[loop_index][index].Y, water_height + track_spline->GetHeightChange());
	vec_water_mesh[loop_index] = GetWorld()->SpawnActor<AWaterMesh>(myLocTree, myRotTree, SpawnInfoTree);
	vec_water_mesh[loop_index]->SetActorScale3D(FVector(30, 30, 30));
}
void UUIWidget::CreateSpline(const int&loop_index) {
	TArray<FVector2D> temp_arr;
	if (point_type) {
		temp_arr = track_points;
	}
	else {
		temp_arr = control_points;
	}

	for (size_t i = 0; i < temp_arr.Num(); i++) {
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
	p_mesh->SetHeightProper(track_spline->GetSEPoints(), track_spline->GetVerts(), loop_index);//changing height of mesh
	p_mesh->ReplaceC(m_colors,loop_index);//replacing heightmap to match new mesh, also normals and smoothing
	track_spline->SetActorLocation(FVector(track_spline->GetActorLocation().X, track_spline->GetActorLocation().Y, track_spline->GetActorLocation().Z));
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
void UUIWidget::InnerStartPlaces(const TArray<FVector>& point_arr, const int& loop_index) {
	if (!is_decal_spawn) {
		auto f = FMath::Lerp(point_arr[0], point_arr[1], 0.25f);
		auto total = track_spline->GetTotalPoints();
		f *= scaling_down_;
		float angle_f = atan2(point_arr[1].Y - point_arr[0].Y, point_arr[1].X - point_arr[0].X) * 180.0f / PI;
		starting_angle = FRotator(0.f, angle_f, 0.f);
		while (!vehicle_pawn->TeleportTo(f, starting_angle, false, false)) {
			f.Z += 0.5f;
		}

		auto ss = point_arr.Num();
		FActorSpawnParameters SpawnInfoDecal;
		FActorSpawnParameters SpawnInfoBox = FActorSpawnParameters();
		FRotator myRotD(0, 0, 0);
		FVector myLocD = FMath::Lerp(point_arr[0], point_arr[1], 0.9f);

		myLocD *= scaling_down_;
		//myLocD.Z += 85.f;
		FName RightName = FName(TEXT("boxendtriggername"));
		SpawnInfoBox.Name = RightName;
		box_start = GetWorld()->SpawnActor<ATriggerBoxDecal>(myLocD, starting_angle, SpawnInfoDecal);
		start_decal = GetWorld()->SpawnActor<AStartDecalActor>(myLocD, starting_angle, SpawnInfoDecal);
		myLocD = point_arr[ss - 2];
		myLocD = FMath::Lerp(point_arr[ss - 2], point_arr[ss - 1], 0.9f);
		myLocD *= scaling_down_;
		if (!is_chunking){
			myLocD = point_arr[ss - 2];
			myLocD = FMath::Lerp(point_arr[ss - 2], point_arr[ss - 1], 0.9f);
			myLocD *= scaling_down_;
			float end_f = atan2(point_arr[ss - 1].Y - point_arr[ss - 2].Y, point_arr[ss - 1].X - point_arr[ss - 2].X) * 180.0f / PI;
			myRotD = FRotator(0, end_f, 0);

			box_end = GetWorld()->SpawnActor<ATriggerBoxDecal>(myLocD, myRotD, SpawnInfoBox);
			end_decal = GetWorld()->SpawnActor<AStartDecalActor>(myLocD, myRotD, SpawnInfoDecal);
		}

		is_decal_spawn = true;
	}
	
}

//point type is true when curved, with width, or both
void UUIWidget::StartPlaces(const int& loop_index) {
	if (!point_type){
		InnerStartPlaces(track_spline->GetTotalPoints(),0);
	}
	else{
		//finds the correct height(z) for the control points to build fvector for position for car and decals
		TArray<FVector> control_points_with_z;
		for (int i = 0; i < control_points.Num(); i++){
			int xp = control_points[i].X;
			int yp = control_points[i].Y;
			float z_from_p_mesh = p_mesh->vec_m_verts[loop_index][(yp) * 400 + (xp)].Z;
			control_points_with_z.Add(FVector(control_points[i].X*s_, control_points[i].Y * s_, z_from_p_mesh));
		}
		if (control_points_with_z[0] == control_points_with_z[1]){
			int sf = 23;//do something about this
		}
		InnerStartPlaces(control_points_with_z,0);
	}
}
void UUIWidget::EndFlag(const TArray<FVector>& point_arr, const int& loop_index) {
	if (is_chunking && loop_index == 2) {
		FActorSpawnParameters SpawnInfoDecal;
		FActorSpawnParameters SpawnInfoBox = FActorSpawnParameters();
		FName RightName = FName(TEXT("boxendtriggername"));
		SpawnInfoBox.Name = RightName;
		auto ss = point_arr.Num();
		FVector myLocD;
		myLocD = point_arr[ss - 2];
		myLocD = FMath::Lerp(point_arr[ss - 2], point_arr[ss - 1], 0.9f);
		myLocD.Y /= 20; myLocD.Y += 400;
		myLocD.Y *= 20; 
		myLocD *= scaling_down_;
		FRotator myRotD(0, 0, 0);
		float end_f = atan2(point_arr[ss - 1].Y - point_arr[ss - 2].Y, point_arr[ss - 1].X - point_arr[ss - 2].X) * 180.0f / PI;
		myRotD = FRotator(0, end_f, 0);

		box_end = GetWorld()->SpawnActor<ATriggerBoxDecal>(myLocD, myRotD, SpawnInfoBox);
		end_decal = GetWorld()->SpawnActor<AStartDecalActor>(myLocD, myRotD, SpawnInfoDecal);
	}
	//divide by 10,20 add 400
}

void UUIWidget::FixScales(const int& loop_index) {
	//setting scales of terrain, spline, water.
	track_spline->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	vec_water_mesh[0]->SetActorScale3D(FVector(30 * scaling_down_, 30 * scaling_down_, 30 * scaling_down_));
	auto t = vec_water_mesh[0]->GetActorLocation();
	vec_water_mesh[0]->SetActorLocation(t * scaling_down_);
	if (!is_start_done){
		StartPlaces(0);
	}
	if (!point_type) {
		EndFlag(track_spline->GetTotalPoints(),loop_index); 
	}
	else{
		TArray<FVector> control_points_with_z;
		for (int i = 0; i < control_points.Num(); i++) {
			int xp = control_points[i].X;
			int yp = control_points[i].Y;
			float z_from_p_mesh = p_mesh->vec_m_verts[loop_index][(yp) * 400 + (xp)].Z;
			control_points_with_z.Add(FVector(control_points[i].X * s_, control_points[i].Y * s_, z_from_p_mesh));
		}
		EndFlag(control_points_with_z, loop_index);
	}
	
	//removes spline and starts the level, bool used for triggering start ui 
	track_spline->Destroy();
	is_level_spawnned = true;
}

void UUIWidget::OnTest() {
	TArray<FLinearColor> temp_color;
	p_mesh->Save(temp_vec, temp_color);
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = FVector(0, 0, 0);
	new_temp = GetWorld()->SpawnActor<AMyProceduralMesh>(myLoc, myRot, SpawnInfo);
	new_temp->SetIsTemp(true);
	if (is_chunking){
		new_temp->SetIsChunking(true);
	}
	new_temp->Resize(temp_vec, 4, temp_color,0);
	new_temp->SetActorScale3D(FVector(2.5f, 2.5f, 10));//2.5 for 4 times increase, 5 times for 2. so scaling/increase
	p_mesh->Destroy();
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
			images_[index_image]->SetVisibility(ESlateVisibility::Visible);
			index_image++;
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
			images_[index_image]->SetVisibility(ESlateVisibility::Visible);
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