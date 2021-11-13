// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

UENUM(BlueprintType)
enum class EState : uint8
{
	InProgress,
	FailedAttempt,
	Lost,
	Won
};

struct BullCows {
	int32 Bulls;
	int32 Cows;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;

	// Your declarations go below!
	private:
	FString HiddenWord;
	int32 Lives;
	EState State;
	TArray<FString> Isograms;
	void ResetGame();
	void PromptToPlayAgain();
	void DisplayVictoryScreen();
	void DisplayFailureScreen();
	static bool IsIsogram(const FString& Word);
	static bool IsValidWord(const FString& Word);

	BullCows GetBullCows(const FString& Word) const;
	
};
