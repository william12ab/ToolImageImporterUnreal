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

	//action maps
	void Handbrake();
	void HandbrakeRelease();
	void Restart();

	//axis maps
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void LookLeft(float AxisValue);
	void LookRight(float AxisValue);

protected:
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	
};

