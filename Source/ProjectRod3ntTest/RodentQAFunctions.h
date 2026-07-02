// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RodentQAFunctions.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTROD3NTTEST_API URodentQAFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Simply takes a filename and a vector, returning true if successful
	UFUNCTION(BlueprintCallable, Category = "Heatmap")
	static bool LogPlayerLocation(FString FileName, FVector Location);
};
