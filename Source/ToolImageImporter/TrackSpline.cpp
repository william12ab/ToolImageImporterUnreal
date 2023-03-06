// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackSpline.h"
#include "Components/SplineMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
// Sets default values
ATrackSpline::ATrackSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    spline = CreateDefaultSubobject<USplineComponent>("Spline");
    if (spline)
    {
        SetRootComponent(spline);
    }
}

// Called when the game starts or when spawned
void ATrackSpline::BeginPlay()
{
	Super::BeginPlay();


}

void ATrackSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	int x = 0;
	for (int i = 0; i < 5; i++) {
		FSplinePoint myPoint;
		myPoint.Position = FVector(x, 0.0f, 0.0f);
		myPoint.Rotation = FRotator(0.0f, 0.0f, 0.0f);
		myPoint.Scale = FVector(0.0f, 0.0f, 0.0f);
		x += 100;
		spline->AddPoint(myPoint, false);
	}
	if (spline)
	{
		UE_LOG(LogTemp, Warning, TEXT("true"));

		const int32 SplinePoints = spline->GetNumberOfSplinePoints();
		UE_LOG(LogTemp, Warning, TEXT("spline points: %d"), SplinePoints);

		for (int SplineCount = 0; SplineCount < (SplinePoints - 1); SplineCount++)
		{
		
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			UStaticMesh* StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'")));
			UMaterialInterface* Material = nullptr;
			Material = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/Materials/testmaterial.testmaterial'"));
			
			Material = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/Materials/testmaterial.testmaterial'"));
			// update mesh details
			SplineMesh->SetStaticMesh(StaticMesh);
			SplineMesh->SetForwardAxis(ESplineMeshAxis::X, true);
			SplineMesh->SetMaterial(0, Material);
			
			// initialize the object
			SplineMesh->RegisterComponentWithWorld(GetWorld());

			SplineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			SplineMesh->SetMobility(EComponentMobility::Movable);

			SplineMesh->AttachToComponent(spline, FAttachmentTransformRules::KeepRelativeTransform);

			// define the positions of the points and tangents
			const FVector StartPoint = spline->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::Type::Local);
			const FVector StartTangent = spline->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::Type::Local);
			const FVector EndPoint = spline->GetLocationAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Type::Local);
			const FVector EndTangent = spline->GetTangentAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::Type::Local);
			SplineMesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);

			// query physics
			SplineMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		}
	}
}