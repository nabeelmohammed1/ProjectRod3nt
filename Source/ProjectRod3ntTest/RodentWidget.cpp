// Fill out your copyright notice in the Description page of Project Settings.


#include "RodentWidget.h"
#include "CommonInputSubsystem.h"

void URodentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Get Subsystem
	if (const ULocalPlayer* LocalPlayer = GetOwningLocalPlayer())
	{
		if (UCommonInputSubsystem* InputSubsystem = UCommonInputSubsystem::Get(LocalPlayer))
		{
			// FIX: Use the 'Native' delegate with 'AddUObject' for C++
			InputSubsystem->OnInputMethodChangedNative.AddUObject(this, &URodentWidget::HandleInputChanged);

			// Call immediately to set the initial state
			HandleInputChanged(InputSubsystem->GetCurrentInputType());
		}
	}
}

void URodentWidget::HandleInputChanged(ECommonInputType NewInputType)
{
	const bool bIsGamepad = (NewInputType == ECommonInputType::Gamepad);
	OnInputTypeChanged(bIsGamepad);
}

