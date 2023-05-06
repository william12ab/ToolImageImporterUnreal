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
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#define LOCTEXT_NAMESPACE "VehiclePawn"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
AVehicleController::AVehicleController(){
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("SkeletalMesh'/Game/Car/correct_size/Subaru_Impreza_22B_STi__CORRECT_SIZE_.Subaru_Impreza_22B_STi__CORRECT_SIZE_'"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Car/correct_size/AMINBP"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	GetMesh()->OnComponentBeginOverlap.AddDynamic(this, &AVehicleController::OnOverlapBegin);

	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	Vehicle4W->WheelSetups[0].WheelClass = UVehicleFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("FL");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, 0.f, 0.f);
	Vehicle4W->WheelSetups[1].WheelClass = UVehicleFrontWheel::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("FR");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 0.f, 0.f);
	Vehicle4W->WheelSetups[2].WheelClass = UVehicleReerWheel::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("RR");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, 0.f, 0.f);
	Vehicle4W->WheelSetups[3].WheelClass = UVehicleReerWheel::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("RL");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 0.f, 0.f);
	Vehicle4W->DragCoefficient = 0.20f;
	Vehicle4W->Mass = 1200.f;
	//tire loading
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;
	//differential setup 
	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.5f;
	Vehicle4W->bReverseAsBrake = false;
	//torque
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.MaxRPM = 8000.f;
	//change those values at 0 rpm we have 500 torque
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 500);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(500.0f, 550);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1000.0f, 590);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(2000.0f, 600);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(2500.0f, 650);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(3000.0f, 680);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(3500.0f, 680);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(4000.0f, 600);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(4500.0f, 600);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5000.0f, 550);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5500.0f, 500);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(6000.0f, 480);
	//Streering
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f*1.2f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 1.0f*1.2f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(90.0f, 1.f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 1.f);
	//gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;
	Vehicle4W->TransmissionSetup.FinalRatio = 4.444f;
	Vehicle4W->TransmissionSetup.ForwardGears.SetNum(5);
	Vehicle4W->TransmissionSetup.ForwardGears[0].Ratio = 3.083f;
	Vehicle4W->TransmissionSetup.ForwardGears[1].Ratio = 2.062f;
	Vehicle4W->TransmissionSetup.ForwardGears[2].Ratio = 1.545f;
	Vehicle4W->TransmissionSetup.ForwardGears[3].Ratio = 1.151f;
	Vehicle4W->TransmissionSetup.ForwardGears[4].Ratio = 1.111f;
	Vehicle4W->TransmissionSetup.ForwardGears[0].DownRatio = 0.5;
	Vehicle4W->TransmissionSetup.ForwardGears[1].DownRatio = 0.5;
	Vehicle4W->TransmissionSetup.ForwardGears[3].DownRatio = 0.4;
	Vehicle4W->TransmissionSetup.ForwardGears[2].DownRatio = 0.5;
	Vehicle4W->TransmissionSetup.ForwardGears[4].DownRatio = 0.4;
	Vehicle4W->TransmissionSetup.ForwardGears[0].UpRatio= 0.95;
	Vehicle4W->TransmissionSetup.ForwardGears[1].UpRatio = 0.85;
	Vehicle4W->TransmissionSetup.ForwardGears[2].UpRatio = 0.85;
	Vehicle4W->TransmissionSetup.ForwardGears[3].UpRatio = 0.85;
	Vehicle4W->TransmissionSetup.ForwardGears[4].UpRatio = 0.85;
	//inertia - harder on the y axis, so over jumps the car is less likely to tip.
	Vehicle4W->InertiaTensorScale = FVector(1.0f,3.0f,1.0f);
	//reverse cam
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 725.0f;
	SpringArm->TargetOffset = FVector(0.f, 0.f, 225.f);
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
	FVector local_loc =FVector(-170.f,100.f,42.f);
	for (int i = 0; i < 2; i++){
		particle_arr[i]->AttachTo(GetMesh());
		particle_arr[i]->bAutoActivate = false;
		particle_arr[i]->SetRelativeScale3D(FVector(0.8f, 0.7f, .5));
		particle_arr[i]->SetRelativeLocation(local_loc);
		local_loc.Y *= -1;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/Effects/Effects/VehicleEffects/P_dirt_wheel_kickup.P_dirt_wheel_kickup'"));
	if (ParticleAsset.Succeeded()){
		particle_arr[0]->SetTemplate(ParticleAsset.Object);
		particle_arr[1]->SetTemplate(ParticleAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> EngineSoundCueObj(TEXT("SoundCue'/Game/Sound/Engine.Engine'"));
	if (EngineSoundCueObj.Succeeded()){
		EngineSoundCue = EngineSoundCueObj.Object;
		EngineComp = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSoundComponent"));
		EngineComp->SetupAttachment(RootComponent);
	}
	//starting
	is_starting_ = false;
	is_stop = false;
	is_start_countdown = false;
	starting_counter = 0.0f;
	is_begin_lap = false;
	lap_counter = 0.0f;
	is_end = false;
	is_stop_display_start_text = false;
	is_paused = false;
	//restarting
	is_restart_level = false;
	is_car_stationary = true;
	is_in_reverse = false;
}
void AVehicleController::BeginPlay() {
	Super::BeginPlay();
	Camera->Activate();
	InternalCamera->Deactivate();
	EngineComp->Activate(true);
	EngineComp->SetSound(EngineSoundCue);
	EngineComp->Play(0.f);
	if (GetVehicleMovement()->GetEngineRotationSpeed() < 600) {
		EngineComp->SetFloatParameter(FName("RPM"), 600);
	}
	else {
		EngineComp->SetFloatParameter(FName("RPM"), GetVehicleMovement()->GetEngineRotationSpeed());
	}
}

void AVehicleController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	ChangeBrakeSystem();
	bInReverseGear = GetVehicleMovement()->GetCurrentGear() < 0;
	if (GetVehicleMovement()->GetEngineRotationSpeed() < 600){
		EngineComp->SetFloatParameter(FName("RPM"), 600);
	}
	else{
		EngineComp->SetFloatParameter(FName("RPM"), GetVehicleMovement()->GetEngineRotationSpeed());
	}
	//for parrticels
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
	//for start
	KPH_over = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	KPH_int_ = FMath::FloorToInt(KPH_over);
	if (is_starting_) {
		if (KPH_int_ < 5) {
			GetVehicleMovementComponent()->SetThrottleInput(0);
			is_stop = true;
			is_stop_display_start_text = true;
		}
		else {
			if (!is_stop) {
				GetVehicleMovementComponent()->SetBrakeInput(1);
			}
		}
		StartFunction(DeltaTime);
	}

	if (is_begin_lap&&!is_end){
		lap_counter += DeltaTime;
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
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AVehicleController::OnPause).bExecuteWhenPaused=true;

	PlayerInputComponent->BindAxis("MoveForward", this, &AVehicleController::MoveForward);
	PlayerInputComponent->BindAxis("Brake", this, &AVehicleController::Brake);
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
	if (is_stop){
		is_start_countdown = true;
	}
}
void AVehicleController::HandbrakeRelease() {
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}
void AVehicleController::Restart() {
	GetVehicleMovement()->DestroyPhysicsState();
	GetVehicleMovement()->RecreatePhysicsState();
	GetVehicleMovementComponent()->DestroyPhysicsState();
	GetVehicleMovementComponent()->RecreatePhysicsState();
}
void AVehicleController::RestartMainLevel() {
	Restart();
	is_starting_ = false;
	is_stop = false;
	is_start_countdown = false;
	starting_counter = 0.0f;
	is_begin_lap = false;
	lap_counter = 0.0f;
	is_end = false;
	is_stop_display_start_text = false;
	is_restart_level = true;
}
void AVehicleController::MoveForward(float AxisValue){
	if (!is_starting_){
		GetVehicleMovementComponent()->SetThrottleInput(AxisValue);
		if (AxisValue > 0.01f) {
			if (bInReverseGear){
				GetVehicleMovementComponent()->SetTargetGear(GetVehicleMovementComponent()->GetCurrentGear() + 2, true);
			}
			is_in_reverse = false;
			is_car_stationary = false;
		}
	}
}
void AVehicleController::Brake(float AxisValue) {
	if (!is_starting_){
		if (is_car_stationary){
			GetVehicleMovementComponent()->SetThrottleInput(-AxisValue);
				is_in_reverse = true;
		}
		else {
			GetVehicleMovementComponent()->SetBrakeInput(AxisValue);
		}
	}
}
void AVehicleController::ChangeBrakeSystem() {
	if ((int)GetVelocityFromComp() == 0) {
		GetVehicleMovement()->bReverseAsBrake = true;
		is_car_stationary = true;
	}
	else {
		if (!is_in_reverse) {
			GetVehicleMovement()->bReverseAsBrake = false;
			is_car_stationary = false;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("station: %s"), (is_car_stationary ? TEXT("true") : TEXT("false")));
	UE_LOG(LogTemp, Warning, TEXT("reverse: %s"), (is_in_reverse ? TEXT("true") : TEXT("false")));
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
void AVehicleController::OnPause(){
	if (is_paused){
		is_paused = false;
	}
	else {
		is_paused = true;
	}
}
void AVehicleController::UnPause() {
	is_paused = false;
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
void AVehicleController::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	is_starting_ = true;
	auto ss=OtherActor->GetName();
	if (ss== "boxendtriggername"){
		is_end = true;
	}
}
void AVehicleController::StartFunction(const float& dt) {
	if (is_start_countdown){
		starting_counter += dt;
		if (starting_counter>=5.0f){
			is_starting_ = false;
			is_begin_lap = true;
		}
	}
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS