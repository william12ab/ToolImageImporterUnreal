#include "MainSounds.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"
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
	is_sound_on = false;
	starting_point = 0.0f;
	base_ = 0.75f;
	is_done=false;
}

// Called when the game starts or when spawned
void AMainSounds::BeginPlay(){
	Super::BeginPlay();
	
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

void AMainSounds::PlayCountdown() {
	countdown_comp->Activate(true);
	countdown_comp->SetSound(countdown_sound_cue);
	countdown_comp->Play(0.f);
}

void AMainSounds::StopCountdown() {
	countdown_comp->Activate(false);
}