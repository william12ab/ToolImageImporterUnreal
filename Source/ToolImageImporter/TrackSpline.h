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

	void SetControlPoints(TArray<FVector2D> c_p) { control_points = c_p; }
	void SetHeightArray(TArray<float> h_) { height_z = h_; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<FVector2D> control_points;
	TArray<float> height_z;
	
};
