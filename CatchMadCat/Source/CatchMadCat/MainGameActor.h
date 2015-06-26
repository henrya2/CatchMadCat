// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MainGameActor.generated.h"

USTRUCT()
struct CATCHMADCAT_API FGridPosition
{
	GENERATED_BODY()

	int32 RowIndex;
	
	int32 ColumnIndex;

	FGridPosition()
		: RowIndex(0)
		, ColumnIndex(0)
	{

	}

	FGridPosition(int32 NewRow, int32 NewColumn)
		: RowIndex(NewRow)
		, ColumnIndex(NewColumn)
	{

	}
};

UCLASS()
class CATCHMADCAT_API AMainGameActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainGameActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostInitializeComponents() override;

	void ClearGridItems();

	virtual void BeginDestroy() override;

	void MoveCat(int32 RowIndex, int32 ColumnIndex);

	class AGridItemActor* GetGridItem(int32 Row, int32 Column) const;

	TArray<FGridPosition> GetAvailableMoves(int32 Row, int32 Column);

	bool IsMovable(int32 Row, int32 Column) const;

protected:
	UFUNCTION()
	void HandleGridItemClicked(class AGridItemActor* GridItemActor);

	void SelectGridItem(class AGridItemActor* GridItemActor);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* GridScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UChildActorComponent* CatActorChildComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AGridItemActor> GridItemActorClass;

	TWeakObjectPtr<class ACatActor> CatActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 GridRowNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 GridColumnNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GridSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GridSpriteSize;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TWeakObjectPtr<class AGridItemActor>> GridItems;
};
