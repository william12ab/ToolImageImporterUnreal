// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
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
	TArray<int> pacenotes_array;
	int note_count ;
	int play_value;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void FindOrder();
	void WhenToPlay(const FVector& p1, const FVector& p2, const FVector& p3);
	void FindAngle(const int& i);

	TArray<int> GetLengths() { return lengths_; }
	TArray<int> GetAngles() { return angles_; }
	TArray<int> GetInclines() { return inclines_; }
	TArray<int> GetWidths() { return widths_; }

	void SetLengths(const TArray<int>&v) { lengths_ = v; }
	void SetAngles(const TArray<int>& v) { angles_ = v; }
	void SetInclines(const TArray<int>& v) { inclines_ = v; }
	void SetWidths(const TArray<int>& v) { widths_= v; }
	void PlayNextNote();
	void PlayAddition();
};
