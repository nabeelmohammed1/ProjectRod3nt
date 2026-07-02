// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DirectionalLightComponent.h"
#include "RodentSun.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeChanged, int32, Hours, int32, Minutes);

//Strut for the time of day stuff
USTRUCT(BlueprintType)
struct FRodentSunPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SunRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LightIntensity = 10.0f;
};



UCLASS()
class PROJECTROD3NTTEST_API ARodentSun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARodentSun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AdvanceTime();
	FTimerHandle TmeUpdateHandle;

public:	

	//Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings")
	int32 StartHr = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings")
	int32 StartMin = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings")
	double TimeScale_Mins = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings")
	double ChangeSunTime_1 = 12.0; //First Variable change at noon

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings")
	double ChangeSunTime_2 = 15.0; //Second Variable change at 6pm

	//States

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings")
	double CurrentMin = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Settings")
	double EndTime_Min = 1080.0; //6pm	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
	bool bCanUpdate = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
	bool bIsTutorial = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
	bool bIsUnderground = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
	bool bHasDied = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UDirectionalLightComponent> SunLightComponent;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnTimeChanged OnTimeChanged;

	int32 LastReportedMin = -1; //Helper to prevent spamming

	//Struct Configuration

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun Settings")
	TMap<FName, FRodentSunPreset> SunPresets;

	//Default preset to start the game with
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sun Settings")
	FName DefaultPresetName = "Morning";

	//Functions
	UFUNCTION(BlueprintCallable, Category = "Time Settings")
	void UpdateSun(); //No longer used but just in case

	UFUNCTION(BlueprintCallable, Category = "Time Settings")
	void ChangeTime(double NewTime); //Function mightnt be used but just in case

	UFUNCTION(BlueprintCallable, Category = "Sun Settings")
	void ApplySunPreset(FName PresetName);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Game Logic")
	void OnGameLost();

private:
	bool bHasLost = false; //stop continuous triggering of OnGameLost
	FName LastAppliedPreset;
};
