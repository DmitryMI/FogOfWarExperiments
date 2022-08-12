// Fill out your copyright notice in the Description page of Project Settings.


#include "FoWControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"

// Sets default values
AFoWControllerBase::AFoWControllerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFoWControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FBox AFoWControllerBase::GetBoundingBox(const FRotator& rotation, const FVector& pivot, const TArray<FVector>& points)
{
	TArray<FVector> rotatedPoints;
	for (int i = 0; i < points.Num(); i++)
	{
		rotatedPoints.Add(rotation.UnrotateVector(points[i] - pivot) + pivot);
	}

	return FBox(rotatedPoints);
}

FPlane AFoWControllerBase::GetGroundPlane()
{
	FPlane groundPlane = FPlane(FVector(0, 0, viewportBoxMinZ), FVector::UpVector);
	return groundPlane;
}

FVector AFoWControllerBase::IntersectGroundPlane(const FVector& start, const FVector& dir)
{
	FVector end = start + dir * screenRayLength;
	FVector intersectionWorld =  FMath::LinePlaneIntersection(start, end, GetGroundPlane());
	DrawDebugLine(GetWorld(), start, intersectionWorld, FColor::Blue);

	return intersectionWorld;
}

void AFoWControllerBase::GetViewportTransform(FBox& OutBoundingBoxWorld, FRotator& OutWorldRotation, FVector& OutCameraLocation, FRotator& OutCameraRotation)
{
	APlayerController* localPlayerController = GetWorld()->GetFirstPlayerController();

	localPlayerController->GetPlayerViewPoint(OutCameraLocation, OutCameraRotation);

	int32 viewportSizeX;
	int32 viewportSizeY;

	localPlayerController->GetViewportSize(viewportSizeX, viewportSizeY);

	FVector mouseWorldLeftTopPosition;
	FVector mouseWorldLeftTopDirection;
	localPlayerController->DeprojectScreenPositionToWorld(0, 0, mouseWorldLeftTopPosition, mouseWorldLeftTopDirection);

	FVector mouseWorldRightTopPosition;
	FVector mouseWorldRightTopDirection;
	localPlayerController->DeprojectScreenPositionToWorld(viewportSizeX, 0, mouseWorldRightTopPosition, mouseWorldRightTopDirection);

	FVector mouseWorldRightBotPosition;
	FVector mouseWorldRightBotDirection;
	localPlayerController->DeprojectScreenPositionToWorld(viewportSizeX, viewportSizeY, mouseWorldRightBotPosition, mouseWorldRightBotDirection);

	FVector mouseWorldLeftBotPosition;
	FVector mouseWorldLeftBotDirection;
	localPlayerController->DeprojectScreenPositionToWorld(0, viewportSizeY, mouseWorldLeftBotPosition, mouseWorldLeftBotDirection);

	FVector mouseWorldCenterPosition;
	FVector mouseWorldCenterDirection;
	localPlayerController->DeprojectScreenPositionToWorld(viewportSizeX / 2.0f, viewportSizeY / 2.0f, mouseWorldCenterPosition, mouseWorldCenterDirection);

	
	FVector groundLeftTop = IntersectGroundPlane(mouseWorldLeftTopPosition, mouseWorldLeftTopDirection);
	FVector groundRightTop = IntersectGroundPlane(mouseWorldRightTopPosition, mouseWorldRightTopDirection);
	FVector groundRightBot = IntersectGroundPlane(mouseWorldRightBotPosition, mouseWorldRightBotDirection);
	FVector groundLeftBot = IntersectGroundPlane(mouseWorldLeftBotPosition, mouseWorldLeftBotDirection);

	OutWorldRotation = FRotator(0, 0, OutCameraRotation.Yaw);
	FBox box = GetBoundingBox(OutWorldRotation,
		OutCameraLocation,
		{ 
			OutCameraLocation,
			groundLeftTop,
			groundRightTop,
			groundRightBot,
			groundLeftBot 
		});

	OutBoundingBoxWorld = box;
}

