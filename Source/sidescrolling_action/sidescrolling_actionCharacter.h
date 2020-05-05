#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "sidescrolling_actionCharacter.generated.h"

UCLASS(config=Game)
class Asidescrolling_actionCharacter : public ACharacter
{
	GENERATED_BODY()

	//side view camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	//camera boom positioning the camera beside the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class Aprojectile> ToSpawn;


	void spawnactor();

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	//doublejump function
	void doublejump();
	//sprint on press
	void sprinton();
	//sprint on release
	void sprintoff();

	void BeginPlay();
	//attack function
	void attack();

	virtual void Tick(float DeltaSeconds) override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface


public:
	Asidescrolling_actionCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION()
	int onTimerEnd();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool health;

	//jumpanimation
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool jumping;

	//shooting animation
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool shooting;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool abletoshoot;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool sprintanimation;

private:
	FTimerHandle _loopTimerHandle;
};
