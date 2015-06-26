// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Delegate.h"
#include "GridItemActor.generated.h"

class AGridItemActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCustomActorOnClickedSignature, AGridItemActor*, GridItemActor);

UCLASS()
class CATCHMADCAT_API AGridItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridItemActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SetRowIndex(int32 NewRow);
	
	void SetColumnIndex(int32 NewColumn);

	int32 GetRowIndex() const { return RowIndex; }
	
	int32 GetColumnIndex() const { return ColumnIndex; }

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void HandleOnClicked();

	UFUNCTION()
	void HandleTouchEnd(ETouchIndex::Type FingerIndex);

	bool IsBlockPot() const { return bBlockPot; }

	void SetBlockPot(bool bNewBlockPot);

public:
	UPROPERTY(BlueprintAssignable, Category = "Input|Mouse Input")
	FCustomActorOnClickedSignature OnClickedItem;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RowIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ColumnIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ItemWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ItemHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OffsetZ;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bBlockPot;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UPaperSprite* AssetPot1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UPaperSprite* AssetPot2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPaperSpriteComponent* RenderSprite;
};
