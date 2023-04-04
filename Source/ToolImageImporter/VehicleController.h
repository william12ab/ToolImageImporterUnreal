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

	void RotatarFinder(const float& d_one, const float& d_two, float& angle_, const float& d_t, const float& rot_speed);
	void AngleCap(float& angle_);

	//action maps
	void Handbrake();
	void HandbrakeRelease();
	void Restart();
	void SwitchCamera();
	void ActiveCamera();
	void RestartPosition();
	void Release();
	//axis maps
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookLeft(float AxisValue);
	void LookRight(float AxisValue);


	//FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	///** Returns Camera subobject **/
	//FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	///** Returns InternalCamera subobject **/
	//FORCEINLINE UCameraComponent* GetInternalCamera() const { return InternalCamera; }
protected:

	virtual void BeginPlay() override;


	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* InternalCamera;
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* InternalCameraBase;
	FVector InternalCameraOrigin;

	bool camera_;
	TArray<UCameraComponent*> cameras;

	float pitch_;
	float yaw_;
	float rot_speed_;
	float left_, right_, vertical_;

	int index_recorder;
	FVector last_point;
	bool pressed_;
	float counter_;
};

