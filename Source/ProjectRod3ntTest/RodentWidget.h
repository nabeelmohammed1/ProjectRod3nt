// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonInputSubsystem.h"
#include "RodentWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTROD3NTTEST_API URodentWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	//Function for the input change
	UFUNCTION()
	void HandleInputChanged(ECommonInputType NewInputType);

	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void OnInputTypeChanged(bool bIsGamepad);
	
};
