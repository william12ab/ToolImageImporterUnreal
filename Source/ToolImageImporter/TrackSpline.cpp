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

    spline = CreateDefaultSubobject<USplineComponent>("Spline",true);
    if (spline)
    {
        SetRootComponent(spline);
    }
	UE_LOG(LogTemp, Warning, TEXT("constructor"));

}

// Called when the game starts or when spawned
void ATrackSpline::BeginPlay()
{
	Super::BeginPlay();

}

void ATrackSpline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//spline->RemoveSplinePoint(0, true);
	spline->ClearSplinePoints();

	
	for (int i = 0; i < (control_points.Num()); i++) {
		int x = control_points[i].X / 20;
		int y = control_points[i].Y / 20;
		spline->AddSplineLocalPoint(FVector(control_points[i].X, control_points[i].Y, (height_z[y * 400 + x]*20)/3.0f));
	}
	
	if (spline)
	{

		const int32 spline_points = spline->GetNumberOfSplinePoints();
		UE_LOG(LogTemp, Warning, TEXT("spline points in loop: %d"), spline_points);

		for (int spline_count = 0; spline_count < (spline_points - 1); spline_count++)
		{
			USplineMeshComponent* spline_mesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			UStaticMesh* static_mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'")));
			UMaterialInterface* Material = nullptr;
			Material = LoadObject<UMaterialInterface>(NULL, TEXT("Material'/Game/Materials/testmaterial.testmaterial'"));

			// update mesh details
			spline_mesh->SetStaticMesh(static_mesh);
			spline_mesh->SetForwardAxis(ESplineMeshAxis::X, true);
			spline_mesh->SetMaterial(0, Material);
			
			// initialize the object
			spline_mesh->RegisterComponentWithWorld(GetWorld());

			spline_mesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			spline_mesh->SetMobility(EComponentMobility::Movable);

			spline_mesh->AttachToComponent(spline, FAttachmentTransformRules::KeepRelativeTransform);

			// define the positions of the points and tangents
			const FVector StartPoint = spline->GetLocationAtSplinePoint(spline_count, ESplineCoordinateSpace::Type::Local);
			const FVector StartTangent = spline->GetTangentAtSplinePoint(spline_count, ESplineCoordinateSpace::Type::Local);
			const FVector EndPoint = spline->GetLocationAtSplinePoint(spline_count + 1, ESplineCoordinateSpace::Type::Local);
			const FVector EndTangent = spline->GetTangentAtSplinePoint(spline_count + 1, ESplineCoordinateSpace::Type::Local);
			spline_mesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);

			// query physics
			spline_mesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
			spline_mesh->bCastDynamicShadow=false;		

			
		}
	}
	
}