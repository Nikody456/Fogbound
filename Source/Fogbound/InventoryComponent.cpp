#include "InventoryComponent.h"
#include "UsableItem.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Конструктор
UInventoryComponent::UInventoryComponent()
{
    SetIsReplicatedByDefault(true);

    Inventory.SetNum(4);
}

void UInventoryComponent::AddItem(AUsableItem* NewItem)
{
    if (!NewItem || bFull) return;

    FInventoryItem Item;
    Item.ItemClass = NewItem->GetClass();
    Item.ItemName = NewItem->ItemName;
    Item.Icon = NewItem->Icon;

    for (int32 i = 0; i < Inventory.Num(); ++i)
    {
        if (Inventory[i].ItemClass == nullptr)
        {
            Inventory[i] = Item;

            OnItemAdded.Broadcast(Item.Icon, i);
            
            break;
        }
    }

    NewItem->Destroy();

    EquippedSlots++;
    if (EquippedSlots == 4)
        bFull = true;
}

void UInventoryComponent::RemoveItem()
{
    if (!Inventory.IsValidIndex(SelectedSlot)) return;
    
    Inventory[SelectedSlot] = FInventoryItem();

    OnItemRemoved.Broadcast(SelectedSlot);
    
    EquippedSlots--;

    SelectedSlot = -1;

    if (bFull && EquippedSlots < 4)
        bFull = false;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UInventoryComponent, SelectedSlot);
}

void UInventoryComponent::SetSelectedSlot()
{
    const bool bIsSelected = SelectedSlot != -1;
    
    OnSelectedSlotChanged.Broadcast(SelectedSlot, bIsSelected);
}

void UInventoryComponent::OnRep_SelectedSlot() const
{
    const bool bIsSelected = SelectedSlot != -1;
    OnSelectedSlotChanged.Broadcast(SelectedSlot, bIsSelected);
}
