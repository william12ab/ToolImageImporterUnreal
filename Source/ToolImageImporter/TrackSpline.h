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
	virtual void Tick(float DeltaTime) override;
	void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, Category = "TrackSpline")
		USplineComponent* spline;

	void SetControlPoints(TArray<FVector2D> c_p) { control_points = c_p; }
	void SetHeightArray(TArray<float> h_) { height_z = h_; }



	void AddSafePoint(const int& index_one, const int& index_zero, const int& index_, const float& t_value);
	//index one would be for example, [1], zero would be [0], and for end it would be [end], [end-1]

	void CombinePoints();
	TArray<FVector> GetVerts() { return m_verts; }
	TArray<FVector> GetSEPoints() { return start_end_points; }
	TArray<FVector2D> GetSafetyPoints() { return saftey_points; }
	TArray<FVector> GetTotalPoints() { return points_total; }
	float GetMinHeight() { return min_height; }

	FVector2D LerpV2D(const FVector2D& p1, const FVector2D& p2, const float& t);

	void SetSpacing(int32 s_) { spacing_ = s_; }
	void SetDivision(float d_) { division_ = d_; }
	float GetHeightChange() { return amount_added_to_z; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<FVector2D> control_points;
	TArray<FVector2D> saftey_points;
	TArray<FVector> points_total;
	TArray<float> height_z;

	TArray<FVector> m_verts;
	int32 spacing_;
	float division_;
	TArray<FVector> start_end_points;
	float min_height;
	float amount_added_to_z;


};
