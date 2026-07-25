#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class FOGBOUND_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category = "Health")
	int32 Health = 3;

	UFUNCTION(BlueprintImplementableEvent)
	void OnRep_Health();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_UseMedKit();

	UFUNCTION(BlueprintCallable)
	void UseMedKit();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TryUseMedkit();

	/** Plays montage on this character's mesh on all machines. Safe from client or server. */
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayReplicatedMontage(UAnimMontage* Montage, float PlayRate = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StopReplicatedMontage(UAnimMontage* Montage, float BlendOutTime = 0.25f);

	FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	UFUNCTION(Server, Reliable)
	void Server_PlayMontage(UAnimMontage* Montage, float PlayRate);

	UFUNCTION(NetMulticast, Reliable)
	void MC_PlayMontage(UAnimMontage* Montage, float PlayRate);

	UFUNCTION(Server, Reliable)
	void Server_StopMontage(UAnimMontage* Montage, float BlendOutTime);

	UFUNCTION(NetMulticast, Reliable)
	void MC_StopMontage(UAnimMontage* Montage, float BlendOutTime);

	void PlayMontageLocal(UAnimMontage* Montage, float PlayRate);
	void StopMontageLocal(UAnimMontage* Montage, float BlendOutTime);
};
