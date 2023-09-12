// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "PaceNotesActor.generated.h"

UCLASS()
class TOOLIMAGEIMPORTER_API APaceNotesActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APaceNotesActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<int>lengths_;
	TArray<int>angles_;
	TArray<int>inclines_;
	TArray<int>widths_;
	TArray<int> directions_;
	TArray<int> pacenotes_array;
	int note_count ;
	float play_value;
	bool is_played;
	bool is_playing;
	float accum_time;
	TArray<USoundCue*> cues_;
	int turn_counter;
	int turn_counter_called;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void FindOrder();
	void WhenToPlay(const FVector2D& p1, const FVector2D& p2, const FVector2D& p3);
	void FindAngle(const int& i);
	void FindDirection(const int& i, const int& n, const int&a);

	TArray<int> GetLengths() { return lengths_; }
	TArray<int> GetAngles() { return angles_; }
	TArray<int> GetInclines() { return inclines_; }
	TArray<int> GetWidths() { return widths_; }
	TArray<int> GetDirections() { return directions_; }
	void SetLengths(const TArray<int>&v) { lengths_ = v; }
	void SetAngles(const TArray<int>& v) { angles_ = v; }
	void SetInclines(const TArray<int>& v) { inclines_ = v; }
	void SetWidths(const TArray<int>& v) { widths_= v; }
	void SetDirections(const TArray<int>& v) { directions_ = v; }
	void SetTurnCounter(const int& a) { turn_counter = a; }
	void PlayNextNote();
	void PlayAddition();
	void SetIsPlayed() { is_played = false; }
	bool GetIsPlayed() { return is_played; }
	void Playing(float DeltaTime);
	void PlayNote(const float& DeltaTime, const int& index);

	//comps

	UAudioComponent* current_comp;
	//sound cues
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* hunder_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* hunderf_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* twohunder_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* one_l_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* one_r_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* two_r_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* two_l_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* three_l_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* three_r_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* four_r_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* four_l_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* five_r_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* five_l_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* six_r_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* six_l_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* hp_l_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* hp_r_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* sq_l_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* sq_r_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* into_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* and_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* dip_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* long_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* narrow_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* open_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* crest_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* short_cue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
		class USoundCue* tighten_cue;
};
