// Fill out your copyright notice in the Description page of Project Settings.

#include "CatchMadCat.h"
#include "GridItemActor.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"


// Sets default values
AGridItemActor::AGridItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RenderSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("RenderSprite"));

	AssetPot1 = nullptr;
	AssetPot2 = nullptr;

	RowIndex = 0;
	ColumnIndex = 0;
	OffsetX = 0;
	OffsetZ = 0;

	bBlockPot = false;
}

// Called when the game starts or when spawned
void AGridItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridItemActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AGridItemActor::SetRowIndex(int32 NewRow)
{
	RowIndex = NewRow;
}

void AGridItemActor::SetColumnIndex(int32 NewColumn)
{
	ColumnIndex = NewColumn;
}

void AGridItemActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	float AddOffsetX = RowIndex % 2 == 0 ? ItemWidth * 0.5f : 0;

	SetActorRelativeLocation(FVector(OffsetX + ColumnIndex * ItemWidth + AddOffsetX, 0, OffsetZ + RowIndex * ItemHeight));

	OnClicked.AddDynamic(this, &AGridItemActor::HandleOnClicked);
	OnInputTouchEnd.AddDynamic(this, &AGridItemActor::HandleTouchEnd);
}

void AGridItemActor::HandleOnClicked()
{
	if (!IsPendingKill())
		OnClickedItem.Broadcast(this);
}

void AGridItemActor::HandleTouchEnd(ETouchIndex::Type FingerIndex)
{
	if (!IsPendingKill())
		OnClickedItem.Broadcast(this);
}

void AGridItemActor::SetBlockPot(bool bNewBlockPot)
{
	if (bBlockPot != bNewBlockPot)
	{
		bBlockPot = bNewBlockPot;

		if (bBlockPot)
		{
			RenderSprite->SetSprite(AssetPot2);
		}
		else
		{
			RenderSprite->SetSprite(AssetPot1);
		}
	}
}

