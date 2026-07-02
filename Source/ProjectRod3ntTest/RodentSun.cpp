// Fill out your copyright notice in the Description page of Project Settings.


#include "RodentSun.h"
#include "TimerManager.h"

// Sets default values
ARodentSun::ARodentSun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create Sun Light Component
	SunLightComponent = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLightComponent"));
	RootComponent = SunLightComponent;
}

// Called when the game starts or when spawned
void ARodentSun::BeginPlay()
{
	Super::BeginPlay();
	CurrentMin = (static_cast<double>(StartHr) * 60.0) + static_cast<double>(StartMin);
	ApplySunPreset(DefaultPresetName);
	LastAppliedPreset = DefaultPresetName;

	int32 StartTotalInt = FMath::FloorToInt(CurrentMin);
	LastReportedMin = StartTotalInt;
	if (OnTimeChanged.IsBound())
	{
		OnTimeChanged.Broadcast(StartTotalInt / 60, StartTotalInt % 60);
	}

	if(bCanUpdate)
	{
		GetWorldTimerManager().SetTimer(TmeUpdateHandle, this, &ARodentSun::AdvanceTime, 1.0f, true);
	}
}

// Called every frame
void ARodentSun::AdvanceTime()
{
	CurrentMin += (0.1 * TimeScale_Mins);

	if (CurrentMin >= EndTime_Min) //6pm where the game ends
	{
		if (!bIsTutorial && !bHasLost)
		{
			bCanUpdate = false;
			bHasLost = true;
			OnGameLost();
			GetWorldTimerManager().ClearTimer(TmeUpdateHandle);
		}
		return;
	}

		//Report Time Change Event
	int32 CurrentTotalInt = FMath::FloorToInt(CurrentMin);

	if (CurrentTotalInt != LastReportedMin)
	{
		LastReportedMin = CurrentTotalInt;

		//Calculuate Hours and Mins for UI

		int32 Hours = CurrentTotalInt / 60.0;
		int32 Mins = CurrentTotalInt % 60;

		if (OnTimeChanged.IsBound())
		{
			OnTimeChanged.Broadcast(Hours, Mins);
		}
	}

	if (bIsUnderground || bHasDied)
	{
		UpdateSun();
	}
}


void ARodentSun::ChangeTime(double NewTime)
{
	CurrentMin += NewTime;
}

void ARodentSun::UpdateSun()
{
	FName TargetPresetName;

	double CurrentHour = CurrentMin / 60.0;	

	if(CurrentHour >= ChangeSunTime_2)
	{
		TargetPresetName = "Evening";
	}
	else if(CurrentHour >= ChangeSunTime_1)
	{
		TargetPresetName = "Midday";
	}
	else
	{
		TargetPresetName = "Morning";
	}

	//Applying the preset only if it's different from the last applied one
	if (TargetPresetName != LastAppliedPreset)
	{
		ApplySunPreset(TargetPresetName);
		LastAppliedPreset = TargetPresetName;
	}
}


void ARodentSun::ApplySunPreset(FName PresetName)
{
	if (!SunLightComponent) return;

	//Finding the preset
	if (SunPresets.Contains(PresetName))
	{
		FRodentSunPreset* FoundPreset = SunPresets.Find(PresetName);
		if (FoundPreset)
		{
			SunLightComponent->SetWorldRotation(FoundPreset->SunRotation);
			SunLightComponent->SetIntensity(FoundPreset->LightIntensity);
		}
	}
}

