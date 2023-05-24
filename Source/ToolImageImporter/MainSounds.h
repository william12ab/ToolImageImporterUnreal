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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* background_sound_cue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float starting_point;
	UAudioComponent* background_audio_comp;
	bool is_sound_on;
	float base_;
	bool is_done;
};
