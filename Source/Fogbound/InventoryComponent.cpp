#include "InventoryComponent.h"
#include "UsableItem.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

UInventoryComponent::UInventoryComponent()
{
	SetIsReplicatedByDefault(true);

	Inventory.SetNum(4);
	PreviousInventory.SetNum(4);
}

bool UInventoryComponent::OwnerHasAuthority() const
{
	const AActor* Owner = GetOwner();
	return Owner && Owner->HasAuthority();
}

void UInventoryComponent::NotifyInventoryDelta()
{
	const int32 Num = FMath::Max(Inventory.Num(), PreviousInventory.Num());
	for (int32 i = 0; i < Num; ++i)
	{
		const bool bHadItem = PreviousInventory.IsValidIndex(i) && PreviousInventory[i].ItemClass != nullptr;
		const bool bHasItem = Inventory.IsValidIndex(i) && Inventory[i].ItemClass != nullptr;

		if (!bHadItem && bHasItem)
		{
			OnItemAdded.Broadcast(Inventory[i].Icon, i);
		}
		else if (bHadItem && !bHasItem)
		{
			OnItemRemoved.Broadcast(i);
		}
		else if (bHadItem && bHasItem && PreviousInventory[i].ItemClass != Inventory[i].ItemClass)
		{
			OnItemRemoved.Broadcast(i);
			OnItemAdded.Broadcast(Inventory[i].Icon, i);
		}
	}

	PreviousInventory = Inventory;
}

void UInventoryComponent::AddItem(AUsableItem* NewItem)
{
	if (!OwnerHasAuthority() || !NewItem || bFull)
	{
		return;
	}

	FInventoryItem Item;
	Item.ItemClass = NewItem->GetClass();
	Item.ItemName = NewItem->ItemName;
	Item.Icon = NewItem->Icon;

	bool bAdded = false;
	for (int32 i = 0; i < Inventory.Num(); ++i)
	{
		if (Inventory[i].ItemClass == nullptr)
		{
			Inventory[i] = Item;
			bAdded = true;
			break;
		}
	}

	if (!bAdded)
	{
		return;
	}

	NewItem->Destroy();

	EquippedSlots++;
	if (EquippedSlots == 4)
	{
		bFull = true;
	}

	NotifyInventoryDelta();
}

void UInventoryComponent::RemoveItem()
{
	if (!OwnerHasAuthority())
	{
		return;
	}

	if (!Inventory.IsValidIndex(SelectedSlot) || Inventory[SelectedSlot].ItemClass == nullptr)
	{
		return;
	}

	Inventory[SelectedSlot] = FInventoryItem();

	EquippedSlots--;
	SelectedSlot = -1;

	if (bFull && EquippedSlots < 4)
	{
		bFull = false;
	}

	NotifyInventoryDelta();
}

void UInventoryComponent::TryRemoveItem()
{
	if (OwnerHasAuthority())
	{
		RemoveItem();
	}
	else
	{
		Server_RemoveItem();
	}
}

void UInventoryComponent::Server_RemoveItem_Implementation()
{
	RemoveItem();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventoryComponent, Inventory, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UInventoryComponent, SelectedSlot, COND_OwnerOnly);
}

void UInventoryComponent::SetSelectedSlot()
{
	const bool bIsSelected = SelectedSlot != -1;
	OnSelectedSlotChanged.Broadcast(SelectedSlot, bIsSelected);
}

void UInventoryComponent::OnRep_Inventory()
{
	NotifyInventoryDelta();
}

void UInventoryComponent::OnRep_SelectedSlot() const
{
	const bool bIsSelected = SelectedSlot != -1;
	OnSelectedSlotChanged.Broadcast(SelectedSlot, bIsSelected);
}
