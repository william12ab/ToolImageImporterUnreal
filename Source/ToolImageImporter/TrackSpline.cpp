// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackSpline.h"
#include "Components/SplineMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
// Sets default values
ATrackSpline::ATrackSpline() {
	PrimaryActorTick.bCanEverTick = true;
	spline = CreateDefaultSubobject<USplineComponent>("Spline", true);
	if (spline) {
		SetRootComponent(spline);
	}
	spacing_ = 20;
	division_ = 5.0f;
	amount_added_to_z = -10;
	is_outside_bounds = false;
	
}

// Called when the game starts or when spawned
void ATrackSpline::BeginPlay() {
	Super::BeginPlay();
	spline->SetGenerateOverlapEvents(false);
	spline->SetNotifyRigidBodyCollision(true);
}

void ATrackSpline::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

FVector StaticMeshToSplineMeshVertexPosition(const FVector& StaticMeshVertexPosition, USplineMeshComponent* SplineMeshComponent) {
	const float VertexPositionAlongSpline = StaticMeshVertexPosition[SplineMeshComponent->ForwardAxis];
	const FTransform StaticMeshToSplineMeshTransform = SplineMeshComponent->CalcSliceTransform(VertexPositionAlongSpline);
	FVector SlicePos = StaticMeshVertexPosition;
	SlicePos[SplineMeshComponent->ForwardAxis] = 0;
	const FVector SplineMeshSpaceVector = StaticMeshToSplineMeshTransform.TransformPosition(SlicePos);
	FVector ResultPos = SplineMeshSpaceVector;
	return SplineMeshSpaceVector;
}

FVector2D ATrackSpline::LerpV2D(const FVector2D& p1, const FVector2D& p2, const float& t) {
	auto a = p2 - p1;
	auto b = FVector2D(a.X * t, a.Y * t);
	auto c = p1 + b;
	return c;
}

void ATrackSpline::CombinePoints() {
	control_points.EmplaceAt(0, saftey_points[0]);
	control_points.EmplaceAt(control_points.Num(), saftey_points[1]);
}
void ATrackSpline::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	spline->ClearSplinePoints();
	min_height = 1000000000000;
	for (int i = 0; i < (control_points.Num()); i++) {
		int x = control_points[i].X / spacing_;
		int y = control_points[i].Y / spacing_;
		if (x>=0 && x<400 &&y>=0&&y<400){
			if ((float)((height_z[y * 400 + x] * spacing_) / division_ < min_height)) {
				min_height = (float)((height_z[y * 400 + x] * spacing_) / division_);
			}
			spline->AddSplineLocalPoint(FVector(control_points[i].X, control_points[i].Y, ((float)((height_z[y * 400 + x] * spacing_) / division_)) + amount_added_to_z));
		}
	}
	for (size_t i = 0; i < spline->GetNumberOfSplinePoints(); i++) {
		spline->SetSplinePointType(i, ESplinePointType::CurveCustomTangent, true);
	}
	if (spline) {
		const int32 spline_points = spline->GetNumberOfSplinePoints();
		for (int spline_count = 0; spline_count < (spline_points - 1); spline_count++)	{
			USplineMeshComponent* spline_mesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
			UStaticMesh* static_mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'")));
			// update mesh details
			spline_mesh->SetStaticMesh(static_mesh);
			spline_mesh->SetForwardAxis(ESplineMeshAxis::X, true);

			// initialize the object
			spline_mesh->RegisterComponentWithWorld(GetWorld());

			spline_mesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			spline_mesh->SetMobility(EComponentMobility::Movable);
			spline_mesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
			spline_mesh->AttachToComponent(spline, FAttachmentTransformRules::KeepRelativeTransform);

			// define the positions of the points and tangents
			const FVector StartPoint = spline->GetLocationAtSplinePoint(spline_count, ESplineCoordinateSpace::Type::Local);
			points_total.Add(FVector(StartPoint));

			const FVector StartTangent = spline->GetTangentAtSplinePoint(spline_count, ESplineCoordinateSpace::Type::Local);
			const FVector EndPoint = spline->GetLocationAtSplinePoint(spline_count + 1, ESplineCoordinateSpace::Type::Local);
			const FVector EndTangent = spline->GetTangentAtSplinePoint(spline_count + 1, ESplineCoordinateSpace::Type::Local);
			spline_mesh->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
			start_end_points.Add(StartPoint);
			start_end_points.Add(EndPoint);
			//holds the start and end point of each spline part.

	
			//getting vertices of spline
			if (spline_mesh->GetStaticMesh()->RenderData->LODResources.Num() > 0) {
				FPositionVertexBuffer* vertex_buffer = &spline_mesh->GetStaticMesh()->RenderData->LODResources[0].VertexBuffers.PositionVertexBuffer;
				if (vertex_buffer) {
					const int32 vertex_count = vertex_buffer->GetNumVertices();
					for (int32 index_ = 0; index_ < vertex_count; index_++) {
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