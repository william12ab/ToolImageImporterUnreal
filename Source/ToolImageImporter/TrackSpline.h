// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"


#include "TrackSpline.generated.h"



UCLASS()
class TOOLIMAGEIMPORTER_API ATrackSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrackSpline();

	void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, Category = "TrackSpline")
		USplineComponent* spline;

	void SetControlPoints(TArray<FVector> c_p) { control_points = c_p; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<FVector> control_points;
	
};
