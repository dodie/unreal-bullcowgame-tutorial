// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay()
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWords.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, UBullCowCartridge::IsValidWord);

    ResetGame();
}

void UBullCowCartridge::OnInput(const FString& Input)
{
    ClearScreen();

    if (State == EState::FailedAttempt)
    {
        if (Input == "y" || Input == "Y")
        {
            ResetGame();
            return;
        }
        else if (Input == "n" || Input == "N")
        {
            State = EState::Lost;
            DisplayFailureScreen();
            return;
        }
        else {
            PromptToPlayAgain();
            return;
        }
    }

    if (State == EState::Lost)
    {
        DisplayFailureScreen();
        return;
    }

    if (State == EState::Won)
    {
        DisplayVictoryScreen();
        return;
    }

    if (HiddenWord == Input)
    {
        State = EState::Won;
        DisplayVictoryScreen();
    }
    else
    {
        if (HiddenWord.Len() != Input.Len())
        {
            PrintLine(TEXT("Input (%s) has incorrect length, length is %i! Try again!"), *Input, HiddenWord.Len());
            return;
        }

        if (!IsIsogram(Input))
        {
            PrintLine(TEXT("Input (%s) is not an isogram! Try again!"), *Input);
            return;
        }

        Lives--;
        BullCows Result = GetBullCows(Input);

        PrintLine(TEXT("You got %i Bulls and %i Cows"), Result.Bulls, Result.Cows);
        PrintLine(TEXT("Remaining Lives:"));
        PrintLine(FString::FromInt(Lives));
        if (Lives == 0)
        {
            DisplayFailureScreen();
            State = EState::FailedAttempt;
            PromptToPlayAgain();
        }
    }
}

void UBullCowCartridge::DisplayVictoryScreen()
{
    ClearScreen();
    PrintLine(TEXT("You won! The hidden word was " + HiddenWord));
}

void UBullCowCartridge::DisplayFailureScreen()
{
    ClearScreen();
    PrintLine(TEXT("Game Over!"));
}

void UBullCowCartridge::PromptToPlayAgain()
{
    ClearScreen();
    PrintLine(TEXT("You lost! The solution was:" + HiddenWord));
    PrintLine(TEXT("Do you want to play another round? Y/N"));
}

void UBullCowCartridge::ResetGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    State = EState::InProgress;
    Lives = 5;

    PrintLine(TEXT("Welcome to Bulls Cows!"));

    //PrintLine(TEXT("DEBUG: %i, %s"), Isograms.Num(), *HiddenWord);

    PrintLine(TEXT(""));
    PrintLine(TEXT("Lives:"));
    PrintLine(FString::FromInt(Lives));
    PrintLine(TEXT("Length of word:"));
    PrintLine(FString::FromInt(HiddenWord.Len()));
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for (int32 i = 0; i < Word.Len(); i++)
    {
        for (int32 j = i + 1; j < Word.Len(); j++)
        {
            if (Word[i] == Word[j])
            {
                return false;
            }
        }
    }

    return true;
}

bool UBullCowCartridge::IsValidWord(const FString& Word)
{
    return IsIsogram(Word) && 4 <= Word.Len() && Word.Len() <= 8;
}


BullCows UBullCowCartridge::GetBullCows(const FString& Word) const
{
    int32 Bulls = 0;
    int32 Cows = 0;

    for (int32 i = 0; i < Word.Len(); i++)
    {
        if (Word[i] == HiddenWord[i])
        {
            Bulls++;
        }
        else
        {
            for (int32 j = 0; j < HiddenWord.Len(); j++)
            {
                if (Word[i] == HiddenWord[j])
                {
                    Cows++;
                    break;
                }
            }
        }
    }

    return {Bulls, Cows};
}
