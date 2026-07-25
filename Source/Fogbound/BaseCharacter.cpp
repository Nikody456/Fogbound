#include "BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

ABaseCharacter::ABaseCharacter()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	PrimaryActorTick.bCanEverTick = true;

	if (HasAuthority())
	{
		Health = 3;
	}
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::TryUseMedkit()
{
	if (HasAuthority())
	{
		UseMedKit();
	}
	else
	{
		Server_UseMedKit();
	}
}

void ABaseCharacter::Server_UseMedKit_Implementation()
{
	UseMedKit();
}

void ABaseCharacter::UseMedKit()
{
	Health = FMath::Clamp(Health + 1, 0, MaxHealth);
}

void ABaseCharacter::PlayMontageLocal(UAnimMontage* Montage, float PlayRate)
{
	if (!Montage)
	{
		return;
	}

	USkeletalMeshComponent* SkelMesh = GetMesh();
	if (!SkelMesh)
	{
		return;
	}

	UAnimInstance* AnimInstance = SkelMesh->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->Montage_Play(Montage, PlayRate);
}

void ABaseCharacter::StopMontageLocal(UAnimMontage* Montage, float BlendOutTime)
{
	if (!Montage)
	{
		return;
	}

	USkeletalMeshComponent* SkelMesh = GetMesh();
	if (!SkelMesh)
	{
		return;
	}

	UAnimInstance* AnimInstance = SkelMesh->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->Montage_Stop(BlendOutTime, Montage);
}

void ABaseCharacter::PlayReplicatedMontage(UAnimMontage* Montage, float PlayRate)
{
	if (!Montage)
	{
		return;
	}

	if (HasAuthority())
	{
		MC_PlayMontage(Montage, PlayRate);
	}
	else
	{
		Server_PlayMontage(Montage, PlayRate);
	}
}

void ABaseCharacter::StopReplicatedMontage(UAnimMontage* Montage, float BlendOutTime)
{
	if (!Montage)
	{
		return;
	}

	if (HasAuthority())
	{
		MC_StopMontage(Montage, BlendOutTime);
	}
	else
	{
		Server_StopMontage(Montage, BlendOutTime);
	}
}

void ABaseCharacter::Server_PlayMontage_Implementation(UAnimMontage* Montage, float PlayRate)
{
	MC_PlayMontage(Montage, PlayRate);
}

void ABaseCharacter::MC_PlayMontage_Implementation(UAnimMontage* Montage, float PlayRate)
{
	PlayMontageLocal(Montage, PlayRate);
}

void ABaseCharacter::Server_StopMontage_Implementation(UAnimMontage* Montage, float BlendOutTime)
{
	MC_StopMontage(Montage, BlendOutTime);
}

void ABaseCharacter::MC_StopMontage_Implementation(UAnimMontage* Montage, float BlendOutTime)
{
	StopMontageLocal(Montage, BlendOutTime);
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, Health);
}
