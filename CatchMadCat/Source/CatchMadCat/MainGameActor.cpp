// Fill out your copyright notice in the Description page of Project Settings.

#include "CatchMadCat.h"
#include "PaperSpriteComponent.h"
#include "MainGameActor.h"
#include "GridItemActor.h"
#include "CatActor.h"



// Sets default values
AMainGameActor::AMainGameActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridItemActorClass = nullptr;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	GridScene = CreateDefaultSubobject<USceneComponent>(TEXT("GridScene"));
	GridScene->AttachParent = RootComponent;
	GUIScene = CreateDefaultSubobject<USceneComponent>(TEXT("GUIScene"));
	GUIScene->AttachParent = RootComponent;
	GUIScene->RelativeLocation = FVector(0, 300, 0);

	StartSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("StartSprite"));
	StartSprite->AttachParent = GUIScene;

	StartSprite->OnInputTouchEnd.AddDynamic(this, &AMainGameActor::HandleStartSpriteTouched);

	ReplaySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ReplaySprite"));
	ReplaySprite->AttachParent = GUIScene;

	ReplaySprite->OnInputTouchEnd.AddDynamic(this, &AMainGameActor::HandleReplaySpriteTouched);

	VictorySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("VictorySprite"));
	VictorySprite->AttachParent = GUIScene;

	FailedSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("FailedSprite"));
	FailedSprite->AttachParent = GUIScene;

	CatActor = nullptr;
	CatActorClass = nullptr;

	GridRowNum = 9;
	GridColumnNum = 9;

	GridSize = 64;
	GridSpriteSize = 45;

	GamingState = EGameState::EGS_GamePrepared;
}

// Called when the game starts or when spawned
void AMainGameActor::BeginPlay()
{
	Super::BeginPlay();

	ReplaySprite->SetHiddenInGame(true);
	ReplaySprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	VictorySprite->SetHiddenInGame(true);
	VictorySprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FailedSprite->SetHiddenInGame(true);
	FailedSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainGameActor::StartGame()
{
	GamingState = EGameState::EGS_GameStarted;

	CatActor->SetActorHiddenInGame(false);

	int32 RandRow = FMath::RandRange(2, GridRowNum - 3);
	int32 RandColumn = FMath::RandRange(2, GridColumnNum - 3);

	MoveCat(RandRow, RandColumn);
}

void AMainGameActor::ReplayGame()
{
	ClearGridsStates();

	FailedSprite->SetHiddenInGame(true);
	VictorySprite->SetHiddenInGame(true);

	StartGame();
}

// Called every frame
void AMainGameActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AMainGameActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CatActor.IsValid())
	{
		CatActor->Destroy();
		CatActor = nullptr;
	}

	if (CatActorClass)
	{
		FTransform SpawnTM;
		auto TmpCatActor = (ACatActor*)UGameplayStatics::BeginSpawningActorFromClass(this, CatActorClass, SpawnTM);
		if (TmpCatActor)
		{
			TmpCatActor->AttachRootComponentTo(GridScene);
			UGameplayStatics::FinishSpawningActor(TmpCatActor, SpawnTM);
			CatActor = TmpCatActor;
			CatActor->SetActorHiddenInGame(true);
		}
	}

	ClearGridItems();

	if (!GridItemActorClass)
		return;

	float OffsetX = -((GridColumnNum - 1) * GridSize + GridSpriteSize) / 2 + GridSpriteSize / 2 - GridSize / 4;
	float OffsetZ = -((GridRowNum - 1) * GridSize + GridSpriteSize) / 2 + GridSpriteSize / 2;

	for (int32 i = 0; i < GridRowNum; ++i)
	{
		for (int32 j = 0; j < GridColumnNum; ++j)
		{
			FTransform SpawnTM;

			auto GridItemActor = (AGridItemActor*)UGameplayStatics::BeginSpawningActorFromClass(this, GridItemActorClass, SpawnTM, true, this);
			if (GridItemActor)
			{
				GridItemActor->SetRowIndex(i);
				GridItemActor->SetColumnIndex(j);
				GridItemActor->OffsetX = OffsetX;
				GridItemActor->OffsetZ = OffsetZ;
				GridItemActor->ItemWidth = GridSize;
				GridItemActor->ItemHeight = GridSize;

				GridItemActor->AttachRootComponentTo(GridScene);
				UGameplayStatics::FinishSpawningActor(GridItemActor, SpawnTM);

				GridItemActor->OnClickedItem.AddDynamic(this, &AMainGameActor::HandleGridItemClicked);

				GridItems.Add(GridItemActor);
			}
		}
	}
}

void AMainGameActor::ClearGridItems()
{
	for (int32 i = 0; i < GridItems.Num(); ++i)
	{
		if (GridItems[i].IsValid())
			GridItems[i]->Destroy();
	}

	GridItems.Empty();
}

void AMainGameActor::BeginDestroy()
{
	if (CatActor.IsValid())
	{
		CatActor->Destroy();
		CatActor = nullptr;
	}
	ClearGridItems();

	Super::BeginDestroy();
}

void AMainGameActor::MoveCat(int32 RowIndex, int32 ColumnIndex)
{
	if (CatActor.IsValid())
	{
		AGridItemActor* GridItemActor = GetGridItem(RowIndex, ColumnIndex);
		if (GridItemActor)
		{
			FVector GridLocation = GridItemActor->GetRootComponent()->RelativeLocation;
			FVector NewLocation(GridLocation.X, 100, GridLocation.Z);
			CatActor->SetActorRelativeLocation(NewLocation);
			CatActor->RowIndex = GridItemActor->RowIndex;
			CatActor->ColumnIndex = GridItemActor->ColumnIndex;
		}
	}
}

