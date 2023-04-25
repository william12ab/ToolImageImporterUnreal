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


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* EngineSoundCue;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText SpeedDisplayString;
	/** The current gear as a string (R,N, 1,2 etc) */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText GearDisplayString;
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		/** The color of the incar gear text in forward gears */
		FColor	GearDisplayColor;
	/** The color of the incar gear text when in reverse */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FColor	GearDisplayReverseColor;
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
		bool bInReverseGear;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void RotatarFinder(const float& d_one, const float& d_two, float& angle_, const float& d_t, const float& rot_speed);
	void AngleCap(float& angle_);

	void UpdateHUDStrings();

	//action maps
	void Handbrake();
	void HandbrakeRelease();
	void Restart();
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
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookLeft(float AxisValue);
	void LookRight(float AxisValue);

	void SetPressed(bool b_) { pressed_ = b_; }
	void SetCounter(float c_) { counter_ = c_; }

	bool GetPressed() { return pressed_; }
	float GetCounter() { return counter_; }
	float GetVelocityFromComp();
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
	bool pressed_;
	float counter_;
};

