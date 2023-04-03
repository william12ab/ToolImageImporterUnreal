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
	scaling_down_ = 8.0f;		//scale factor of everything, so 8 times what it is now
	Label->SetText(FText::FromString("Plane Generator"));

	delete_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickDelete);
	file_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickLoadNewTrack);
	test_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnTest);
	pitch_ = 0.0f;
	//player_pawn = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	pressed_ = false;
	counter_ = 0.0f;
	rot_speed_ = 100.0f;
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

void AngleCap(float& angle_){
	if (angle_ < -90.0f) {
		angle_ = -90.0f;
	}
	else if (angle_ > 90.0f) {
		angle_ = 90.0f;
	}
}

void RotatarFinder(const float &d_one, const float& d_two,float &angle_, const float &d_t, const float& rot_speed)
{
	if (d_one == -1.0f) {
		angle_ -= rot_speed * d_t;
		
	}
	else if (d_two == 1.0f) {
		angle_ += rot_speed * d_t;
	}
	else {
		if (angle_ < -(1.0f)) {
			angle_ += rot_speed * d_t;
		}
		else if (angle_ > (1.0f)) {
			angle_ -= rot_speed * d_t;
		}
		else {
			angle_ = 0.0f;
		}
	}
	AngleCap(angle_);
}

void UUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime){
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);
	// Do your custom tick stuff here
	
	//setting camera work
	//if (player_pawn) {
	//	auto r = player_pawn->GetActorRotation();
	//	auto t = player_pawn->InputComponent->GetAxisValue(FName("LookUp"));
	//	if (player_pawn) {
	//		auto side_right = player_pawn->InputComponent->GetAxisValue(FName("LookRight")); //here

	//		auto side_left = player_pawn->InputComponent->GetAxisValue(FName("LookLeft"));
	//		side_left *= -1.0f;

	//		RotatarFinder(side_left, side_right, yaw_, InDeltaTime, rot_speed_);
	//		RotatarFinder(t, t, pitch_, InDeltaTime, rot_speed_);
	//		r.Pitch += pitch_;
	//		r.Yaw += yaw_;
	//		player_pawn->GetController()->SetControlRotation(r);
	//		//camera end


	//		//collisons for restarting position
	//		auto l = player_pawn->GetActorLocation();
	//		l /= s_;
	//		l /= scaling_down_;
	//		FVector2D current_point = FVector2D(l.X, l.Y);
	//		for (int i = 0; i < track_points.Num(); i++) {
	//			if (static_cast<int>(current_point.X) == static_cast<int>(track_points[i].X) && static_cast<int>(current_point.Y) == static_cast<int>(track_points[i].Y)) {
	//				last_point = FVector(track_points[i].X, track_points[i].Y, l.Z);
	//				index_recorder = i;
	//			}
	//		}

	//		player_pawn->InputComponent->BindAction("Restart", IE_Pressed, this, &UUIWidget::RestartPosition);
	//		player_pawn->InputComponent->BindAction("Restart", IE_Released, this, &UUIWidget::Release);

	//		if (pressed_) {
	//			counter_ += InDeltaTime;
	//			if (counter_ >= 1.5f) {
	//				pressed_ = false;
	//				float angle = atan2(track_points[index_recorder + 1].Y - last_point.Y, track_points[index_recorder + 1].X - last_point.X) * 180.0f / PI;
	//				last_point *= scaling_down_;
	//				last_point *= s_;
	//				player_pawn->TeleportTo(last_point, FRotator(0.0f, angle, 0.0f));
	//			}
	//		}
	//	}
	//}
}

void UUIWidget::RestartPosition(){
	pressed_ = true;
}

