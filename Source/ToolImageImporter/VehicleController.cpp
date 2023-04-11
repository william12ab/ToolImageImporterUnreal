
#include "VehicleController.h"
#include "VehicleFrontWheel.h"
#include "VehicleReerWheel.h"
#include "CarHUD.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Controller.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

#define LOCTEXT_NAMESPACE "VehiclePawn"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
AVehicleController::AVehicleController(){
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("SkeletalMesh'/Game/Car/lowerwheels/Subaru_Impreza_22B_STi__1_.Subaru_Impreza_22B_STi__1_'"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Car/lowerwheels/Anim"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);


	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	Vehicle4W->WheelSetups[0].WheelClass = UVehicleFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("FL");//Wheel_Front_Left
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	Vehicle4W->WheelSetups[1].WheelClass = UVehicleFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("FR");//Wheel_Front_Right
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	Vehicle4W->WheelSetups[2].WheelClass = UVehicleReerWheel::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("RR");//Wheel_Rear_Left
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, 0.f, 0.f);

	Vehicle4W->WheelSetups[3].WheelClass = UVehicleReerWheel::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("RL");//Wheel_Rear_Right
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 0.f, 0.f);
	
	

	//tire loading
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;
	
	//torque
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->MaxEngineRPM = 6000.f;

	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1000.0f, 500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(3590.0f, 500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(6000.0f, 400.0f);

	//Streering
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.90f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.70f);

	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.65f;

	//gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;
	

	Vehicle4W->TransmissionSetup.ForwardGears.SetNum(5);
	Vehicle4W->TransmissionSetup.ForwardGears[0].Ratio = 3.083f;
	Vehicle4W->TransmissionSetup.ForwardGears[1].Ratio = 2.062f;
	Vehicle4W->TransmissionSetup.ForwardGears[2].Ratio = 1.545f;
	Vehicle4W->TransmissionSetup.ForwardGears[3].Ratio = 1.151f;
	Vehicle4W->TransmissionSetup.ForwardGears[4].Ratio = 0.825f;
	
	Vehicle4W->InertiaTensorScale = FVector(1.0f,5.0f,1.0f);
	//UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(Vehicle4W->UpdatedComponent);
	//if (UpdatedPrimitive){
	//	//UpdatedPrimitive->BodyInstance.COMNudge = FVector(8.0f, 0.0f, 0.0f);
	//	UpdatedPrimitive->BodyInstance.UpdateMassProperties();
	//}

	//reverse cam
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 650.0f;
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagMaxDistance = 100.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->CameraRotationLagSpeed = 3.7f;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = true;
	Camera->FieldOfView = 90.f;
	
	//inside
	InternalCameraOrigin = FVector(-45.0f, 35.0f, 120.0f);
	InternalCameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("InternalCameraBase"));
	InternalCameraBase->SetRelativeLocation(InternalCameraOrigin);
	InternalCameraBase->SetupAttachment(GetMesh());
	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->bUsePawnControlRotation = false;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetupAttachment(InternalCameraBase);

	cameras.Add(Camera);
	cameras.Add(InternalCamera);

	//defaults
	left_=0.0f, right_=0.0f, vertical_=0.0f;
	yaw_ = 0.0f;
	pitch_ = 0.0f;
	pressed_ = false;
	counter_ = 0.0f;
	rot_speed_ = 100.0f;
	Camera->Activate();
	InternalCamera->Deactivate();
	reverse_p = false;
	max_camera_rot = 80.f;

	//hud
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);

	// Colors for the in-car gear display. One for normal one for reverse
	GearDisplayReverseColor = FColor(255, 0, 0, 255);
	GearDisplayColor = FColor(255, 255, 255, 255);
	bInReverseGear = false;
	static ConstructorHelpers::FObjectFinder<UMaterial> TextMaterial(TEXT("Material'/Engine/EngineMaterials/AntiAliasedTextMaterialTranslucent.AntiAliasedTextMaterialTranslucent'"));
	UMaterialInterface* Material = TextMaterial.Object;

	ParticleSystemRightWheel = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particlesright"));
	ParticleSystemLeftWheel = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particlesleft"));

	//particle system work
	particle_arr.Add(ParticleSystemRightWheel);
	particle_arr.Add(ParticleSystemLeftWheel);
	FVector local_loc =FVector(-170.f,100.f,0.f);
	for (int i = 0; i < 2; i++){
		particle_arr[i]->AttachTo(GetMesh());
		particle_arr[i]->bAutoActivate = false;
		particle_arr[i]->SetRelativeLocation(local_loc);
		local_loc.Y *= -1;
	}
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/Effects/Effects/VehicleEffects/P_dirt_wheel_kickup.P_dirt_wheel_kickup'"));
	if (ParticleAsset.Succeeded()){
		particle_arr[0]->SetTemplate(ParticleAsset.Object);
		particle_arr[1]->SetTemplate(ParticleAsset.Object);
	}

}
void AVehicleController::BeginPlay() {
	Super::BeginPlay();
	Camera->Activate();
	InternalCamera->Deactivate();

}

