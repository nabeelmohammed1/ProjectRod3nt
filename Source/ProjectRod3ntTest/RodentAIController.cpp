// Fill out your copyright notice in the Description page of Project Settings.


#include "RodentAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h" // Required for DrawDebugCone



ARodentAIController::ARodentAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = false;
	//set tick to false so it won't run until player spawns in

	//Create the Perception Comp
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	SetPerceptionComponent(*AIPerceptionComp);
}

void ARodentAIController::BeginPlay()
{
	Super::BeginPlay();
	InitSightDefaults();
}

void ARodentAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	InitSightDefaults();
}

void ARodentAIController::SetAiActive(bool bNewState)
{
	bStartNow = bNewState;
	SetActorTickEnabled(bNewState);

}

void ARodentAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bStartNow)
	{
		LastTimeSawPlayer();
	}
}

void ARodentAIController::InitSightDefaults()
{
	if(bDefaultsInitialized && IsValid(SightConfig))
	{
		return;
	}

	if(!IsValid(AIPerceptionComp))
	{
		AIPerceptionComp = FindComponentByClass<UAIPerceptionComponent>();
		if(!IsValid(AIPerceptionComp))
		{
			UE_LOG(LogTemp, Warning, TEXT("RodentAIController: No AIPerceptionComponent found!"));
			return;
		}
	}

	SightConfig = AIPerceptionComp->GetSenseConfig<UAISenseConfig_Sight>();
	

	if(!IsValid(SightConfig))
	{
		return;
	}
	DefaultPeripheralAngleDeg = SightConfig->PeripheralVisionAngleDegrees;
	DefaultSightRadius = SightConfig->SightRadius;
	DefaultLoseSightRadius = SightConfig->LoseSightRadius;
	bDefaultsInitialized = true;
}

void ARodentAIController::ReduceVisionTemporaily(float NewPeripheralAngle, float NewSightRadius)
{
	InitSightDefaults();

	if(!IsValid(AIPerceptionComp) || !IsValid(SightConfig))
	{
		return;
	}

	SightConfig->PeripheralVisionAngleDegrees = FMath::Clamp(NewPeripheralAngle, 5.f, 180.f);
	SightConfig->SightRadius = FMath::Max(0.f, NewSightRadius);

	//This should help prevent flickering
	const float DesiredLoseSightRadius = SightConfig->SightRadius + 100.f;
	SightConfig->LoseSightRadius = FMath::Max(DesiredLoseSightRadius, SightConfig->LoseSightRadius);
	AIPerceptionComp->RequestStimuliListenerUpdate();

	//Update the perception component with new config
	AIPerceptionComp->ConfigureSense(*SightConfig);
}

void ARodentAIController::RestoreVisionToNormal()
{
	InitSightDefaults();
	if(!IsValid(AIPerceptionComp) || !IsValid(SightConfig) || !bDefaultsInitialized)
	{
		return;
	}
	SightConfig->PeripheralVisionAngleDegrees = DefaultPeripheralAngleDeg;
	SightConfig->SightRadius = DefaultSightRadius;
	SightConfig->LoseSightRadius = DefaultLoseSightRadius;
	AIPerceptionComp->RequestStimuliListenerUpdate();
	//Update the perception component with new config
	AIPerceptionComp->ConfigureSense(*SightConfig);
}