void UUIWidget::Release(){
	pressed_ = false;
	counter_ = 0.0f;
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

		if (array_[i].Contains(FString("end"))){
			found_ = true;
		}
		else
		{
			if (!found_) {
				auto index_ = array_[0].Find(" ");
				control_points.Add(FVector2D(FCString::Atoi(*array_[i]), FCString::Atoi(*array_[i].Right(index_ + 1))));
			}
			else {
				auto index_ = array_[0].Find(" ");
				track_points.Add(FVector2D(FCString::Atoi(*array_[i]), FCString::Atoi(*array_[i].Right(index_ + 1))));
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
	for (int t = 0; t < 10; t++) {
		float t_val = t / 10.f;
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
	LerpCalculation(temp_vec_first_pos, 0, 0);
	temp_vec_first_pos.Empty();

	LerpCalculation(temp_vec_first_pos, 1, track_points.Num()-1);
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
	myLocTree = FVector(p_mesh->m_verts[index].X, p_mesh->m_verts[index].Y, (min + (max * 0.050f)));
	w_mesh = GetWorld()->SpawnActor<AWaterMesh>(myLocTree, myRotTree, SpawnInfoTree);
	w_mesh->SetActorScale3D(FVector(30, 30, 30));
	FixScales();

}


void UUIWidget::CreateSpline(){
	for (size_t i = 0; i < control_points.Num(); i++)
	{
		control_points[i].X *= s_;
		control_points[i].Y *= s_;
		//track_points[i].X *= s_;
		//track_points[i].Y *= s_;
	}
	FActorSpawnParameters SpawnInfoTree;
	FRotator myRotTree(0, 0, 0);
	FVector myLocTree = FVector(0, 0, 0);
	FTransform t_transform_{
					FRotator{0,0,0},
					FVector{0, 0, 0},
					FVector{1, 1, 1} };
	track_spline = GetWorld()->SpawnActor<ATrackSpline>(myLocTree, myRotTree, SpawnInfoTree);
	track_spline->SetSpacing(s_);
	track_spline->SetDivision(m_);
	track_spline->SetControlPoints(control_points);
	track_spline->SetHeightArray(m_colors);
	track_spline->OnConstruction(t_transform_);
	p_mesh->SetHeightProper(track_spline->GetSEPoints(), track_spline->GetVerts());
	p_mesh->ReplaceC(m_colors);
	track_spline->SetActorLocation(FVector(track_spline->GetActorLocation().X, track_spline->GetActorLocation().Y, track_spline->GetActorLocation().Z + 27.f));
	track_spline->SetActorEnableCollision(true);
	//for (size_t i = 0; i < track_points.Num(); i++)
	//{
	//	//control_points[i].X *= s_;
	//	//control_points[i].Y *= s_;
	//	track_points[i].X /= s_;
	//	track_points[i].Y /= s_;
	//}
}

void UUIWidget::FixScales()
{
	p_mesh->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	track_spline->SetActorScale3D(FVector(scaling_down_, scaling_down_, scaling_down_));
	w_mesh->SetActorScale3D(FVector(30* scaling_down_, 30* scaling_down_, 30* scaling_down_));
	auto t = w_mesh->GetActorLocation();
	w_mesh->SetActorLocation(t* scaling_down_);
	FVector loc_ = track_spline->GetSEPoints()[0];

	loc_ *= scaling_down_;
	//loc_.X += 220.f;
	float angle = atan2(track_spline->GetSEPoints()[1].Y - track_spline->GetSEPoints()[0].Y, track_spline->GetSEPoints()[1].X - track_spline->GetSEPoints()[0].X) * 180.0f / PI;
	//player_pawn->TeleportTo(loc_, FRotator(0.0f, angle, 0.0f));
	track_spline->Destroy();
}


void UUIWidget::OnTest()
{
	TArray<FLinearColor> temp_color;
	p_mesh->Save(temp_vec, temp_color);
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = FVector(0, 0, 0);

	
	new_temp = GetWorld()->SpawnActor<AMyProceduralMesh>(myLoc, myRot, SpawnInfo);

	new_temp->Resize(temp_vec,2, temp_color);
	new_temp->SetActorScale3D(FVector(80, 80, scaling_down_));

}