void AVehicleController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;

	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	if (KPH>2.f){
		if (ParticleSystemRightWheel != nullptr) {
			ParticleSystemRightWheel->SetActive(true);
			ParticleSystemLeftWheel->SetActive(true);
		}
	}
	else{
		ParticleSystemRightWheel->SetActive(false);
		ParticleSystemLeftWheel->SetActive(false);
	}

	UpdateHUDStrings();
	//inside camera
	if (InternalCamera->IsActive()){
		InternalCamera->bUsePawnControlRotation = false;
		if (!reverse_p) {
			auto r = InternalCamera->GetRelativeRotation();
			RotatarFinder(left_, right_, r.Yaw, DeltaTime, rot_speed_);
			RotatarFinder(vertical_, vertical_, r.Pitch, DeltaTime, rot_speed_);
			r.Pitch += pitch_;
			r.Yaw += yaw_;
			InternalCamera->SetRelativeRotation(r);
		}
	}
	else{
		SpringArm->bUsePawnControlRotation = false;
		Camera->bUsePawnControlRotation = false;
		if (!reverse_p) {
			auto r = SpringArm->GetRelativeRotation();
			RotatarFinder(left_, right_, r.Yaw, DeltaTime, rot_speed_);
			RotatarFinder(vertical_, vertical_, r.Pitch, DeltaTime, rot_speed_);
			r.Pitch += pitch_;
			r.Yaw += yaw_;
			SpringArm->SetRelativeRotation(r);
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
	PlayerInputComponent->BindAction("CameraReverse", IE_Pressed, this, &AVehicleController::CameraReverseView);
	PlayerInputComponent->BindAction("CameraReverse", IE_Released, this, &AVehicleController::CameraReverseViewRelease);
	PlayerInputComponent->BindAction("GearUp", IE_Pressed, this, &AVehicleController::GearUp);
	PlayerInputComponent->BindAction("GearDown", IE_Pressed, this, &AVehicleController::GearDown);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVehicleController::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVehicleController::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AVehicleController::LookUp);
	PlayerInputComponent->BindAxis("LookLeft", this, &AVehicleController::LookLeft);
	PlayerInputComponent->BindAxis("LookRight", this, &AVehicleController::LookRight);
}
void AVehicleController::CameraReverseViewRelease() {
	if (InternalCamera->IsActive()){
		auto r = InternalCamera->GetRelativeRotation();
		r.Yaw += 180.f;
		InternalCamera->SetRelativeRotation(r);
	}
	else{
		auto r = SpringArm->GetRelativeRotation();
		r.Yaw += 180.f;
		SpringArm->SetRelativeRotation(r);
	}
	reverse_p = false;
}
void AVehicleController::CameraReverseView() {
	if (InternalCamera->IsActive()) {
		auto r = InternalCamera->GetRelativeRotation();
		r.Yaw -= 180.f;
		InternalCamera->SetRelativeRotation(r);
	}
	else{
		auto r = SpringArm->GetRelativeRotation();
		r.Yaw -= 180.f;
		SpringArm->SetRelativeRotation(r);
	}
	reverse_p = true;
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
		vertical_ = AxisValue;
}
void AVehicleController::LookLeft(float AxisValue){
		left_ = AxisValue;
		left_ *= -1.0f;
}
void AVehicleController::LookRight(float AxisValue){
		right_ = AxisValue;
}
void AVehicleController::GearUp() {
	GetVehicleMovementComponent()->SetTargetGear(GetVehicleMovementComponent()->GetCurrentGear()+1, true);
}
void AVehicleController::GearDown() {
	GetVehicleMovementComponent()->SetTargetGear(GetVehicleMovementComponent()->GetCurrentGear() - 1, true);
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
	if (angle_ < -max_camera_rot) {
		angle_ = -max_camera_rot;
	}
	else if (angle_ > max_camera_rot) {
		angle_ = max_camera_rot;
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
	AngleCap(angle_);}

void AVehicleController::RestartPosition() {
	pressed_ = true;
}

void AVehicleController::Release() {
	pressed_ = false;
	counter_ = 0.0f;
	GetVehicleMovementComponent()->SetThrottleInput(0.f);
}

void AVehicleController::UpdateHUDStrings(){
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);

	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));

	if (bInReverseGear == true){
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else{
		int32 Gear = GetVehicleMovement()->GetCurrentGear();
		GearDisplayString = (Gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(Gear);
	}
}

float AVehicleController::GetVelocityFromComp() {
	float KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f; 
	return KPH; 
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS