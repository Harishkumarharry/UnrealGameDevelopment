// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("!!!Welcome to Bull Cow Game!!!"));

    Isograms = GetValidHiddenWords(HiddenWordNames);
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len()*2;
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Total Lives %i."), Lives);
    // PrintLine(TEXT("Word: %s"), *HiddenWord);
    PrintLine(TEXT("Type to guess and \npress enter to continue....."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again....."));
}

void UBullCowCartridge::ProcessGuess(const FString &Guess)
{
    FBullCowCount Score = GetBullCows(Guess);

    --Lives;
    if (Lives > 0)
    {
        if (Guess == HiddenWord)
        {
            ClearScreen();
            PrintLine(TEXT("!!!Congratulations You Win!!!"));
            EndGame();
            return;
        }

        if (Guess.Len() != HiddenWord.Len())
        {
            PrintLine(TEXT("Sorry, try guessing again! \nLives left: %i."), Lives);
            PrintLine(TEXT("The Hidden Word is %i Characters long."), HiddenWord.Len());
            return;
        }

        if (!IsGuessWordCheck(Guess))
        {
            PrintLine(TEXT("Sorry, try guessing again!"));
            PrintLine(TEXT("Lives left: %i"), Lives);
            PrintLine(TEXT("The Hidden Word does not have \nrepeated characters in it."));
            return;
        }
        PrintLine(TEXT("Guess word is wrong."));
        PrintLine(TEXT("You lost a life!"));

        PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

        PrintLine(TEXT("Lives left: %i."), Lives);
        return;
    }

    ClearScreen();
    PrintLine(TEXT("You have no lives left!"));
    PrintLine(TEXT("The hidden word is %s."), *HiddenWord);
    EndGame();
    return;
}

bool UBullCowCartridge::IsGuessWordCheck(const FString &GuessWord) const
{
    for (int32 i = 0; i < GuessWord.Len(); i++)
    {
        for (int32 j = i + 1; j < GuessWord.Len(); j++)
        {
            if (GuessWord[i] == GuessWord[j])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidHiddenWords(const TArray<FString> &WordList) const
{
    TArray<FString> FilteredHiddenWord;
    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsGuessWordCheck(Word))
        {
            FilteredHiddenWord.Emplace(Word);
        }
    }

    return FilteredHiddenWord;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Guess) const
{    
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}