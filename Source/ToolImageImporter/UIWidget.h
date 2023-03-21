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
	void ReadSliders();			//reads the values from the sliders and sets them to text fields
	void SliderFunc(const int& val_, UEditableTextBox* text_box);
	void CreateTrack();
	void CreateFoilage();
	void CreateSpline();
	void FixScales();


	void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
		class UButton* delete_button;
	UPROPERTY(meta = (BindWidget))
		class UButton* file_button;

	UPROPERTY(meta = (BindWidget))
		class UButton* update_button;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Label;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* height_label;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* width_label;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* spacing_label;
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* height_modi;

	UPROPERTY(meta = (BindWidget))
		class USlider* modi_slider;
	UPROPERTY(meta = (BindWidget))
		class USlider* padding_slider;

	UFUNCTION()
		void OnClickLoadNewTrack();		//on click load button
	UFUNCTION()
		void OnClickDelete();		//calls delete
	UFUNCTION()
		void OnEnterText();			//allows the user to enter text to boxes
	UFUNCTION()
		void OnClickUpdateButton();		//updates the mesh with the new values from the slider. couldnt figure out how to work the sldier function

		
	AWaterMesh* w_mesh;
	AMyProceduralMesh* p_mesh;		//obj
	//ATrackMesh* track_mesh;
	ABasicTree* tree_instance;
	int32 w_, h_,m_;		//width, height, modifier
	float s_;				//spacing

	FString name_;

	TArray<float> m_colors;
	UTexture* t_;

	TArray<FVector2D> control_points;
	TArray<FVector2D> track_points;
	APawn* player_pawn;

	ATrackSpline* track_spline;

	float pitch_;
	float yaw_;
	float rot_speed_;
};
