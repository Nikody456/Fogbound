#include "UsableItem.h"
#include "BaseCharacter.h"
#include "InventoryComponent.h"
#include "Animation/AnimInstance.h"

AUsableItem::AUsableItem()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	SetReplicates(true);
	MeshComponent->SetIsReplicated(true);

	Icon = nullptr;
	ItemName = FText::GetEmpty();
}

void AUsableItem::BeginPlay()
{
	Super::BeginPlay();
}

void AUsableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUsableItem::MC_SetMeshCollision_Implementation(bool bEnableCollision)
{
	if (!MeshComponent)
	{
		return;
	}

	if (bEnableCollision)
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	}
	else
	{
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
}

void AUsableItem::Interact_Implementation(ABaseCharacter* InstigatorActor, bool bIsServerInteract)
{
	if (!InstigatorActor || bIsEquipped)
	{
		return;
	}

	if (bIsServerInteract)
	{
		if (UInventoryComponent* InventoryComp = InstigatorActor->FindComponentByClass<UInventoryComponent>())
		{
			InventoryComp->AddItem(this);
		}
	}
}

void AUsableItem::UpdateMeshCollision(bool bEnableCollision)
{
	if (HasAuthority())
	{
		MC_SetMeshCollision(bEnableCollision);
	}
}

void AUsableItem::SetPhysics(bool bNewState)
{
	if (MeshComponent)
	{
		MeshComponent->SetSimulatePhysics(bNewState);
	}
}

ABaseCharacter* AUsableItem::ResolveMontageCharacter(USkeletalMeshComponent* SkeletalMeshComp) const
{
	if (SkeletalMeshComp)
	{
		if (ABaseCharacter* FromMesh = Cast<ABaseCharacter>(SkeletalMeshComp->GetOwner()))
		{
			return FromMesh;
		}
	}

	if (ABaseCharacter* FromOwner = Cast<ABaseCharacter>(GetOwner()))
	{
		return FromOwner;
	}

	return Cast<ABaseCharacter>(GetAttachParentActor());
}

void AUsableItem::PlayInHandsMontage(USkeletalMeshComponent* SkeletalMeshComp, float PlayRate)
{
	if (!DefaultMontage)
	{
		return;
	}

	if (ABaseCharacter* Character = ResolveMontageCharacter(SkeletalMeshComp))
	{
		Character->PlayReplicatedMontage(DefaultMontage, PlayRate);
		return;
	}

	if (!SkeletalMeshComp)
	{
		return;
	}

	UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->Montage_Play(DefaultMontage, PlayRate);
}

void AUsableItem::StopInHandsMontage(USkeletalMeshComponent* SkeletalMeshComp, float BlendOutTime)
{
	if (!DefaultMontage)
	{
		return;
	}

	if (ABaseCharacter* Character = ResolveMontageCharacter(SkeletalMeshComp))
	{
		Character->StopReplicatedMontage(DefaultMontage, BlendOutTime);
		return;
	}

	if (!SkeletalMeshComp)
	{
		return;
	}

	UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->Montage_Stop(BlendOutTime, DefaultMontage);
}
