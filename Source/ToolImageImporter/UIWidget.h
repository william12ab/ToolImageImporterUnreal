// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "MyProceduralMesh.h"
#include "TrackMesh.h"
#include "TrackInstance.h"
#include "BasicTree.h"
#include "WaterMesh.h"
#include "TrackSpline.h"
#include "VehicleController.h"
#include "StartDecalActor.h"
#include "TriggerBoxDecal.h"
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
	void CreateTrack();//creates track
	void CreateFoilage();//creates foilage
	void CreateSpline();//spine
	void FixScales();//does scale of everything
	void FillInGaps();//creates end and start
	void LerpCalculation(TArray<FVector2D>& temp_arr, const int& index_saftey_p, const int& index_t_p);//used with above function
	void StartTextFunction();//ui func
	void HandBreakTextFunction();//ui func
	void CountdownImageFunction(const float& dt);
	void LapTimerFunction(const float& dt);
	void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	//ui 
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* start_instruction_text;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* lap_timer_text;
	UPROPERTY(meta = (BindWidget))
		class UButton* delete_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* file_button;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Label;
	UPROPERTY(meta = (BindWidget))
		class UButton* test_button;
	UPROPERTY(meta = (BindWidget))
		class UImage* image_slot_1;
	UPROPERTY(meta = (BindWidget))
		class UImage* image_slot_2;
	UPROPERTY(meta = (BindWidget))
		class UImage* image_slot_3;
	UPROPERTY(meta = (BindWidget))
		class UImage* image_slot_4;
	UPROPERTY(meta = (BindWidget))
		class UImage* image_slot_5;
	UFUNCTION()
		void OnClickLoadNewTrack();		//on click load button
	UFUNCTION()
		void OnClickDelete();		//calls delete
	UFUNCTION()
		void OnTest();//new heightmap with more verts merge this

	TArray<UImage*> images_;

	AWaterMesh* w_mesh;//water
	AMyProceduralMesh* p_mesh;		//obj
	ABasicTree* tree_instance;//trees
	int32 w_, h_,m_;		//width, height, modifier
	float s_;				//spacing
	float scaling_down_;//scaling factor, so multiples everything so set scale
	FString name_;
	bool point_type; //used for reading the data from file and choosing what points to use in generation. true = t.p, false = c.p

	AMyProceduralMesh* new_temp;//new mesh
	AStartDecalActor* start_decal;
	AStartDecalActor* end_decal;
	ATriggerBoxDecal* box_start;
	ATriggerBoxDecal* box_end;

	TArray<float> m_colors;//heightmapcolours
	UTexture* t_;//texutre obj for terrain 

	TArray<FVector2D> control_points;//control points
	TArray<FVector2D> track_points;
	AVehicleController* vehicle_pawn;
	ATrackSpline* track_spline;//track spline
	int index_recorder;//used for resetting car to track
	FVector last_point;//used for keeping track of last active point on car
	float counter_;//time counter for reset
	TArray<FVector> temp_vec;//used for filling in gaps, creating new trackpoints for end and start


	bool is_level_spawnned;

	//for UI
	int minutes;
	int seconds;
	float point_seconds;

};
