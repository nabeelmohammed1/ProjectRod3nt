// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"	
#include "BaseAttributes.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTROD3NTTEST_API UBaseAttributes : public UAttributeSet
{
	GENERATED_BODY()
public:
	UBaseAttributes();
	//Stamina Attribute
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Replicated=OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributes, Stamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Replicated = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributes, MaxStamina);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Replicated=OnRep_Swap)
	FGameplayAttributeData Swap;
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributes, Swap);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", Replicated = OnRep_MaxSwap)
	FGameplayAttributeData MaxSwap;
	ATTRIBUTE_ACCESSORS_BASIC(UBaseAttributes, MaxSwap);



public:
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const {
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributes, Stamina, OldStamina);
	}
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const {
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributes, MaxStamina, OldMaxStamina);
	}

	UFUNCTION()
	void OnRep_Swap(const FGameplayAttributeData& OldSwap) const {
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributes, Swap, OldSwap);
	}

	UFUNCTION()
	void OnRep_MaxSwap(const FGameplayAttributeData& OldMaxSwap) const {
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributes, MaxSwap, OldMaxSwap);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
