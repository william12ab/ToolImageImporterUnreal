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
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "ToolContextInterfaces.h"
#define LOCTEXT_NAMESPACE "VehiclePawn"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
AVehicleController::AVehicleController(){
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("SkeletalMesh'/Game/Car/with_toe/Subaru_with_toe.Subaru_with_toe'"));
	// CarMesh(TEXT("SkeletalMesh'/Game/Car/test/Subaru_Impreza_22B_STi_wheel_test.Subaru_Impreza_22B_STi_wheel_test'"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Car/with_toe/NewAnimBP"));
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
	Vehicle4W->WheelSetups[2].bDisableSteering = true;
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, 0.f, 0.f);
	Vehicle4W->WheelSetups[3].WheelClass = UVehicleReerWheel::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("RL");
	Vehicle4W->WheelSetups[2].bDisableSteering = true;
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(0.f, 0.f, 0.f);
	Vehicle4W->DragCoefficient = 0.33f;
	Vehicle4W->Mass = 1598.690;
	//tire loading
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;
	//differential setup 
	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;

	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.60f;
	Vehicle4W->bReverseAsBrake = false;
	//torque
	
	
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.MaxRPM = 8000.f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 550);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(500.0f, 580);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1000.0f, 590);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(2000.0f, 620);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(2500.0f, 650);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(3000.0f, 680);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(3500.0f, 680);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(4000.0f, 670);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(4500.0f, 660);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5000.0f, 650);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5500.0f, 640);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(6000.0f, 630);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(8500.0f, 590);
	

	//Streering
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.950f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(90.0f, 0.95f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(100.0f, 0.93f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.885f);
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
	Vehicle4W->TransmissionSetup.ForwardGears[1].UpRatio = 0.75;
	Vehicle4W->TransmissionSetup.ForwardGears[2].UpRatio = 0.75;
	Vehicle4W->TransmissionSetup.ForwardGears[3].UpRatio = 0.75;
	Vehicle4W->TransmissionSetup.ForwardGears[4].UpRatio = 0.75;
	//inertia - harder on the y axis, so over jumps the car is less likely to tip.
	Vehicle4W->InertiaTensorScale = FVector(1.20f,3.0f,1.20f);
	//com
	UpdatedPrimitive = Cast<UPrimitiveComponent>(Vehicle4W->UpdatedComponent);
	if (UpdatedPrimitive){
		UpdatedPrimitive->BodyInstance.COMNudge = FVector(35.f, -3.656f, -58.5f);//further down and along the body 
		//28.4f//(35.f, -3.656f, -22.5f)
		//75.f, -3.656f, -58.5f)
		UpdatedPrimitive->BodyInstance.UpdateMassProperties();
	}
	Vehicle4W->EngineSetup.DampingRateZeroThrottleClutchEngaged =0.5;
	Vehicle4W->EngineSetup.DampingRateZeroThrottleClutchDisengaged = 0.15;

	Vehicle4W->ChassisHeight = 22.f;
	Vehicle4W->ChassisWidth = 150.f;

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
	InternalCameraOrigin = FVector(-40.0f, 30.0f, 140.0f);
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
	GearDisplayReverseColor = FColor(208, 34, 65, 255);
	GearDisplayColor = FColor(255, 250, 239, 255);
	is_in_reverse_gear = false;
	static ConstructorHelpers::FObjectFinder<UMaterial> TextMaterial(TEXT("Material'/Engine/EngineMaterials/AntiAliasedTextMaterialTranslucent.AntiAliasedTextMaterialTranslucent'"));
	UMaterialInterface* Material = TextMaterial.Object;



	ParticleSystemRightWheel = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particlesright"));
	ParticleSystemLeftWheel = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particlesleft"));
	//particle system work
	particle_arr.Add(ParticleSystemRightWheel);
	particle_arr.Add(ParticleSystemLeftWheel);
	FVector local_loc =FVector(-170.f,80.f,53.f);
	for (int i = 0; i < 2; i++){
		particle_arr[i]->AttachTo(GetMesh());
		particle_arr[i]->bAutoActivate = false;
		particle_arr[i]->SetRelativeScale3D(FVector(0.8f, 0.7f, .5));
		particle_arr[i]->SetWorldLocation(local_loc);
		local_loc.Y *= -1;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("ParticleSystem'/Game/Effects/Effects/VehicleEffects/P_dirt_wheel_kickup.P_dirt_wheel_kickup'"));
	if (ParticleAsset.Succeeded()){
		particle_arr[0]->SetTemplate(ParticleAsset.Object);
		particle_arr[1]->SetTemplate(ParticleAsset.Object);
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
	current_gear = 0;
	current_RPM = 0.f;
	current_KPH = 0.0f;
	speed_timer = 0.0f;
	is_unorthadox_start = false;
	is_countdown_set = false;
	is_breaking = false;
	is_lights_on = false;
	gear_marker = 1;

	sound_ref_veh = Cast<AMainSounds>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainSounds::StaticClass()));
	is_render_timer = true;
	is_render_spedo = true;
	penalty_time = 0.0f;
	cp_counter = 0;

	//spheres
	sphere_left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereLeft"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	sphere_left->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	sphere_left->SetStaticMesh(SphereMeshAsset.Object);
	sphere_left->AttachTo(GetMesh());
	sphere_left->SetWorldScale3D(FVector(0.05f, 0.05f, 0.05f));
	

	sphere_right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereRight"));
	sphere_right->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	sphere_right->SetStaticMesh(SphereMeshAsset.Object);
	sphere_right->AttachTo(GetMesh());
	sphere_right->SetWorldScale3D(FVector(0.05f, 0.05f, 0.05f));
	GetMesh()->bReceivesDecals = true;


	//lights
	material_red_light_off = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/AutomotiveMaterials/Materials/Exterior/Lights/MI_Off_Red.MI_Off_Red'"));
	material_red_light_on = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/AutomotiveMaterials/Materials/Exterior/Lights/MI_Lamp_Red.MI_Lamp_Red'"));
	material_white_light_off = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/AutomotiveMaterials/Materials/Exterior/Lights/MI_Blue_off.MI_Blue_off'"));
	material_white_light_on = LoadObject<UMaterialInterface>(NULL, TEXT("MaterialInstanceConstant'/Game/AutomotiveMaterials/Materials/Exterior/Lights/MI_Lamp_Blue.MI_Lamp_Blue'"));

}
void AVehicleController::BeginPlay() {
	Super::BeginPlay();
	Camera->Activate();
	InternalCamera->Deactivate();

	SetDecals();
	//sound set in mainsounds beginplay
}

