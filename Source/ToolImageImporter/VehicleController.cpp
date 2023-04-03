#include "VehicleController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"

AVehicleController::AVehicleController()
{
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	//tire loading
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;


	//torque
	Vehicle4W->MaxEngineRPM = 5700.0f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f, 400.0f);

	//Streering
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.70f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.60f);

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.65;

	//gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->FieldOfView = 90.0f;
	
}

void AVehicleController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);


}

void AVehicleController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed,this,&AVehicleController::Handbrake);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AVehicleController::HandbrakeRelease);

	PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &AVehicleController::Restart);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVehicleController::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVehicleController::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AVehicleController::LookUp);
	PlayerInputComponent->BindAxis("LookLeft", this, &AVehicleController::LookLeft);
	PlayerInputComponent->BindAxis("LookRight", this, &AVehicleController::LookRight);
}

void AVehicleController::Handbrake(){
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}
void AVehicleController::HandbrakeRelease() {
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}
void AVehicleController::Restart(){
}
void AVehicleController::MoveForward(float AxisValue){
	GetVehicleMovementComponent()->SetThrottleInput(AxisValue);
}
void AVehicleController::MoveRight(float AxisValue){
	GetVehicleMovementComponent()->SetSteeringInput(AxisValue);
}
void AVehicleController::LookUp(float AxisValue){
	if (AxisValue!=0.f)
	{
		AddControllerPitchInput(AxisValue);
	}
}
void AVehicleController::LookLeft(float AxisValue){
	if (AxisValue != 0.f)
	{
		AddControllerYawInput(AxisValue);
	}
}
void AVehicleController::LookRight(float AxisValue){
	if (AxisValue != 0.f)
	{
		AddControllerYawInput(AxisValue);
	}
}