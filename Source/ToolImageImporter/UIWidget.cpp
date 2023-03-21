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
	s_ = 20.0f;
	m_ = 3;
	Label->SetText(FText::FromString("Plane Generator"));

	delete_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickDelete);
	file_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickLoadNewTrack);
	update_button->OnClicked.AddUniqueDynamic(this, &UUIWidget::OnClickUpdateButton);
	pitch_ = 0.0f;
	player_pawn = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
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
	OnEnterText();
	ReadSliders();
	
	auto r = player_pawn->GetActorRotation();
	auto t = player_pawn->InputComponent->GetAxisValue(FName("LookUp"));
	auto side_right = player_pawn->InputComponent->GetAxisValue(FName("LookRight"));
	auto side_left = player_pawn->InputComponent->GetAxisValue(FName("LookLeft"));
	side_left *= -1.0f;

	RotatarFinder(side_left, side_right, yaw_, InDeltaTime, rot_speed_);
	RotatarFinder(t, t, pitch_, InDeltaTime, rot_speed_);
	r.Pitch += pitch_;
	r.Yaw += yaw_;
	player_pawn->GetController()->SetControlRotation(r);
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

void UUIWidget::OnClickUpdateButton() {
	//update mesh after changing padding
	p_mesh->Destroy();
	GeneratePlane();
	p_mesh->ModiVerts(m_colors, m_);
	p_mesh->SetMaterial(t_);
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

void UUIWidget::OnEnterText(){
	FText spacing_text = spacing_label->GetText();
	s_ = FCString::Atof(*spacing_text.ToString());
	FText height_text = height_modi->GetText();
	m_ = FCString::Atof(*height_text.ToString());
}


void UUIWidget::OnClickHeightmapButton(){
	ReadFileInfo(name_);
}

void UUIWidget::SliderFunc(const int& val_, UEditableTextBox* text_box){
	FString string_ = FString::FromInt(int(val_));
	text_box->SetText(FText::FromString(string_));
}

void UUIWidget::ReadSliders(){
	SliderFunc(modi_slider->GetValue(),height_modi);
	SliderFunc(padding_slider->GetValue(),spacing_label);
	height_label->SetText(FText::FromString(FString::FromInt(h_)));
	width_label->SetText(FText::FromString(FString::FromInt(w_)));
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


void UUIWidget::CreateFoilage()
{
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
	FActorSpawnParameters SpawnInfoTree;
	FRotator myRotTree(0, 0, 0);
	FVector myLocTree = FVector(0, 0, 0);
	for (int i = 0; i < 4; i++) {//tree
		ABasicTree* tree_instancea;
		tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
		tree_instancea->AddClusterTrees(p_mesh->m_verts, max, min, track_points, false);
		tree_instancea->SetActorScale3D(FVector(6, 6, 6));
	}
	for (int i = 0; i < 2; i++) {//ferns bushes
		ABasicTree* tree_instancea;
		tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
		tree_instancea->AddClusterTrees(p_mesh->m_verts, max, min, track_points, true);
		tree_instancea->SetActorScale3D(FVector(6, 6, 6));
	}
	for (int i = 0; i < 1; i++) {//rocks
		ABasicTree* tree_instancea;
		tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
		tree_instancea->AddRockClusters(track_points, p_mesh->m_verts);
		tree_instancea->SetActorScale3D(FVector(6, 6, 6));
	}
	for (int i = 0; i < 1; i++) {//grass
		ABasicTree* tree_instancea;
		tree_instancea = GetWorld()->SpawnActor<ABasicTree>(myLocTree, myRotTree, SpawnInfoTree);
		tree_instancea->AddGrass(track_points, p_mesh->m_verts, max, min);
		tree_instancea->SetActorScale3D(FVector(6, 6, 6));
	}
	//water
	myLocTree = FVector(p_mesh->m_verts[index].X, p_mesh->m_verts[index].Y, (min + (max * 0.050f)));
	w_mesh = GetWorld()->SpawnActor<AWaterMesh>(myLocTree, myRotTree, SpawnInfoTree);
	w_mesh->SetActorScale3D(FVector(30, 30, 30));
	CreateSpline();
}


void UUIWidget::CreateSpline(){
	for (size_t i = 0; i < control_points.Num(); i++)
	{
		control_points[i].X *= 20.0f;
		control_points[i].Y *= 20.0f;
	}
	FActorSpawnParameters SpawnInfoTree;
	FRotator myRotTree(0, 0, 0);
	FVector myLocTree = FVector(0, 0, 0);
	track_spline = GetWorld()->SpawnActor<ATrackSpline>(myLocTree, myRotTree, SpawnInfoTree);

	track_spline->SetControlPoints(control_points);
	track_spline->SetHeightArray(m_colors);
	FTransform t_transform_{
					FRotator{0,0,0},
					FVector{0, 0, 0},
					FVector{1, 1, 1} };	//Scale
	track_spline->OnConstruction(t_transform_);
	p_mesh->SetHeightProper(track_spline->GetSEPoints(), track_spline->GetVerts());
	track_spline->SetActorLocation(FVector(track_spline->GetActorLocation().X, track_spline->GetActorLocation().Y, track_spline->GetActorLocation().Z + 27.f));
	 
	track_spline->SetActorEnableCollision(true);
	FixScales();
}

void UUIWidget::FixScales()
{
	p_mesh->SetActorScale3D(FVector(6, 6, 6));
	track_spline->SetActorScale3D(FVector(6, 6, 6));
	w_mesh->SetActorScale3D(FVector(30*6, 30*6, 30*6));
	auto t = w_mesh->GetActorLocation();
	w_mesh->SetActorLocation(t*6.0f);
	FVector loc_ = track_spline->GetSEPoints()[0];

	loc_ *= 6.0f;
	loc_.X += 220.f;
	float angle = atan2(track_spline->GetSEPoints()[1].Y - track_spline->GetSEPoints()[0].Y, track_spline->GetSEPoints()[1].X - track_spline->GetSEPoints()[0].X) * 180.0f / PI;
	player_pawn->TeleportTo(loc_, FRotator(0.0f, angle, 0.0f));
}