void AVehicleController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//current values
	GetWheelAngle();
	if (GetVehicleMovement()->GetCurrentGear()>gear_marker) {
		//play pop sound
		if (sound_ref_veh != nullptr) {
			sound_ref_veh->PlayPopSound(GetActorLocation());
		}
	}
	if (current_gear != 0) {
		gear_marker = current_gear;
	}
	current_RPM=GetVehicleMovement()->GetEngineRotationSpeed();
	current_gear = GetVehicleMovement()->GetCurrentGear();
	current_KPH = FMath::Abs(GetVehicleMovement()->GetForwardSpeed()) * 0.036f;
	is_in_reverse_gear = current_gear < 0;
	//

	

	if (current_RPM>600&&current_KPH<3){
		GetVehicleMovementComponent()->SetTargetGear(1,true);
	}

	//SpeedTest(DeltaTime);
	ChangeBrakeSystem();
	//sound
	if (sound_ref_veh !=nullptr){
		sound_ref_veh->SetEngineParam(current_RPM, current_KPH);
	}
	//for parrticels
	if (current_KPH >2.f){
		if (ParticleSystemRightWheel != nullptr) {
			ParticleSystemRightWheel->SetActive(true);
			ParticleSystemLeftWheel->SetActive(true);
		}
	}
	else{
		ParticleSystemRightWheel->SetActive(false);
		ParticleSystemLeftWheel->SetActive(false);
	}
	KPH_int_ = FMath::FloorToInt(current_KPH);
	if (is_starting_) {
		if (KPH_int_ < 5) {
			is_stop = true;
			is_stop_display_start_text = true;
			if (!is_start_countdown){
				GetVehicleMovementComponent()->SetThrottleInput(0);
			}
		}
		else {
			if (!is_stop) {
				GetVehicleMovementComponent()->SetBrakeInput(1);
			}
		}
		StartFunction(DeltaTime);
		if (!is_begin_lap){
			CheckForStart();
		}
	}

	if (is_begin_lap&&!is_end){
		lap_counter += DeltaTime;
	}

	if (is_end){
		sound_ref_veh->StopCountdown();
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
	UpdateDriver(DeltaTime);
	pace3 = GetActorLocation();
	
	if (is_lights_on){
		GetMesh()->SetMaterial(6, material_white_light_on);
		is_breaking = true;
	}
	else {
		GetMesh()->SetMaterial(6, material_white_light_off);
	}
	if (is_breaking){
		GetMesh()->SetMaterial(5, material_red_light_on);
	}
	else {
		GetMesh()->SetMaterial(5, material_red_light_off);
	}
	is_breaking = false;

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
	PlayerInputComponent->BindAction("Lights", IE_Pressed, this, &AVehicleController::TurnOnLights);
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
		GetVehicleMovementComponent()->SetThrottleInput(AxisValue);
		if (AxisValue > 0.01f) {
			if (is_in_reverse_gear){
				GetVehicleMovementComponent()->SetTargetGear(current_gear + 2, true);
			}
			is_in_reverse = false;
			is_car_stationary = false;
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
		if (AxisValue>0.1f){
			is_breaking = true;
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
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
		FVector2D size_;
		GEngine->GameViewport->GetViewportSize(size_);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetMouseLocation(size_.X / 2, size_.Y / 2);
	}
}
void AVehicleController::UnPause() {
	is_paused = false;
}
void AVehicleController::GearUp() {
	GetVehicleMovementComponent()->SetTargetGear(current_gear+1, true);
	if (sound_ref_veh!=nullptr){
		sound_ref_veh->PlayPopSound(GetActorLocation());
	}
}
void AVehicleController::GearDown() {
	GetVehicleMovementComponent()->SetTargetGear(current_gear - 1, true);
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
void AVehicleController::TurnOnLights() {
	if (is_lights_on){
		is_lights_on = false;
		left_light->TurnOn(false);
		right_light->TurnOn(false);
	}
	else {
		is_lights_on = true;
		left_light->TurnOn(true);
		right_light->TurnOn(true);
	}
	
}
void AVehicleController::GetWheelAngle() {
	auto a1=GetVehicleMovementComponent()->Wheels[0]->GetSteerAngle();
	auto a2=GetVehicleMovementComponent()->Wheels[1]->GetSteerAngle();

	auto abs1 = FMath::Abs(a1);
	auto abs2 = FMath::Abs(a2);

	if (abs1>abs2){
		steering_angle = a1;
	}
	else {
		steering_angle = a2;
	}

	//gets location of wheel
	FName wheel_bone = "bone_wheel";
	auto s = GetMesh()->GetBoneQuaternion(wheel_bone, EBoneSpaces::ComponentSpace);
	auto p=s.Rotator();
	FVector BoneLocation = GetMesh()->GetBoneLocation(wheel_bone, EBoneSpaces::ComponentSpace);
	FVector centre_point = GetMesh()->GetBoneLocation(wheel_bone, EBoneSpaces::ComponentSpace);

	//sets spheres to points on steering wheel for hands to go on 
	FVector radius_ = FVector(6, 15, 13);
	float hands_angle = (-3 * steering_angle);
	FVector RotateValue = radius_.RotateAngleAxis(hands_angle, FVector(1, 0, 0));
	centre_point.X += RotateValue.X;
	centre_point.Y += RotateValue.Y;
	centre_point.Z += RotateValue.Z;
	sphere_right->SetRelativeLocation(centre_point);
	//angle for arm rot right along horizontaol axis
	float x = centre_point.X - (centre_point.X - 36.f);
	float y = centre_point.Y - (46.f);
	float z = centre_point.Z - (117.f);
	yaw_angle_r = FMath::Atan2(y, x);
	yaw_angle_r *= (180.f / 3.141592f);
	yaw_angle_r *= -1;

	roll_angle_r = FMath::Atan2(z, x);
	roll_angle_r *= (180.f / 3.141592f);
	roll_angle_r *= -1;
	centre_point = GetMesh()->GetBoneLocation(wheel_bone, EBoneSpaces::ComponentSpace);
	radius_ = FVector(6, -15, 13);
	hands_angle = (-3 * steering_angle);
	RotateValue = radius_.RotateAngleAxis(hands_angle, FVector(1, 0, 0));
	centre_point.X += RotateValue.X;
	centre_point.Y += RotateValue.Y;
	centre_point.Z += RotateValue.Z;
	right_loc = centre_point;
	sphere_left->SetRelativeLocation(centre_point);

	//angle for arm rot left along horizontal axis
	x = centre_point.X -(centre_point.X - 36.f);
	y = centre_point.Y - (16.f);
	z = centre_point.Z - (117.f);
	yaw_angle = FMath::Atan2(y, x);
	yaw_angle *= (180.f / 3.141592f);

	roll_angle_l = FMath::Atan2(z, x);
	roll_angle_l *= (180.f / 3.141592f);
	roll_angle_l *= -1;
	left_loc = sphere_left->GetComponentLocation();
	sphere_left->SetVisibility(false);
	sphere_right->SetVisibility(false);
}

void AVehicleController::UpdateDriver(const float& dt) {
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
	penalty_time += 5.0f;
	
}

void AVehicleController::UpdateHUDStrings(){
	int32 KPH_int = FMath::FloorToInt(current_KPH);
	int32 RPM_int = FMath::FloorToInt(current_RPM);
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} "), FText::AsNumber(KPH_int));
	RPMDisplayString = FText::Format(LOCTEXT("RPMFormat", "{0} "), FText::AsNumber(RPM_int));
	if (is_in_reverse_gear == true){
		GearDisplayString = FText(LOCTEXT("ReverseGear", "R"));
	}
	else{
		GearDisplayString = (current_gear == 0) ? LOCTEXT("N", "N") : FText::AsNumber(current_gear);
	}
}
float AVehicleController::GetVelocityFromComp() {
	return current_KPH;
}
void AVehicleController::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	is_starting_ = true;
	auto ss=OtherActor->GetName();
	if (ss== "boxendtriggername"){
		is_end = true;
	}
	auto sss = OtherActor->GetName();//is the vehicle
	if (sss.Contains("ControlPointTriggerBox")){
		pace1 = OtherActor->GetActorLocation();
		if (box_names.Num() == 0) {
			if (sss == "ControlPointTriggerBox_0") {
			}
			else {
				box_names.Add(sss);
				cp_counter++;
			}
		}
		else {
			if (sss == "ControlPointTriggerBox_0") {
			}
			else {
				bool is_dont_add = false;
				for (size_t i = 0; i < box_names.Num(); i++) {
					if (sss == box_names[i]) {
						is_dont_add = true;
					}
				}
				if (!is_dont_add) {
					box_names.Add(sss);
					cp_counter++;
				}
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("cp_counter: %d"), cp_counter);
	}
}
void AVehicleController::StartFunction(const float& dt) {
	if (is_start_countdown){
		starting_counter += dt;
		if (starting_counter>=3.0f){
			if (!is_countdown_set) {
				sound_ref_veh->PlayCountdown();
				is_countdown_set = true;
			}
		}
		if (starting_counter>=5.0f){
			is_starting_ = false;
			is_begin_lap = true;

		}
	}
}

void AVehicleController::SpeedTest(const float& dt) {
	if (current_KPH> 0.1f && current_KPH < 100.0f) {
		speed_timer += dt;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("time: %f"), speed_timer);
	}
}

