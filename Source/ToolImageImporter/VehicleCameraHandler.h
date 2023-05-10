#pragma once
class TOOLIMAGEIMPORTER_API VehicleCameraHandler
{
public:
	VehicleCameraHandler();
	~VehicleCameraHandler();

protected:
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


	//array of cameras used for swapping
	TArray<UCameraComponent*> cameras;

	//defaults these control camera stuff
	float pitch_;
	float yaw_;
	float rot_speed_;
	float left_, right_, vertical_;
	float max_camera_rot;
	bool reverse_p;//used for keeping track of if camera is in resever or not
};
