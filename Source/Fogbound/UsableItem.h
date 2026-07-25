#pragma once

#include "CoreMinimal.h"
#include "Interact.h"
#include "Use.h"
#include "GameFramework/Actor.h"
#include "UsableItem.generated.h"

UCLASS()
class FOGBOUND_API AUsableItem : public AActor, public IInteract, public IUse
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;

public:
	AUsableItem();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Collision")
	void UpdateMeshCollision(bool bEnableCollision);

	UFUNCTION(NetMulticast, Reliable)
	void MC_SetMeshCollision(bool bEnableCollision);
	void MC_SetMeshCollision_Implementation(bool bEnableCollision);

	virtual void Interact_Implementation(ABaseCharacter* InstigatorActor, bool bIsServerInteract) override;

	UFUNCTION(BlueprintCallable)
	void SetPhysics(bool bNewState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	UAnimMontage* DefaultMontage = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayInHandsMontage(USkeletalMeshComponent* SkeletalMeshComp, float PlayRate = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StopInHandsMontage(USkeletalMeshComponent* SkeletalMeshComp, float BlendOutTime = 0.25f);

private:
	UPROPERTY(BlueprintReadWrite, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool bIsEquipped = false;

	ABaseCharacter* ResolveMontageCharacter(USkeletalMeshComponent* SkeletalMeshComp) const;
};
