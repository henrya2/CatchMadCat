// Fill out your copyright notice in the Description page of Project Settings.

#include "CatchMadCat.h"
#include "CatActor.h"
#include "PaperFlipbookComponent.h"


// Sets default values
ACatActor::ACatActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	RootComponent = FlipbookComponent;

	RowIndex = 0;
	ColumnIndex = 0;
}

// Called when the game starts or when spawned
void ACatActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACatActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

