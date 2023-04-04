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


	//reverse cam
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 650.0f;
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagMaxDistance = 100.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = true;
	Camera->FieldOfView = 90.f;
	
	//inside
	InternalCameraOrigin = FVector(0.0f, -40.0f, 120.0f);
	InternalCameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("InternalCameraBase"));
	InternalCameraBase->SetRelativeLocation(InternalCameraOrigin);
	InternalCameraBase->SetupAttachment(GetMesh());
	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->bUsePawnControlRotation = true;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetupAttachment(InternalCameraBase);

	cameras.Add(Camera);
	cameras.Add(InternalCamera);

	left_=0.0f, right_=0.0f, vertical_=0.0f;
	yaw_ = 0.0f;
	pitch_ = 0.0f;
	pressed_ = false;
	counter_ = 0.0f;
	rot_speed_ = 100.0f;
	Camera->Activate();
	InternalCamera->Deactivate();
	
}


void AVehicleController::BeginPlay() {
	Super::BeginPlay();
	Camera->Activate();
	InternalCamera->Deactivate();

}

void AVehicleController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//inside camera
	if (InternalCamera->IsActive()){
		InternalCamera->bUsePawnControlRotation = false;
		auto r = InternalCamera->GetRelativeRotation();
		RotatarFinder(left_, right_, r.Yaw, DeltaTime, rot_speed_);
		RotatarFinder(vertical_, vertical_, r.Pitch, DeltaTime, rot_speed_);
		r.Pitch += pitch_;
		r.Yaw += yaw_;
		InternalCamera->SetRelativeRotation(r);

		UE_LOG(LogTemp, Warning, TEXT("pitch: %f"),r.Pitch);
		UE_LOG(LogTemp, Warning, TEXT("yaw: %f"), r.Yaw);
	}
	
	if (pressed_) {
		counter_ += DeltaTime;
		if (counter_ >= 1.5f) {
			pressed_ = false;
			/*float angle = atan2(track_points[index_recorder + 1].Y - last_point.Y, track_points[index_recorder + 1].X - last_point.X) * 180.0f / PI;
			last_point *= scaling_down_;
			last_point *= s_;
			player_pawn->TeleportTo(last_point, FRotator(0.0f, angle, 0.0f));*/
		}
	}

}

void AVehicleController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed,this,&AVehicleController::Handbrake);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AVehicleController::HandbrakeRelease);
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &AVehicleController::SwitchCamera);
	PlayerInputComponent->BindAction("Restart", IE_Pressed, this, &AVehicleController::RestartPosition);
	PlayerInputComponent->BindAction("Restart", IE_Released, this, &AVehicleController::Release);


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
	
	if (InternalCamera->IsActive()){
		vertical_ = AxisValue;
	}
	else{
		if (AxisValue!=0.f){
			AddControllerPitchInput(-AxisValue);
		}
	}
}
void AVehicleController::LookLeft(float AxisValue){
	if (InternalCamera->IsActive()){
		left_ = AxisValue;
		left_ *= -1.0f;
	}
	else{
		if (AxisValue != 0.f){
			AddControllerYawInput(-AxisValue);
		}
	}
}
void AVehicleController::LookRight(float AxisValue){
	if (InternalCamera->IsActive()) {
		right_ = AxisValue;
	}
	else {
		if (AxisValue != 0.f) {
			AddControllerYawInput(AxisValue);
		}
	}
}

void AVehicleController::SwitchCamera() {
	ActiveCamera();
	cameras[0]->Activate();
	cameras[1]->Deactivate();
}

void AVehicleController::ActiveCamera() {
	cameras.Swap(0, 1);
}

void AVehicleController::AngleCap(float& angle_) {
	if (angle_ < -90.0f) {
		angle_ = -90.0f;
	}
	else if (angle_ > 90.0f) {
		angle_ = 90.0f;
	}
}

void AVehicleController::RotatarFinder(const float& d_one, const float& d_two, float& angle_, const float& d_t, const float& rot_speed){
	if (d_one == -1.0f) {
		angle_ -= rot_speed * d_t;
		
	}
	else if (d_two == 1.0f) {
		angle_ += rot_speed * d_t;
	}
	else {
		if (angle_ < -(1.0f)) {
			angle_ += rot_speed * d_t;
		}
		else if (angle_ > (1.0f)) {
			angle_ -= rot_speed * d_t;
		}
		else {
			angle_ = 0.0f;
		}
	}
	AngleCap(angle_);
	

	UE_LOG(LogTemp, Warning, TEXT("angle : %f"), angle_);
	
}

void AVehicleController::RestartPosition() {
	pressed_ = true;
}

void AVehicleController::Release() {
	pressed_ = false;
	counter_ = 0.0f;
}
