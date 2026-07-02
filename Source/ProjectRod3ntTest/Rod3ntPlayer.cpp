// Fill out your copyright notice in the Description page of Project Settings.


#include "Rod3ntPlayer.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BaseAttributes.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DisplayDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/AudioComponent.h" 
#include "Sound/SoundBase.h"
#include "TimerManager.h"

// Sets default values
ARod3ntPlayer::ARod3ntPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true; // ensure the actor itself replicates

	// Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(ASCReplicationMode);

	// Capsule size
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// Use movement-based rotation, not controller yaw
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Character movement tuning (your values kept)
	CachedMoveComp = GetCharacterMovement();
	CachedMoveComp->bOrientRotationToMovement = true;
	CachedMoveComp->RotationRate = FRotator(0.f, 540.f, 0.f);
	CachedMoveComp->JumpZVelocity = 600.f;
	CachedMoveComp->AirControl = 0.2f;
	CachedMoveComp->MaxWalkSpeed = 600.f;
	CachedMoveComp->MinAnalogWalkSpeed = 150.f;
	CachedMoveComp->BrakingDecelerationWalking = 2048.f;
	CachedMoveComp->BrakingDecelerationFalling = 1500.f;

	//Grab the Defaults to reset back to after spawn glide
	NormalGravityScale = CachedMoveComp->GravityScale;
	NormalAirControl = CachedMoveComp->AirControl;
	NormalBrakingDeceleration = CachedMoveComp->BrakingDecelerationFalling;

	//Add Base Attribute set
	BaseAttributes = CreateDefaultSubobject<UBaseAttributes>(TEXT("BaseAttributes"));

	//Initialize Audio Components
	SpawnGlideAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("SpawnGlideAudioComp"));
	SpawnGlideAudioComp->SetupAttachment(RootComponent);
	SpawnGlideAudioComp->bAutoActivate = false; //Don't play on start

	RegularGlideAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("RegularGlideAudioComp"));
	RegularGlideAudioComp->SetupAttachment(RootComponent);
	RegularGlideAudioComp->bAutoActivate = false; //Don't play on start

	CurrentWindDirection = FVector::ZeroVector;



}

// Called when the game starts or when spawned
void ARod3ntPlayer::BeginPlay()
{
	Super::BeginPlay();

	CachedMoveComp = GetCharacterMovement(); //ensuring its cached 

	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	// Assign sounds to components if set in BP
	if (SpawnGlideSound)
	{
		SpawnGlideAudioComp->SetSound(SpawnGlideSound);
	}
	if (RegularGlideSound)
	{
		RegularGlideAudioComp->SetSound(RegularGlideSound);
	}

	//Starting the timer for optamizing stuff
	GetWorldTimerManager().SetTimer(ClimbingTraceTimerHandle, this, &ARod3ntPlayer::PerformClimbingTrace, 0.1f, true);

	//Start with Spawn Glide if set

	if (bStartwithSpawnGlide && !CachedMoveComp->IsMovingOnGround())
	{
		bIsSpawnGliding = true;

		// PLAY SOUND
		if (SpawnGlideAudioComp) SpawnGlideAudioComp->Play();

		StartGroundDistanceUpdates();
		UpdateGroundDistance();

		//Set the initial distance from ground
		SmoothenDistanceFromGround = DistanceFromGround;

		InitalDistancefromGround = FMath::Min(DistanceFromGround, MaxGroundDistance);

		//Set the Glide Physics
		CachedMoveComp->GravityScale = 0.0f;
		CachedMoveComp->BrakingDecelerationFalling = 0.0f;
		CachedMoveComp->AirControl = 0.0f;
	}
	
}

// Called every frame
void ARod3ntPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Stopping the Throw check by placing the bool check here instead
	if (bChargingThrow && bCanThrow)
	{
		ChargeThrow(DeltaTime);
	}

	if (bIsSpawnGliding)
	{
		// Smoothing and physics stay in Tick — must sync with movement component
		SmoothenDistanceFromGround = FMath::FInterpTo(SmoothenDistanceFromGround, DistanceFromGround, DeltaTime, SmoothingSpeed);
		HandleSpawnGlidePhysics(DeltaTime);
	}
	else if (bIsGliding)
	{
		GlideLogic(DeltaTime);
	}
}

