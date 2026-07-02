// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttributes.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UBaseAttributes::UBaseAttributes()
{
	Stamina = 100.f;
	MaxStamina = 100.f;
	Swap = 0.f;
	MaxSwap = 100.f;
}

void UBaseAttributes::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, Swap, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributes, MaxSwap, COND_None, REPNOTIFY_Always);
}

void UBaseAttributes::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
	else if(Attribute == GetSwapAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxSwap());
	}
}

void UBaseAttributes::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(GetStamina());
	}
	else if (Data.EvaluatedData.Attribute == GetSwapAttribute())
	{
		SetSwap(GetSwap());
	}
}

