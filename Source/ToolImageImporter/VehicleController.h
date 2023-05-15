#pragma once
#include "WheeledVehicle.h"
#include "VehicleController.generated.h"
UCLASS()
class TOOLIMAGEIMPORTER_API AVehicleController : public AWheeledVehicle
{
	GENERATED_BODY()
public:

	AVehicleController();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	//sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* EngineSoundCue;

	//hud
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText SpeedDisplayString;
	/** The current gear as a string (R,N, 1,2 etc) */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText GearDisplayString;
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText RPMDisplayString;
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		/** The color of the incar gear text in forward gears */
		FColor	GearDisplayColor;
	/** The color of the incar gear text when in reverse */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FColor	GearDisplayReverseColor;
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
		bool is_in_reverse_gear;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void RotatarFinder(const float& d_one, const float& d_two, float& angle_, const float& d_t, const float& rot_speed);
	void AngleCap(float& angle_);
	void StartFunction(const float& dt);
	void UpdateHUDStrings();
	void ChangeBrakeSystem();
	void CheckForStart();

	//action maps
	void Handbrake();
	void HandbrakeRelease();
	void Restart();
	void RestartMainLevel();
	void SwitchCamera();
	void ActiveCamera();
	void RestartPosition();
	void Release();
	void CameraReverseView();
	void CameraReverseViewRelease();
	void GearUp();
	void GearDown();
	//axis maps
	void MoveForward(float AxisValue);
	void Brake(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookLeft(float AxisValue);
	void LookRight(float AxisValue);
	//setter
	void SetPressed(bool b_) { pressed_ = b_; }
	void SetCounter(float c_) { counter_ = c_; }
	void SetIsRestartLevel() { is_restart_level = false; }
	void SetPause(bool p_) { is_paused = p_; }
	//getter
	bool GetPressed() { return pressed_; }
	float GetCounter() { return counter_; }
	float GetVelocityFromComp();
	bool GetBoolStartText() {return is_stop_display_start_text;}
	bool GetBoolCountdown() {return is_start_countdown;}
	float GetCountdown() { return starting_counter; }
	float GetLapTimer() { return lap_counter; }
	bool GetBoolEnd() { return is_end; }
	bool GetBoolBeginLap() { return is_begin_lap; }
	const bool GetIsPaused() { return is_paused; }
	const bool GetIsRestartLevel() {return is_restart_level;}
	bool GetIsUnorthadox() { return is_unorthadox_start; }
	//pause
	void OnPause();
	void UnPause();

	void SpeedTest(const float& dt);
protected:

	virtual void BeginPlay() override;

	//camera stuff
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* InternalCamera;
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* InternalCameraBase;
	FVector InternalCameraOrigin;


	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* ParticleSystemRightWheel;
	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* ParticleSystemLeftWheel;

	TArray<UParticleSystemComponent*> particle_arr;
	UAudioComponent* EngineComp;
	//array of cameras used for swapping
	TArray<UCameraComponent*> cameras;
	//defaults these control camera stuff
	float pitch_;
	float yaw_;
	float rot_speed_;
	float left_, right_, vertical_;
	float max_camera_rot;
	bool reverse_p;//used for keeping track of if camera is in resever or not
	//used for resetting position
	int index_recorder;
	FVector last_point;
	bool pressed_;//is restart position key(triangle) pressed
	float counter_;//counter for above
	//used for start
	bool is_starting_;//used for checking if car is on the line
	bool is_stop;//used for checking if car is at a stop
	float KPH_over;//speed
	int KPH_int_;//speed
	bool is_start_countdown;//used for starting countdown(when handbreak is pressed)
	float starting_counter;//starting counter for beging
	bool is_begin_lap;//used for resetting input.
	bool is_stop_display_start_text;
	//used for timer lap
	float lap_counter;
	bool is_end;//for detecting if user completed lap
	//used for pause
	bool is_paused;//is game paused
	bool is_restart_level;//is game restarting
	//used for brake system
	bool is_car_stationary;//is stationary
	bool is_in_reverse;//reversing
	int32 current_gear;//gives current info
	float current_RPM;
	float current_KPH;
	bool is_unorthadox_start;//used when user drives off before start

	//testing 
	UPrimitiveComponent* UpdatedPrimitive;
	float speed_timer;
};

