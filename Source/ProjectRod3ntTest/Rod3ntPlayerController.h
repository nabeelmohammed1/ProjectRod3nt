// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "InputActionValue.h"
#include "Rod3ntPlayerController.generated.h"


class UInputAction;	
/**
 * 
 */
UCLASS()
class PROJECTROD3NTTEST_API ARod3ntPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	ARod3ntPlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;


	//input overrides
	virtual void SetupInputComponent() override;

protected:
	//InputAction for registering input activity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputAction> AnyActivityAction;

	//seconds of inactivity before we consider the player idle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inactivity")
	float idledelay;

	//Widget to play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inactivity")
	TSubclassOf<UUserWidget> IdleWidgetClass;

	//Last time input used
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Inactivity")
	float LastInputTime;

	//Current Widget Instance
	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> IdleWidgetInstance;

	//Widget Visible?
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Inactivity")
	bool bIdleWidgetVisible;

	void OnInputActivity(const FInputActionValue& Value);

	//helper function for axis inputs
	void OnAxisInput(float Val);

	//Called whenever input is occurs
	void RegisterInputActivity();

	//Show or hide the idle widget
	void ShowIdleWidget();
	void HideIdleWidget();


};
