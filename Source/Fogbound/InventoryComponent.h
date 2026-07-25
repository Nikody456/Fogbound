#pragma once

#include "CoreMinimal.h"
#include "UsableItem.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAdded, UTexture2D*, Icon, int32, SlotNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, int32, SlotNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectedSlotChanged, int32, SlotNumber, bool, bIsSelected);

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AUsableItem> ItemClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FOGBOUND_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnItemRemoved OnItemRemoved;

	UPROPERTY(ReplicatedUsing = OnRep_Inventory, BlueprintReadWrite)
	TArray<FInventoryItem> Inventory;

	UPROPERTY(ReplicatedUsing = OnRep_SelectedSlot, BlueprintReadWrite)
	int32 SelectedSlot = -1;

	/** Server-only. No-ops without authority. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(AUsableItem* NewItem);

	/** Server-only. Prefer TryRemoveItem from client/UI. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem();

	/** Safe from client or server - routes remove to authority. */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void TryRemoveItem();

	UPROPERTY(BlueprintReadOnly)
	int EquippedSlots = 0;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnSelectedSlotChanged OnSelectedSlotChanged;

	UFUNCTION(BlueprintCallable)
	void SetSelectedSlot();

protected:
	UFUNCTION(Server, Reliable)
	void Server_RemoveItem();

private:
	bool bFull = false;

	/** Local mirror for OnRep diffing (not replicated). */
	TArray<FInventoryItem> PreviousInventory;

	bool OwnerHasAuthority() const;
	void NotifyInventoryDelta();

	UFUNCTION()
	void OnRep_Inventory();

	UFUNCTION()
	void OnRep_SelectedSlot() const;
};
