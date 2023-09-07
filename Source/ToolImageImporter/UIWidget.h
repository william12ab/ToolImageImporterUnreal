#pragma once
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "MyProceduralMesh.h"
#include "TrackMesh.h"
#include "TrackInstance.h"
#include "BasicTree.h"
#include "WaterMesh.h"
#include "TrackSpline.h"
#include "VehicleController.h"
#include "StartDecalActor.h"
#include "TriggerBoxDecal.h"
#include "HeightmapHandler.h"
#include "PaceNotesActor.h"
#include "UIWidget.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API UUIWidget : public UUserWidget
{
	GENERATED_BODY()
public:
protected:
	void GeneratePlane(const int& loop_index);		//spawns plane using get world, etc and calls create mesh with params.
	void CreateTrack(const int& loop_index);//creates track
	void CreateFoilage(const int&loop_index);//creates foilage
	void CreateSpline(const int&loop_index);//spine
	void FixScales(const int&loop_index);//does scale of everything
	void FillInGaps();//creates end and start
	void LerpCalculation(TArray<FVector2D>& temp_arr, const int& index_saftey_p, const int& index_t_p);//used with above function
	void StartTextFunction();//ui func
	void HandBreakTextFunction();//ui func
	void CountdownImageFunction(const float& dt);//displays the traffic light starting images
	void LapTimerFunction(const float& dt);//for the timer display
	void RestartLap();//restating
	void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SkipCountdown();//skips countdown if user off before finsih
	void RenderTimer();//renders or unrenders timer
	void SetLapTimeFinal();//sets final time for display in end ui
	void StartPlaces(const int&loop_index);//used for choosing the starting places of both the flags and car
	void InnerStartPlaces(const TArray<FVector>& point_arr, const int&loop_index);
	void EndFlag(const TArray<FVector>& point_arr, const int& loop_index);

	void CheckForChunking(const int&index_, ABasicTree* instance_);
	//ui 
	UPROPERTY(meta = (BindWidget))
		class UImage* start_image;
	UPROPERTY(meta = (BindWidget))
		class UCircularThrobber* triangle_button;
	UPROPERTY(meta = (BindWidget))
		class UImage* instruction_image;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* lap_timer_text;
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
	UPROPERTY(meta = (BindWidget))
		class UImage* light_system;
	UFUNCTION()
		void ResizeMesh();//new heightmap with more verts merge this
	TArray<UImage*> images_;//has UI images - the starting countdown set

	TArray<FVector2D> total_track_points;

	AWaterMesh* w_mesh;//water
	AWaterMesh* w_mesh1;
	AWaterMesh* w_mesh2;
	AWaterMesh* w_mesh3;
	AMyProceduralMesh* p_mesh;		//obj
	AMyProceduralMesh* track_obj;
	ABasicTree* tree_instance;//trees
	int32 w_, h_,m_;		//width, height, modifier
	float s_;				//spacing
	float scaling_down_;//scaling factor, so multiples everything so set scale
	FString name_;//name of file
	bool point_type; //used for reading the data from file and choosing what points to use in generation. true = t.p, false = c.p
	AMyProceduralMesh* new_temp;//new mesh
	AStartDecalActor* start_decal;//decal start
	AStartDecalActor* end_decal;//decal end
	ATriggerBoxDecal* box_start;//trigger box 
	ATriggerBoxDecal* box_end;//trigger box
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
	HeightmapHandler level_loader;//for loading map
	APaceNotesActor* pace_notes_actor;
	FVector starting_position;//starting position
	FRotator starting_angle;//starting angle

	bool is_level_spawnned;//singalling if everything is generated
	bool give_time_penalty;//for false start
	//for UI
	int minutes;
	int seconds;
	float point_seconds;
	int point_sec_int;
	float counter_countdown;//countdown for the start
	int index_image;//index of the images when dispplaying them
	bool is_images_off;//to stop changing visibility or checking constantly
	bool is_system_on;//for same above

	//for chunking 
	TArray<AWaterMesh*> vec_water_mesh;//for water mesh
	bool is_chunking;
	bool is_start_done;
	bool is_created;
	bool is_decal_spawn;
	bool is_spinner_enabled;
};
