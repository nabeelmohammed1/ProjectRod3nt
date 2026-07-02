// Fill out your copyright notice in the Description page of Project Settings.


#include "Rod3ntPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "InputCoreTypes.h"//for the keys 


ARod3ntPlayerController::ARod3ntPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	idledelay = 60.f; //seconds
	LastInputTime = 0.f;
	bIdleWidgetVisible = false;
	IdleWidgetInstance = nullptr;
}

void ARod3ntPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(UWorld*World = GetWorld())
	{
		LastInputTime = World->GetTimeSeconds();
	}
}

void ARod3ntPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!GetWorld() || bIdleWidgetVisible || !IdleWidgetClass)
	{
		return;
	}

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	const float TimeIdle = CurrentTime - LastInputTime;

	if (TimeIdle >= idledelay)
	{
		ShowIdleWidget();
	}
}

//Input Code

void ARod3ntPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent*EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if(AnyActivityAction)
		{
			EnhancedInputComponent->BindAction(AnyActivityAction, ETriggerEvent::Triggered, this, &ARod3ntPlayerController::OnInputActivity);
		}
	}
}

void ARod3ntPlayerController::OnInputActivity(const FInputActionValue& Value)
{
	RegisterInputActivity();
}

//idle Logic

void ARod3ntPlayerController::RegisterInputActivity()
{
	if (!GetWorld())
	{
		return;
	}

	LastInputTime = GetWorld()->GetTimeSeconds();

	if(bIdleWidgetVisible)
	{
		HideIdleWidget();
	}
}

void ARod3ntPlayerController::ShowIdleWidget()
{
	if (bIdleWidgetVisible || !IdleWidgetClass)
	{
		return;
	}
	IdleWidgetInstance = CreateWidget<UUserWidget>(this, IdleWidgetClass);
	if (IdleWidgetInstance)
	{
		IdleWidgetInstance->AddToViewport();
		bIdleWidgetVisible = true;
	}
}

void ARod3ntPlayerController::HideIdleWidget()
{
	if (!bIdleWidgetVisible)
	{
		return;
	}
	if (IdleWidgetInstance)
	{
		IdleWidgetInstance->RemoveFromParent();
		IdleWidgetInstance = nullptr;
	}

	bIdleWidgetVisible = false;
}