class AGridItemActor* AMainGameActor::GetGridItem(int32 Row, int32 Column) const
{
	check(Row >= 0 && Row < GridRowNum && Column >= 0 && Column < GridColumnNum);

	return GridItems[Row * GridColumnNum + Column].Get();
}

TArray<FGridPosition> AMainGameActor::GetAvailableMoves(int32 Row, int32 Column)
{
	TArray<FGridPosition> GridPosition;

	int RowTmp = 0;
	int ColumnTmp = 0;

	//left
	RowTmp = Row;
	ColumnTmp = Column - 1;
	if (IsMovable(RowTmp, ColumnTmp))
		GridPosition.Push(FGridPosition(RowTmp, ColumnTmp));

	// right
	RowTmp = Row;
	ColumnTmp = Column + 1;
	if (IsMovable(RowTmp, ColumnTmp))
		GridPosition.Push(FGridPosition(RowTmp, ColumnTmp));

	// top
	RowTmp = Row + 1;
	ColumnTmp = Column;
	if (IsMovable(RowTmp, ColumnTmp))
		GridPosition.Push(FGridPosition(RowTmp, ColumnTmp));

	// bottom
	RowTmp = Row - 1;
	ColumnTmp = Column;
	if (IsMovable(RowTmp, ColumnTmp))
		GridPosition.Push(FGridPosition(RowTmp, ColumnTmp));

	// odd row topLeft or even row topRight
	RowTmp = Row + 1;
	if (Row % 2 != 0)
	{
		ColumnTmp = Column - 1;
	}
	else
	{
		ColumnTmp = Column + 1;
	}
	if (IsMovable(RowTmp, ColumnTmp))
		GridPosition.Push(FGridPosition(RowTmp, ColumnTmp));

	// odd row bottomLeft or even row bottomRight
	RowTmp = Row - 1;
	if (Row % 2 != 0)
	{
		ColumnTmp = Column - 1;
	}
	else
	{
		ColumnTmp = Column + 1;
	}
	if (IsMovable(RowTmp, ColumnTmp))
		GridPosition.Push(FGridPosition(RowTmp, ColumnTmp));

	return GridPosition;
}

bool AMainGameActor::IsMovable(int32 Row, int32 Column) const
{
	if (Row < 0 
		|| Column < 0
		|| Row >= GridRowNum 
		|| Column >= GridColumnNum)
		return false;

	AGridItemActor* GridItem = GetGridItem(Row, Column);

	return !GridItem->IsBlockPot();
}

bool AMainGameActor::IsEscaped(int32 Row, int32 Column) const
{
	if (Row == 0 || Column == 0 || Row == GridRowNum - 1 || Column == GridColumnNum - 1)
	{
		return true;
	}

	return false;
}

void AMainGameActor::HandleGridItemClicked(class AGridItemActor* GridItemActor)
{
	if (GamingState != EGameState::EGS_GameStarted)
		return;

	SelectGridItem(GridItemActor);
}

void AMainGameActor::HandleStartSpriteTouched(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	StartSprite->SetHiddenInGame(true);
	StartSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	StartGame();
}

void AMainGameActor::HandleReplaySpriteTouched(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	ReplaySprite->SetHiddenInGame(true);
	ReplaySprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ReplayGame();
}

void AMainGameActor::SelectGridItem(class AGridItemActor* GridItemActor)
{
	if (GridItemActor && !GridItemActor->IsBlockPot() && CatActor.IsValid())
	{
		if (CatActor->RowIndex != GridItemActor->RowIndex
			|| CatActor->ColumnIndex != GridItemActor->ColumnIndex)
		{
			GridItemActor->SetBlockPot(true);

			if (IsEscaped(CatActor->RowIndex, CatActor->ColumnIndex))
			{
				GotoFailed();
				return;
			}

			TArray<FGridPosition> AvailableMoves = GetAvailableMoves(CatActor->RowIndex, CatActor->ColumnIndex);

			if (AvailableMoves.Num() > 0)
			{
				int32 RandIndex = FMath::RandRange(0, AvailableMoves.Num() - 1);
				MoveCat(AvailableMoves[RandIndex].RowIndex, AvailableMoves[RandIndex].ColumnIndex);
			}
			else
			{
				GotoVictory();

				return;
			}
		}
	}
}

void AMainGameActor::ClearGridsStates()
{
	for (int i = 0; i < GridItems.Num(); ++i)
	{
		if (GridItems[i].IsValid())
		{
			GridItems[i]->SetBlockPot(false);
		}
	}
}

void AMainGameActor::GotoFailed()
{
	DoEndGame();

	FailedSprite->SetHiddenInGame(false);
}

void AMainGameActor::GotoVictory()
{
	DoEndGame();

	VictorySprite->SetHiddenInGame(false);
}

void AMainGameActor::DoEndGame()
{
	GamingState = EGameState::EGS_GameEnded;
	ClearGridsStates();

	if (CatActor.IsValid())
	{
		CatActor->SetActorHiddenInGame(true);
	}
	ReplaySprite->SetHiddenInGame(false);
	ReplaySprite->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
