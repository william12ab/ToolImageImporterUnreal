#include "MainSounds.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
// Sets default values
AMainSounds::AMainSounds(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USoundCue> background_sound_obj(TEXT("SoundCue'/Game/Sound/sound_cues/background_main.background_main'"));
	if (background_sound_obj.Succeeded()) {
		background_sound_cue = background_sound_obj.Object;
		background_audio_comp = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundComp"));
		background_audio_comp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> CountdownObj(TEXT("SoundCue'/Game/Sound/countdown_cue.countdown_cue'"));
	if (CountdownObj.Succeeded()) {
		countdown_sound_cue = CountdownObj.Object;
		countdown_comp = CreateDefaultSubobject<UAudioComponent>(TEXT("CountdownComp"));
		countdown_comp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> EngineSoundCueObj(TEXT("SoundCue'/Game/Sound/Engine.Engine'"));
	if (EngineSoundCueObj.Succeeded()) {
		engine_sound_cue = EngineSoundCueObj.Object;
		engine_comp = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSoundComponent"));
		engine_comp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue> ground_obj(TEXT("SoundCue'/Game/Sound/ground_cue.ground_cue'"));
	if (ground_obj.Succeeded()) {
		ground_sound_cue = ground_obj.Object;
		ground_audio_comp = CreateDefaultSubobject<UAudioComponent>(TEXT("GroundComp"));
		ground_audio_comp->SetupAttachment(RootComponent);
	}
	static ConstructorHelpers::FObjectFinder<USoundBase> pop_obj(TEXT("SoundWave'/Game/Sound/Subaru_sounds/det3.det3'"));
	if (pop_obj.Succeeded()){
		pop_sound_base = pop_obj.Object;
	}
	
	is_sound_on = false;
	starting_point = 0.0f;
	base_ = 0.75f;
	is_done=false;
	pop_base = 0.6f;
}

// Called when the game starts or when spawned
void AMainSounds::BeginPlay(){
	Super::BeginPlay();
	engine_comp->Activate(true);
	engine_comp->SetSound(engine_sound_cue);
	engine_comp->Play(0.f);
	ground_audio_comp->Activate(true);
	ground_audio_comp->SetSound(ground_sound_cue);
	ground_audio_comp->Play(0.f);
}

// Called every frame
void AMainSounds::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

	if (is_sound_on && !is_done){
		base_ += DeltaTime;
		SetVolume(base_);
		if (base_>=1.3f){
			is_done = true;
		}
	}
}

void AMainSounds::PlayBackgroundMusic() {
	if (!is_sound_on){
		SelectStartPointBackground();
		background_audio_comp->Activate(true);
		background_audio_comp->SetSound(background_sound_cue);
		background_audio_comp->Play(starting_point);
		is_sound_on = true;
	}
}

void AMainSounds::SetVolume(const float& increase_) {
	background_audio_comp->SetVolumeMultiplier(increase_);
}
void AMainSounds::SetVolumeEngine(const float& increase_) {
	engine_comp->SetVolumeMultiplier(increase_);
	ground_audio_comp->SetVolumeMultiplier(increase_);
	pop_base = (increase_ / 2.f);
}
void AMainSounds::SetVolumeVoice(const float& increase_) {
	countdown_comp->SetVolumeMultiplier(increase_);
}

void AMainSounds::StopBackgroundMusic() {
	background_audio_comp->Activate(false);
}
void AMainSounds::SelectStartPointBackground() {
	starting_point = FMath::RandRange(0.0f, 11460.f);
}

float AMainSounds::GetVolume() {
	float vol_=background_audio_comp->VolumeMultiplier;
	return vol_;
}
float AMainSounds::GetEngineVolume() {
	float vol_ = engine_comp->VolumeMultiplier;
	return vol_;
}
float AMainSounds::GetVoiceVolume() {
	float vol_ = countdown_comp->VolumeMultiplier;
	return vol_;
}
void AMainSounds::PlayCountdown() {
	countdown_comp->Activate(true);
	countdown_comp->SetSound(countdown_sound_cue);
	countdown_comp->Play(0.f);
}

void AMainSounds::StopCountdown() {
	countdown_comp->Activate(false);
	countdown_comp->Stop();
}

void AMainSounds::SetEngineParam(const float& rpm, const float& kph){
	if (rpm < 600) {
		engine_comp->SetFloatParameter(FName("RPM"), 600);
	}
	else {
		engine_comp->SetFloatParameter(FName("RPM"), rpm);
		
		ground_audio_comp->SetFloatParameter(FName("RPM"), kph);
	}
}

void AMainSounds::PlayPopSound(const FVector& loc_) {
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), pop_sound_base, loc_, pop_base, 0.2f);
}