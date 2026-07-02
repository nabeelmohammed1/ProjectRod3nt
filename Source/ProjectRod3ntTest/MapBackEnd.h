// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h" //Texture
#include "MapBackEnd.generated.h"


// Enum for different map states
UENUM(BlueprintType)
enum class EMapState : uint8
{
	AboveGround,
	InBunker
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTROD3NTTEST_API UMapBackEnd : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMapBackEnd();

	bool bIsPlayerUnderground = false; // NEED to use this for WBP_PlayerIcon

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// Assign in Details Panel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map System|UI")
	TSubclassOf<UUserWidget> WBP_Map;          // Assign WBP_Map in Panel

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Map System|UI")
	TArray<UUserWidget*> MapWidgets;

	UPROPERTY(BlueprintReadOnly, Category = "Map System|UI")
	UUserWidget* MapWidget = nullptr;

	// Icon texture2D needs to be EditAnywhere to appear in the Details
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map System|UI")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Map System|UI")
	void CacheMapWidgets();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map System")
	AActor* TargetActor;

	//Dimensions variables
	//X= Min, Y=Max
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map System")
	FVector2D MapWorldMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map System")
	FVector2D MapWorldMax;

	//Change State for the logic
	UPROPERTY(BlueprintReadWrite, Category = "Map System")
	EMapState CurrentMapState;

	//Function to callfor entering and exit bunker
	UFUNCTION(BlueprintCallable, Category = "Map System")
	void SetMapState(EMapState NewState);

	//Helpers for the UI part
	UFUNCTION(BlueprintCallable, Category = "Map System")
	FVector2D GetNormalizedPosition(FVector2D WorldLocation);

	UFUNCTION(BlueprintCallable, Category = "Map System")
	FVector GetTargetLocation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map System") // Float Z to see when to swtich maps
	float BunkerZThreshold = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Map System")
	bool UpdateUndergroundState();

	UFUNCTION(BlueprintCallable, Category = "Map System|Tracking") // Get player's all three LoC
	FVector GetPlayerLocation() const;

	UFUNCTION(BlueprintCallable, Category = "Map System|Tracking")
	FVector2D GetPlayerNormalizedPosition();

	UFUNCTION(BlueprintCallable, Category = "Map System|Tracking")
	FVector2D GetTargetNormalizedPosition();

};