void ARod3ntPlayer::PerformClimbingTrace()
{
	if (bIsClimbingTraceActive)
	{
		bool bCurrentHit = ClimbingTrace(WallHitResult);
		if (bIsFacingWall && !bCurrentHit && CachedMoveComp->IsFlying())
		{
			StopClimbing();
		}
		bIsFacingWall = bCurrentHit;
	}
	else if (CachedMoveComp->IsFalling() && !bIsSpawnGliding)
	{
		FHitResult AutoHit;
		if(ClimbingTrace(AutoHit))
		{
			if (bIsGliding) StopGliding();
			StartClimbingTrace();
			AutoClimbStart();
		}
	}
}


// Called to bind functionality to input
void ARod3ntPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARod3ntPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Server path: refresh ASC actor info after possession
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GrantAbilities(StartingAbilities);
	}
}

void ARod3ntPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Client path for replicated pawns
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

UAbilitySystemComponent* ARod3ntPlayer::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

TArray<FGameplayAbilitySpecHandle> ARod3ntPlayer::GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant)
{
	if(!AbilitySystemComponent || !HasAuthority())
	{
		return TArray<FGameplayAbilitySpecHandle>();
	}

	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;
	for (TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesToGrant)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(AbilityClass, 1, -1, this));
			GrantedAbilityHandles.Add(SpecHandle);
		}
	}
	SendAbilitiesChangedEvent();
	return GrantedAbilityHandles;
}

void ARod3ntPlayer::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove)
{
	if(!AbilitySystemComponent || !HasAuthority())
	{
		return;
	}
	for (FGameplayAbilitySpecHandle& SpecHandle : AbilityHandlesToRemove)
	{
		AbilitySystemComponent->ClearAbility(SpecHandle);
	}
	SendAbilitiesChangedEvent();
}

void ARod3ntPlayer::SendAbilitiesChangedEvent()
{
	FGameplayEventData EventData;	
	EventData.EventTag = FGameplayTag::RequestGameplayTag(FName("Event.AbilitiesChanged"));
	EventData.Instigator = this;
	EventData.Target = this;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);
}

void ARod3ntPlayer::ServerSendGameplayEventToSelf_Implementation(FGameplayEventData EventData)
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);
}

//Climbing Functions
bool ARod3ntPlayer::ClimbingTrace(FHitResult& OutHit) 
{
	//Starting and Ending points for the trace
	FVector Start = GetActorLocation();
	//Lifting the starting point
	Start.Z += 20.0f; //Adjust as needed to match character height

	FVector Forward = GetActorForwardVector();
	FVector End = Start + (Forward * ClimbingTraceDistance);

	//makeing sure trace doesnt hit itself
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	//Updated for sphere tracing now
	float Radius = 20.0f;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

	return GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere, TraceParams)
		&& FMath::Abs(OutHit.ImpactNormal.Z) < 0.3f; // Ensure it's a wall, not the ground or ceiling
}

void ARod3ntPlayer::StartClimbingTrace()
{
	bIsClimbingTraceActive = true;
}

void ARod3ntPlayer::StopClimbingTrace()
{
	bIsClimbingTraceActive = false;
	bIsFacingWall = false;
}


//Throwing Functions
void ARod3ntPlayer::ChargeThrow(float DeltaTime)
{
	if (bChargingThrow && bCanThrow)
	{
		float SpeedIncrease = DeltaTime * ThrowFactor;
		float NewSpeed = ThrowSpeed + SpeedIncrease;
		ThrowSpeed = FMath::Clamp(NewSpeed, 0.0f, ThrowChargeLimit);
	}
}

void ARod3ntPlayer::StartThrowCharge()
{
	bChargingThrow = true;
	ThrowSpeed = 0.0f;
}

void ARod3ntPlayer::ReleaseThrow()
{
	bChargingThrow = false;
	//Implement throw logic here, using ThrowSpeed variable
	ThrowSpeed = 0.0f;
	bCanThrow = false;
}

//Gliding Logic
void ARod3ntPlayer::GlideLogic(float DeltaTime)
{
	if (!bAmISquirrel) return;//Squirrel Only
	
	if(CachedMoveComp->IsMovingOnGround() || GlideTime >= GlideTimeLimit)
	{
		StopGliding();
		return;
	}
	GlideTime += DeltaTime;
}

