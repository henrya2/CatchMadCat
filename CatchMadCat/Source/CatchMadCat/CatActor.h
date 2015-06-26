// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CatActor.generated.h"

UCLASS()
class CATCHMADCAT_API ACatActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACatActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 RowIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ColumnIndex;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPaperFlipbookComponent* FlipbookComponent;
};
