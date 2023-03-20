// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackSpline.h"
#include "Components/SplineMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

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

FVector StaticMeshToSplineMeshVertexPosition(const FVector& StaticMeshVertexPosition, USplineMeshComponent* SplineMeshComponent)
{
	const float VertexPositionAlongSpline = StaticMeshVertexPosition[SplineMeshComponent->ForwardAxis];
	const FTransform StaticMeshToSplineMeshTransform = SplineMeshComponent->CalcSliceTransform(VertexPositionAlongSpline);
	FVector SlicePos = StaticMeshVertexPosition;
	SlicePos[SplineMeshComponent->ForwardAxis] = 0;
	const FVector SplineMeshSpaceVector = StaticMeshToSplineMeshTransform.TransformPosition(SlicePos);
	FVector ResultPos = SplineMeshSpaceVector;
	return SplineMeshSpaceVector;
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
			spline_mesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

			spline_mesh->AttachToComponent(spline, FAttachmentTransformRules::KeepRelativeTransform);

			// define the positions of the points and tangents
			const FVector StartPoint = spline->GetLocationAtSplinePoint(spline_count, ESplineCoordinateSpace::Type::Local);
			const FVector StartTangent = spline->GetTangentAtSplinePoint(spline_count, ESplineCoordinateSpace::Type::Local);
			const FVector EndPoint = spline->GetLocationAtSplinePoint(spline_count + 1, ESplineCoordinateSpace::Type::Local);
			const FVector EndTangent = spline->GetTangentAtSplinePoint(spline_count + 1, ESplineCoordinateSpace::Type::Local);
			spline_mesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
			start_end_points.Add(StartPoint);
			start_end_points.Add(EndPoint);

			// query physics
			//getting vertices of spline
			if (spline_mesh->GetStaticMesh()->RenderData->LODResources.Num() > 0)
			{
				FPositionVertexBuffer* vertex_buffer = &spline_mesh->GetStaticMesh()->RenderData->LODResources[0].VertexBuffers.PositionVertexBuffer;
				if (vertex_buffer)
				{
					const int32 vertex_count= vertex_buffer->GetNumVertices();
					for (int32 index_ = 0; index_ < vertex_count; index_++)
					{
						//This is in the Static Mesh Actor Class, so it is location and tranform of the SMActor
						
						const FVector StaticMeshSpacePosition = vertex_buffer->VertexPosition(index_);
						const FVector SplineMeshSpacePosition = StaticMeshToSplineMeshVertexPosition(StaticMeshSpacePosition, spline_mesh);
						const FVector WorldSpacePosition = spline_mesh->GetComponentLocation() + spline_mesh->GetComponentTransform().TransformVector(SplineMeshSpacePosition);
						m_verts.Add(WorldSpacePosition);
					}
				}
			}
		}
	}
	
}
