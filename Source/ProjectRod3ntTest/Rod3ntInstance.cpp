// Fill out your copyright notice in the Description page of Project Settings.

#include "Rod3ntInstance.h"
#include "Misc/CoreDelegates.h"

void URod3ntInstance::Init()
{
    Super::Init();

    FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(this, &URod3ntInstance::OnAppGainedFocus);
    FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddUObject(this, &URod3ntInstance::OnAppLostFocus);
}

void URod3ntInstance::OnAppGainedFocus()
{
    OnWindowFocusChanged.Broadcast(true);
}

void URod3ntInstance::OnAppLostFocus()
{
    OnWindowFocusChanged.Broadcast(false);
}

void URod3ntInstance::HandleWindowFocusChanged(bool bIsFocused)
{
    // Broadcast to Blueprints
    OnWindowFocusChanged.Broadcast(bIsFocused);
}