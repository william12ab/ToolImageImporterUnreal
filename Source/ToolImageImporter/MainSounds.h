// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GameFramework/Actor.h"
#include "MainSounds.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API AMainSounds : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainSounds();
	virtual void Tick(float DeltaTime) override;

	void PlayBackgroundMusic();
	void StopBackgroundMusic();
	void SelectStartPointBackground();
	void SetVolume(const float &increase_);
	float GetVolume();
	void PlayCountdown();
	void StopCountdown();
	void SetEngineParam(const float& rpm, const float& kph);
	void PlayPopSound(const FVector& loc_);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* background_sound_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* countdown_sound_cue;
	UAudioComponent* countdown_comp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* engine_sound_cue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* ground_sound_cue;
	UPROPERTY(Category = MainMenuWidg, EditAnywhere, BlueprintReadWrite)
		USoundBase* pop_sound_base;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float starting_point;
	UAudioComponent* background_audio_comp;
	UAudioComponent* engine_comp;
	UAudioComponent* ground_audio_comp;
	bool is_sound_on;
	float base_;
	bool is_done;
};
