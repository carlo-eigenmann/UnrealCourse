/*
The game i s a simple guess the word game based on Mastermind.
This file contains the actual game logic and the default constructor for the main game instance.
No view code or direct user interaction.
*/

#pragma once

#include "FBullCowGame.h"
#include <map>

// to make syntax Unreal friendly
#define TMAP std::map
using int32 = int;

FBullCowGame::FBullCowGame() { Reset(); } // constructor

int32 FBullCowGame::GetDifficulty() const { return MyDifficulty; }
int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWordLength; }

void FBullCowGame::SetDifficulty(int32 Difficulty)
{
	MyDifficulty = Difficulty;
}

void FBullCowGame::SetHiddenWordLength(int32 HiddenWordLength)
{
	MyHiddenWordLength = HiddenWordLength;

	if (MyHiddenWordLength == 3)
	{
		MyHiddenWord = "pet";
	}
	else if (MyHiddenWordLength == 4)
	{
		MyHiddenWord = "link";
	}
	else if (MyHiddenWordLength == 5)
	{
		MyHiddenWord = "heart";
	}
	else if (MyHiddenWordLength == 6)
	{
		MyHiddenWord = "planet";
	}
	else if (MyHiddenWordLength == 7)
	{
		MyHiddenWord = "stumped";
	}
}

bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const
{
	TMAP<int32, int32> WordLengthToMaxTries{ {3,(5-MyDifficulty)}, {4,(8 - MyDifficulty)}, {5,(11 - MyDifficulty)}, {6,(16 - MyDifficulty)}, {7,(21 - MyDifficulty)} };
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

void FBullCowGame::Reset() 
{
	const FString HIDDEN_WORD = ""; // this MUST be an isogram
	MyHiddenWord = HIDDEN_WORD;
	MyCurrentTry = 1;
	bGameIsWon = false;
	return; 
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (!IsAlpha(Guess)) // if guess isn't an isogram
	{
		return EGuessStatus::Not_Alphabetic;
	}
	else if (!IsLowercase(Guess)) // if the guess isn't all lowercase
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (!IsIsogram(Guess)) // if guess isn't an isogram
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (Guess.length() != GetHiddenWordLength()) // if guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else // otherwise
	{
		return EGuessStatus::OK;
	}
}

// receives a VALID guess, increments  turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

	// loop through all letters in the hidden word
	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++)
	{
		// compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++) // compares letters against the hidden word
		{
			// if they match then
			if (Guess[GChar] == MyHiddenWord[MHWChar])
			{
				if (MHWChar == GChar) // if they're in the same place
				{
					BullCowCount.Bulls++; // increments bulls
				}
				else
				{
					BullCowCount.Cows++; // increments cows
				}
			}
		}
	}
	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}

bool FBullCowGame::IsAlpha(FString Guess) const
{
	for (auto Letter : Guess) // for all letters of the word
	{
		if (!isalpha(Letter)) // if not a alphabetical letter
		{
			return false;
		}
	}
	return true; // in case \0 is entered
}

bool FBullCowGame::IsIsogram(FString Guess) const
{
	// treat 0 and 1 letter words as isograms
	if (Guess.length() <= 1) { return true; }

	TMAP<char, bool> LetterRead; // set up the map
	for (auto Letter : Guess) // for all letters of the word
	{
		Letter = tolower(Letter); // handle mixed case
		if (LetterRead[Letter])	// if a letter is in the map
		{
			return false; // we do not have an isogram
		}
		else
		{
			LetterRead[Letter] = true; // add the letter to the map
		}
	}
	return true; // in cases were \0 is entered
}

bool FBullCowGame::IsLowercase(FString Guess) const
{
	for (auto Letter : Guess) // for all letters of the word
	{
			if (!islower(Letter)) // if not a lowercase letter
			{
				return false;
			}
	}
	return true; // in case \0 is entered
}
