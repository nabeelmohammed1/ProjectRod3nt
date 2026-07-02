// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Rod3ntInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWindowFocusChanged, bool, bIsFocused);

UCLASS()
class PROJECTROD3NTTEST_API URod3ntInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;

    UPROPERTY(BlueprintAssignable, Category = "System Events")
    FOnWindowFocusChanged OnWindowFocusChanged;

protected:
    void HandleWindowFocusChanged(bool bIsFocused);
    void OnAppGainedFocus();
    void OnAppLostFocus();
};