#include "sidescrolling_actionCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include <GameFramework/Actor.h>
#include "GameFramework/Actor.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "projectile.h"


//allows me to use print() for debuging
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, TEXT(text))


//declaring all animation var's
Asidescrolling_actionCharacter::Asidescrolling_actionCharacter() : health(false), jumping(false), shooting(false), abletoshoot(true), 
sprintanimation(false)
{
	// et size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 750.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	//create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	//character movement component
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 10000.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void Asidescrolling_actionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// 
	/* basic ue4 jump settings *turned off*.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindTouch(IE_Released, this, &Asidescrolling_actionCharacter::TouchStopped);
	*/

	//bind for movement
	PlayerInputComponent->BindAxis("MoveRight", this, &Asidescrolling_actionCharacter::MoveRight);
	//bind for jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &Asidescrolling_actionCharacter::doublejump);
	//bind for sprint press
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &Asidescrolling_actionCharacter::sprinton);
	//bind for sprint release
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &Asidescrolling_actionCharacter::sprintoff);
	//bind for attack
	PlayerInputComponent->BindAction("attack1", IE_Pressed, this, &Asidescrolling_actionCharacter::attack);
}

//spawn projectile.cpp 
void Asidescrolling_actionCharacter::spawnactor()
{
		FActorSpawnParameters spawning;
		spawning.Owner = this;
		FRotator rotator = GetActorRotation();
		FVector spawnlocation = GetActorLocation() + GetCapsuleComponent()->GetForwardVector();
		GetWorld()->SpawnActor<Aprojectile>(ToSpawn, spawnlocation, rotator, spawning);

}

//move mechanic
void Asidescrolling_actionCharacter::MoveRight(float Value)
{
	// add movement in a direction
	//AddMovementInput(FVector(0.f,-1.f,0.f), Value);
	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

//sprint pressed
void Asidescrolling_actionCharacter::sprinton()
{
	//check if the player is on ground
	if (GetCharacterMovement()->IsWalking() == true)
	{
		sprintanimation = true;
		//apply faster movement
		GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	}
}

//sprint release
void Asidescrolling_actionCharacter::sprintoff()
{
	sprintanimation = false;
	//apply decreased movement
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

//doublejump bool initialization
bool allowdoublejump = NULL, diagonaljump = NULL;
FVector diagonalvector;
FVector upvectorrotation = diagonalvector.RotateAngleAxis(80, FVector(0, 0, 1));

void Asidescrolling_actionCharacter::doublejump()
{
	jumping = true;
	//if on the ground, allow jump()
	if (GetCharacterMovement()->IsWalking() == true)
	{
		//ue4 jump 
		Jump();
		//set allowdoublejump to true
		allowdoublejump = true;
	}
		//if off the ground and double jump bool conditions are met, allow a second impulse which acts as a second jump
		if (GetCharacterMovement()->IsWalking() == false && allowdoublejump == true)
		{
			//apply impulse in the up vector from the characters location to act as a second jump
			GetCharacterMovement()->AddImpulse(GetCapsuleComponent()->GetUpVector() * 1000, true);
			//set double jump bool to false
			allowdoublejump = false;
			diagonaljump = true;
		}
			if (GetCharacterMovement()->IsWalking() == false && diagonaljump == true)
			{
				GetCharacterMovement()->AddImpulse(GetCapsuleComponent()->GetUpVector()* upvectorrotation * 2500, true);
				diagonaljump = false;
			}	
}

//Begin Play
void Asidescrolling_actionCharacter::BeginPlay()
{
	Super::BeginPlay();

    //TIMER
	//GetWorld()->GetTimerManager().SetTimer(_loopTimerHandle, this, &Asidescrolling_actionCharacter::onTimerEnd, 2.f, false);

}

//attacking function. Var's are being sent to BP's and 'abletoshoot' will be set to true in BP's
void Asidescrolling_actionCharacter::attack()
{ 
	if (abletoshoot == true && shooting == false)
	{
		//start shooting animation
		abletoshoot = false;
		shooting = true;
		spawnactor();
	}
}

//ue4 function to get jump end
void Asidescrolling_actionCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{	
	StopJumping();
}

//eventick
void Asidescrolling_actionCharacter::Tick(float a) 
{

	if (GetCharacterMovement()->IsWalking() == true)
	{
		jumping = false;
	}

}

//timer ending function.
int Asidescrolling_actionCharacter::onTimerEnd()
{
	print("timer ended");
	return NULL;
}

