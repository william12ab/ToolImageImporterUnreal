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
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void FindOrder();
	void WhenToPlay();

	TArray<int> GetLengths() { return lengths_; }
	TArray<int> GetAngles() { return angles_; }
	TArray<int> GetInclines() { return inclines_; }
	TArray<int> GetWidths() { return widths_; }
};