void AVehicleController::CheckForStart() {
	if (is_start_countdown) {
		if (current_KPH > 10.f) {
			sound_ref_veh->StopCountdown();
			is_unorthadox_start = true;
			is_starting_ = false;
			is_begin_lap = true;
			//start timer
			//remove UI
		}
	}
}

void AVehicleController::CreateDecal(FVector l, FVector s, FRotator r, FString st, ALiveryActor* actor_) {
	FName name_sock = "Root";
	actor_->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), name_sock);
	actor_->Init(l, s, r, st);
}

void AVehicleController::SetDecals() {
	//loc,scale,rot,name
	FVector loc_, scale_;
	FRotator rot_;
	FString name_;
	FActorSpawnParameters SpawnInfo;
	FRotator myRot(0, 0, 0);
	FVector myLoc = FVector(0, 0, 0);
	ALiveryActor* left_logo;
	ALiveryActor* right_logo;
	ALiveryActor* back_logo;
	ALiveryActor* front_logo;
	ALiveryActor* roof_logo;
	ALiveryActor* left_num;
	ALiveryActor* right_num;
	ALiveryActor* roof_num;
	ALiveryActor* right_pirelli;
	ALiveryActor* left_pirelli;
	ALiveryActor* left_sti;
	ALiveryActor* right_sti;
	left_logo = GetWorld()->SpawnActor<ALiveryActor>(myLoc, myRot, SpawnInfo);
	right_logo = GetWorld()->SpawnActor<ALiveryActor>(myLoc, myRot, SpawnInfo);
	back_logo = GetWorld()->SpawnActor<ALiveryActor>(myLoc, myRot, SpawnInfo);
	front_logo = GetWorld()->SpawnActor<ALiveryActor>(myLoc, myRot, SpawnInfo);
	roof_logo = GetWorld()->SpawnActor<ALiveryActor>(myLoc, myRot, SpawnInfo);
	left_num = GetWorld()->SpawnActor<ALiveryActor>(myLoc, myRot, SpawnInfo);
	right_num = GetWorld()->SpawnActor<ALiveryActor>(myLoc, myRot, SpawnInfo);
	roof_num = GetWorld()->SpawnActor<ALiveryActor>(myLoc, myRot, SpawnInfo);
	right_pirelli = GetWorld()->SpawnActor<ALiveryActor>(myLoc, myRot, SpawnInfo);
	left_pirelli = GetWorld()->SpawnActor<ALiveryActor>(myLoc, myRot, SpawnInfo);
	left_sti = GetWorld()->SpawnActor<ALiveryActor>(myLoc, myRot, SpawnInfo);
	right_sti = GetWorld()->SpawnActor<ALiveryActor>(myLoc, myRot, SpawnInfo);

	loc_ = FVector(-13.7f, 97.4f, -27.8f);
	rot_ = FRotator(0, -90.f, 90.f);//pitch,yaw, roll
	scale_ = FVector(0.0575f, 0.1f, 0.2075f);
	name_ = "badgemat.badgemat";
	CreateDecal(loc_, scale_, rot_, name_,left_logo);
	loc_ = FVector(-23.97f, -102.4f, -25.8f);
	rot_ = FRotator(0, 90.f, 90.f);//pitch,yaw, roll
	CreateDecal(loc_, scale_, rot_, name_, right_logo);
	loc_ = FVector(-243.69f, -2.24f, -8.2f);
	rot_ = FRotator(0, 0.f, 90.f);//pitch,yaw, roll
	scale_ = FVector(0.1f, 0.05f, 0.15f);
	name_ = "logomat.logomat";
	CreateDecal(loc_, scale_, rot_, name_, back_logo);
	loc_ = FVector(236.30f, -2.4f, -8.3f);
	rot_ = FRotator(-90, -90.f, 0.f);//pitch,yaw, roll
	CreateDecal(loc_, scale_, rot_, name_, front_logo);
	loc_ = FVector(13.930f, 2.4f, 71.3f);
	rot_ = FRotator(-90, 0, -90.f);//pitch,yaw, roll
	scale_ = FVector(0.1f, 0.0625f, 0.25f);
	name_ = "logodark.logodark";
	CreateDecal(loc_, scale_, rot_, name_, roof_logo);

	loc_ = FVector(66, 97.4f, -28.3f);
	rot_ = FRotator(0, -90, 90.f);//pitch,yaw, roll
	scale_ = FVector(0.1, 0.1f, 0.1);
	name_ = "number_decal.number_decal";
	CreateDecal(loc_, scale_, rot_, name_, left_num);
	loc_ = FVector(56, -102.4f, -26.3f);
	rot_ = FRotator(0, 90, 90.f);//pitch,yaw, roll
	CreateDecal(loc_, scale_, rot_, name_, right_num);
	loc_ = FVector(-43, -1.8f, 82.4f);
	rot_ = FRotator(-90,0, 90.f);//pitch,yaw, roll
	CreateDecal(loc_, scale_, rot_, name_, roof_num);


	loc_ = FVector(-254, -62.28f, -28.4f);
	rot_ = FRotator(0, 0, 90.f);//pitch,yaw, roll
	scale_ = FVector(0.1, 0.015625f, 0.0625f);
	name_ = "pirellilogo.pirellilogo";
	CreateDecal(loc_, scale_, rot_, name_, left_pirelli);
	loc_ = FVector(-254, 47.28f, -28.4f);
	rot_ = FRotator(0, 0, 90.f);//pitch,yaw, roll
	CreateDecal(loc_, scale_, rot_, name_, right_pirelli);

	loc_ = FVector(245, -62.28f, -54.27f);
	rot_ = FRotator(0, 180, 90.f);//pitch,yaw, roll
	scale_ = FVector(0.1, 0.01f, 0.035f);
	name_ = "stilogo.stilogo";
	CreateDecal(loc_, scale_, rot_, name_, left_sti);
	loc_ = FVector(255, 62.28f, -54.27f);
	rot_ = FRotator(0, 180, 90.f);//pitch,yaw, roll
	CreateDecal(loc_, scale_, rot_, name_, right_sti);




	left_light = GetWorld()->SpawnActor<AHeaedLight>(myLoc, myRot, SpawnInfo);
	right_light = GetWorld()->SpawnActor<AHeaedLight>(myLoc, myRot, SpawnInfo);
	FName name_sock = "Root";
	left_light->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), name_sock);
	right_light->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), name_sock);
	left_light->SetActorRelativeLocation(FVector(237.f,-63.f,-21.4f));
	right_light->SetActorRelativeLocation(FVector(237.5f,56.6f,-21.4f));
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS