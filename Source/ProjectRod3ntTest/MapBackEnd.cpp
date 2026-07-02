// Fill out your copyright notice in the Description page of Project Settings.


#include "MapBackEnd.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

// Sets default values for this component's properties
UMapBackEnd::UMapBackEnd()
{
	//Component shouldn't need to tick I think
	PrimaryComponentTick.bCanEverTick = false;
	CurrentMapState = EMapState::AboveGround;
}

bool UMapBackEnd::UpdateUndergroundState() // Update when player is Exterior or Interior of the level (use this in WBP_Map)
{
	const AActor* OwnerActor = GetOwner();
	bIsPlayerUnderground = OwnerActor ? (OwnerActor->GetActorLocation().Z < BunkerZThreshold) : false;
	return bIsPlayerUnderground;
}

// Called when the game starts
void UMapBackEnd::BeginPlay()
{
	Super::BeginPlay();
	CacheMapWidgets();
	//Maybe the find all the entrances here?
}

//Creates Map Reference
void UMapBackEnd::CacheMapWidgets()
{
	MapWidgets.Reset();
	MapWidget = nullptr;
	
	if (!WBP_Map) return;

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), MapWidgets, WBP_Map, false);
	MapWidget = (MapWidgets.Num() > 0) ? MapWidgets[0] : nullptr;
}
//Change state for the map logic
void UMapBackEnd::SetMapState(EMapState NewState)
{
	CurrentMapState = NewState;
}

FVector UMapBackEnd::GetTargetLocation()
{
	if (TargetActor)
	{
		return TargetActor->GetActorLocation();
	}
	return FVector::ZeroVector;
}

FVector2D UMapBackEnd::GetNormalizedPosition(FVector2D WorldLocation)
{
	//Math for getting normalized position

	float RangeX = MapWorldMax.X - MapWorldMin.X;
	float RangeY = MapWorldMax.Y - MapWorldMin.Y;

	//Preventing the division by zero

	if (RangeX == 0 || RangeY == 0) return FVector2D::ZeroVector;

	float NormalizedX = (WorldLocation.X - MapWorldMin.X) / RangeX;
	float NormalizedY = (WorldLocation.Y - MapWorldMin.Y) / RangeY;

	return FVector2D(NormalizedX, NormalizedY);
}

FVector UMapBackEnd::GetPlayerLocation() const
{
	const AActor* OwnerActor = GetOwner();
	return OwnerActor ? OwnerActor->GetActorLocation() : FVector::ZeroVector;
}

FVector2D UMapBackEnd::GetPlayerNormalizedPosition()
{
	const FVector P = GetPlayerLocation();
	return GetNormalizedPosition(FVector2D(P.X, P.Y));
}

FVector2D UMapBackEnd::GetTargetNormalizedPosition()
{
	const FVector T = GetTargetLocation();
	return GetNormalizedPosition(FVector2D(T.X, T.Y));
}

