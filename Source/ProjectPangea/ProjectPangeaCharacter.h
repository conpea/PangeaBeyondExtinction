// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TestWorldClass.h"
#include "GameFramework/Character.h"
#include "ProjectPangeaCharacter.generated.h"

class USkeletalMeshComponent;
class UBipedIKAnim;

UCLASS(config=Game)
class AProjectPangeaCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AProjectPangeaCharacter();

  //Meshes
  UPROPERTY(VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent *sk_cape_;
  UPROPERTY(VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent *sk_chest_;
  UPROPERTY(VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent *sk_bracers_;
  UPROPERTY(VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent *sk_gloves_;
  UPROPERTY(VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent *sk_pants_;
  UPROPERTY(VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent *sk_shoes_;
  UPROPERTY(VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent *sk_eyebrows_;
  UPROPERTY(VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent *sk_beard_;
  /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float forward_speed_;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float side_speed_;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool focused_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Walking")
    float walking_speed_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Walking")
    float walking_rotation_speed_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Walking")
    float walking_friction_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Running")
    float running_speed_;
  
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Walking")
    float running_friction_;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement | Walking")
    float running_rotation_speed_;


  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float crouching_speed_;

  bool attacking;

  UPROPERTY(BlueprintReadOnly, Category = "Movement")
  bool running;
  FTimerHandle attack_timer_;

  //Extra meshes (later on component)
  
  //To identify cloth meshes
  enum ClothingItem {
    kCape = 0,
    kChest = 1, //Examples
    kBracers = 2,
    kGloves = 3,
    kPants = 4,
    kShoes = 5,
    kEyebrows = 6,
    kBeard = 7, //more to be added later
  };

  const int num_cloth_elements_ = 8;

  TArray<USkeletalMeshComponent *> clothing;
  UBipedIKAnim *anim_instance_;
protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

  virtual void BeginPlay() override;

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
  
  UFUNCTION()
    void Focus();

  UFUNCTION()
    void UnFocus();

  UFUNCTION()
    void Die();

  UFUNCTION()
    void Attack();

  UFUNCTION()
    void EndAttack();

  UFUNCTION()
    void SwitchCrouched();

  UFUNCTION()
    void Run();

  UFUNCTION()
    void StopRun();

  UFUNCTION()
      void TestSave();

  UFUNCTION()
      void TestLoad();

  UFUNCTION()
    void Zoom(float value);
private:

  bool dead;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