void AFoWControllerBase::DrawDebugViewportTransform(const FBox& OutBoundingBoxWorld, const FRotator& worldRotation, const FVector& cameraLocation, const FRotator& cameraRotation)
{
	FQuat boxRotationQuat = FQuat::MakeFromRotator(worldRotation);

	DrawDebugBox(GetWorld(), OutBoundingBoxWorld.GetCenter(), OutBoundingBoxWorld.GetSize() / 2.0, boxRotationQuat, FColor::Green);
}

const TArray<UFoWVisionBlockerComponentBase*>& AFoWControllerBase::GetVisionBlockers() const
{
	return visionBlockers;
}

const TArray<UFoWVisionSourceComponentBase*>& AFoWControllerBase::GetVisionSources() const
{
	return visionSources;
}

AActor* AFoWControllerBase::GetFogOfWarActor()
{
	return this;
}

void AFoWControllerBase::AdjustFogOfWarActorTransform(const FBox& viewportWorldBox, const FRotator& viewportWorldRotation)
{
	FBox expandedBox = viewportWorldBox.ExpandBy(FVector(0, 0, -fogOfWarMinZ));

	AActor* actor = GetFogOfWarActor();

	FVector location = expandedBox.GetCenter();

	location.Z = fogOfWarMinZ;	

	FVector boxExtent = expandedBox.GetExtent();
	FVector decalSize = FVector(boxExtent.Z, boxExtent.Y, boxExtent.X);

	actor->SetActorLocation(location);
	actor->SetActorRotation(viewportWorldRotation);

	UDecalComponent* decal = Cast<UDecalComponent>(GetComponentByClass(UDecalComponent::StaticClass()));
	decal->DecalSize = decalSize;
}

void AFoWControllerBase::RenderFogOfWar()
{
}

FVector AFoWControllerBase::GetFogOfWarCenter()
{
	return GetFogOfWarActor()->GetActorLocation();
}

// Called every frame
void AFoWControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FBox boundingBox;
	FRotator viewportWorldRotation;
	FVector viewportCameraLocation;
	FRotator viewportCameraRotation;

	GetViewportTransform(boundingBox, viewportWorldRotation, viewportCameraLocation, viewportCameraRotation);
	AdjustFogOfWarActorTransform(boundingBox, viewportWorldRotation);
	DrawDebugViewportTransform(boundingBox, viewportWorldRotation, viewportCameraLocation, viewportCameraRotation);

	RenderFogOfWar();
}

void AFoWControllerBase::RegisterVisionComponent(UFoWComponentBase* component)
{
	FString actorName;
	component->GetOwner()->GetName(actorName);	

	auto source = Cast<UFoWVisionSourceComponentBase>(component);
	if (source != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("FoW Vision Source registered for actor %s"), *actorName);
		RegisterVisionSource(source);
	}
	auto blocker = Cast<UFoWVisionBlockerComponentBase>(component);
	if (blocker != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("FoW Vision Blocker registered for actor %s"), *actorName);
		RegisterVisionBlocker(blocker);
	}
}

void AFoWControllerBase::UnregisterVisionComponent(UFoWComponentBase* component)
{
	FString actorName;
	component->GetOwner()->GetName(actorName);	

	auto source = Cast<UFoWVisionSourceComponentBase>(component);
	if (source != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FoW Vision Blocker unregistered for actor %s"), *actorName);
		UnregisterVisionSource(source);
	}
	auto blocker = Cast<UFoWVisionBlockerComponentBase>(component);
	if (blocker != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FoW Vision Blocker unregistered for actor %s"), *actorName);
		UnregisterVisionBlocker(blocker);
	}

	//FMath::UnwindAngle();
}

void AFoWControllerBase::RegisterVisionSource(UFoWVisionSourceComponentBase* source)
{
	visionSources.Add(source);
}

void AFoWControllerBase::RegisterVisionBlocker(UFoWVisionBlockerComponentBase* blocker)
{
	visionBlockers.Add(blocker);
}

void AFoWControllerBase::UnregisterVisionSource(UFoWVisionSourceComponentBase* source)
{
	visionSources.Remove(source);
}

void AFoWControllerBase::UnregisterVisionBlocker(UFoWVisionBlockerComponentBase* blocker)
{
	visionBlockers.Remove(blocker);
}

