// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "MyProceduralMesh.h"
#include "TrackMesh.h"
#include "TrackInstance.h"
#include "BasicTree.h"
#include "WaterMesh.h"
#include "TrackSpline.h"
#include "UIWidget.generated.h"


UCLASS()
class TOOLIMAGEIMPORTER_API UUIWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	void LoadTrackPointsIn();		//lodas in the text file and parses the data into a vector2d
	void OnClickHeightmapButton();		//calls readfile info onn button heightmap set
	void GeneratePlane();		//spawns plane using get world, etc and calls create mesh with params.
	void DeletePlane();			//deletes the plane
	void ReadFileInfo(const FString& name_);		//reads file for height map also calls generate plane and modi verts.
	void CreateTrack();
	void CreateFoilage();
	void CreateSpline();
	void FixScales();
	void FillInGaps();
	void LerpCalculation(TArray<FVector2D>& temp_arr, const int& index_saftey_p, const int& index_t_p);

	void RestartPosition();
	void Release();
	void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
		class UButton* delete_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* file_button;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Label;

	UPROPERTY(meta = (BindWidget))
		class UButton* test_button;

	UFUNCTION()
		void OnClickLoadNewTrack();		//on click load button
	UFUNCTION()
		void OnClickDelete();		//calls delete

	UFUNCTION()
		void OnTest();
		
	AWaterMesh* w_mesh;
	AMyProceduralMesh* p_mesh;		//obj
	ABasicTree* tree_instance;
	int32 w_, h_,m_;		//width, height, modifier
	float s_;				//spacing
	float scaling_down_;
	FString name_;


	AMyProceduralMesh* new_temp;

	TArray<float> m_colors;
	UTexture* t_;

	TArray<FVector2D> control_points;
	TArray<FVector2D> track_points;
	//APawn* player_pawn;

	ATrackSpline* track_spline;

	float pitch_;
	float yaw_;
	float rot_speed_;

	int index_recorder;

	FVector last_point;
	bool pressed_;
	float counter_;


	TArray<FVector> temp_vec;
};
