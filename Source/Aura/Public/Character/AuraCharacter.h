// Copyright HigeNeko

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	/** End Combat Interface */
	/** Player Interface */
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual int32 GetAttributesPointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual void LevelUp_Implementation() override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial = nullptr) override;
	virtual void HideMagicCircle_Implementation() override;
	/** End Player Interface */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	virtual void OnRep_Stunned() override;

protected:
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};