void ARod3ntPlayer::StartGliding()
{
	if(bAmISquirrel && !CachedMoveComp->IsMovingOnGround() && !bIsGliding)
	{
		GlideTime = 0.0f;
		bIsGliding = true;

		CachedMoveComp->GravityScale = GlideGravityScale;
		CachedMoveComp->AirControl = GlideAirControl;

		// PLAY SOUND
		if (RegularGlideAudioComp) RegularGlideAudioComp->Play();

		//Air Brake
		FVector CurrentVelocity = CachedMoveComp->Velocity;
		CachedMoveComp->Velocity = FVector(CurrentVelocity.X, CurrentVelocity.Y,0.0f);
	}
}

void ARod3ntPlayer::StopGliding()
{
	bIsGliding = false;
	CachedMoveComp->GravityScale = NormalGravityScale;
	CachedMoveComp->AirControl = NormalAirControl;

	// STOP SOUND
	if (RegularGlideAudioComp) RegularGlideAudioComp->Stop();
}

void ARod3ntPlayer::SetWindVelocity(FVector NewWind)
{
	CurrentWindDirection = NewWind;
}


void ARod3ntPlayer::HandleSpawnGlidePhysics(float DeltaTime)
{
	if (CachedMoveComp->IsMovingOnGround())
	{
		//Landed
		HandleLanding();
		return;
	}

	//Getting Camera Direction
	FVector CameraDirection = GetControlRotation().Vector();

	//Get the direction from the camera and * by the spawn glide speed
	FVector NewVelocity = CameraDirection * SpawnGlideSpeed;

	//Constraint for looking down

	if(NewVelocity.Z > -SpawnMinDescentSpeed)
	{
		NewVelocity.Z = -SpawnMinDescentSpeed;
	}

	FVector FinalVelocity = NewVelocity + CurrentWindDirection;

	//Apply the new velocity
	CachedMoveComp->Velocity = FinalVelocity;

	//Might remove this but its to rotate the rodent to the dive direction
	FRotator TargetRotation = NewVelocity.Rotation();
	FRotator CurrentRotation = GetActorRotation();

	if (!CurrentRotation.Equals(TargetRotation, 1.0f))
	{
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, SpawnTurnInterpSpeed));
	}
}

void ARod3ntPlayer::UpdateGroundDistance()
{
	FVector Start = GetActorLocation();
	FVector End = Start - (FVector::UpVector * MaxGroundDistance);

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	//Trace to get the distance to ground
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, TraceParams);

	if(bHit)
	{
		DistanceFromGround = HitResult.Distance;
	}
	else
	{
		DistanceFromGround = MaxGroundDistance;
	}
}

void ARod3ntPlayer::StartGroundDistanceUpdates(float Rate)
{
	GetWorldTimerManager().SetTimer(GroundDistanceTimerHandle, this, &ARod3ntPlayer::UpdateGroundDistance, Rate, true);
}

void ARod3ntPlayer::StopGroundDistanceUpdates()
{
	GetWorldTimerManager().ClearTimer(GroundDistanceTimerHandle);
}

void ARod3ntPlayer::HandleLanding()
{
	bIsSpawnGliding = false;
	bIsGliding = false;
	
	StopGroundDistanceUpdates();
	if (SpawnGlideAudioComp) SpawnGlideAudioComp->Stop();
	if (RegularGlideAudioComp) RegularGlideAudioComp->Stop();

	if(CachedMoveComp)
	{
		CachedMoveComp->GravityScale = NormalGravityScale;
		CachedMoveComp->AirControl = NormalAirControl;
		CachedMoveComp->BrakingDecelerationFalling = NormalBrakingDeceleration;
	}

	CachedMoveComp->SetMovementMode(MOVE_Walking);

	FVector CurrentVelocity = CachedMoveComp->Velocity;
	CurrentVelocity.Z = 0.0f; // Remove vertical velocity on landing
	CachedMoveComp->Velocity = CurrentVelocity;

	//Making sure rotation is upright
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FRotator(0.0f, CurrentRotation.Yaw, 0.0f));
	CurrentWindDirection = FVector::ZeroVector;
}


