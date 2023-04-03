#include "VehicleController.h"
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
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.65f;

	//gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPCamera"));
	Camera->FieldOfView = 90.0f;

}

void AVehicleController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);


}



void AVehicleController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Handbrake", IE_Pressed,this,&AVehicleController::Handbrake);
	InputComponent->BindAction("Restart", IE_Pressed, this, &AVehicleController::Restart);

	InputComponent->BindAxis("MoveForward", this, &AVehicleController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AVehicleController::MoveRight);
	InputComponent->BindAxis("LookUp", this, &AVehicleController::LookUp);
	InputComponent->BindAxis("LookLeft", this, &AVehicleController::LookLeft);
	InputComponent->BindAxis("LookRight", this, &AVehicleController::LookRight);
}

void AVehicleController::Handbrake(){
	UE_LOG(LogTemp, Warning, TEXT("hb"));
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}
void AVehicleController::HandbrakeRelease() {
	UE_LOG(LogTemp, Warning, TEXT("hb r"));
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}
void AVehicleController::Restart(){
	UE_LOG(LogTemp, Warning, TEXT("r"));
}
void AVehicleController::MoveForward(float AxisValue){
	UE_LOG(LogTemp, Warning, TEXT("mf"));
	GetVehicleMovementComponent()->SetThrottleInput(AxisValue);
}
void AVehicleController::MoveRight(float AxisValue){
	UE_LOG(LogTemp, Warning, TEXT("mr/l"));
}
void AVehicleController::LookUp(float AxisValue){
	UE_LOG(LogTemp, Warning, TEXT("lu/d"));
}
void AVehicleController::LookLeft(float AxisValue){
	UE_LOG(LogTemp, Warning, TEXT("ll"));
}
void AVehicleController::LookRight(float AxisValue){
	UE_LOG(LogTemp, Warning, TEXT("lr"));
}