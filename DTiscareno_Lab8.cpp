/*------------------------------------------------------*
*CREATED BY:											*
*Name: David Tiscareno									*
*Class: GSP480											*
*Week 8 Lab Assignment									*
*Date: 8/26/13											*
*DTiscareno_Lab8.cpp									*
*-------												*
*This file contains a blackjack game along with a Monte-*
*Carlo simulator.										*
*-------------------------------------------------------*/
#include <iostream>
#include <random>
#include <vector>
#include <time.h>
using namespace std;

int DrawCard(); //Draws a card for a player
int GameWin(vector<int> playerCards, vector<int> dealerCards); //Checks to see if a game is won
bool DealerHit(vector<int> dealerCards); //Checks the rules to see if the dealer will hit
bool TwentyOne(vector<int> cards); //Checks to see if a player has 21
bool Bust(vector<int> cards); //Checks to see if a player has busted
void PrintGame(vector<int> playerCards, vector<int> dealerCards); //Prints the regular game board
void PrintFull(vector<int> playerCards, vector<int> dealerCards); //Prints the game board, showing the dealer's cards
void MonteCarlo(vector<int> playerCards, vector<int> dealerCards, int runs); //Runs the Monte-Carlo simulation

int main()
{
	srand(time(NULL)); //seed the random generator
	bool quit = false;
	cout << "Welcome to David Tiscareno's Blackjack Game!" << endl;
	while (!quit) //Start the game loop
	{
		int play;
		//Ask the player if they want to play
		cout << "Would you like to play a game? (1 for Yes, 0 for No): ";
		cin >> play;
		if (play == 1) //Play the game
		{
			vector<int> playerCards; //The player's cards
			vector<int> dealerCards; //The dealer's cards

			//Deal out the opening hands
			dealerCards.push_back(DrawCard());
			dealerCards.push_back(DrawCard());
			playerCards.push_back(DrawCard());
			playerCards.push_back(DrawCard());

			bool gameOver = false;
			cout << endl << endl << "Initial Deal:" << endl;
			while (!gameOver) //As long as the game hasn't ended
			{
				char hitStand;
				PrintGame(playerCards, dealerCards); //Print the game board
				if (Bust(playerCards)) //If you have busted, game over
				{
					cout << endl << endl << "I'm sorry. You busted and lost the game." << endl << endl;
					gameOver = true;
				}
				else if (TwentyOne(playerCards)) //If you have 21, automatically stand
				{
					//Show the entire board
					cout << endl << endl << "The dealer shows his hand . . ." << endl << endl;
					PrintFull(playerCards, dealerCards);
					//Check to see if the dealer will hit
					bool hit = DealerHit(dealerCards);
					while (hit == true) //The dealer keeps hitting as long as the rules allow
					{
						if (playerCards.size() != 2) //If the player has a blackjack, the game ends without the dealer hitting
						{
							cout << endl << endl << "The dealer hits . . ." << endl << endl;
							dealerCards.push_back(DrawCard());
							hit = DealerHit(dealerCards);
							PrintFull(playerCards, dealerCards);
						}
						else
							hit = false;
					}
					if (TwentyOne(dealerCards)) //If the dealer hits 21 . . .
					{
						if (playerCards.size() == 2 && dealerCards.size() == 2) //If both have blackjack
							cout << endl << endl << "You both have blackjack. This is a push (tie)." << endl << endl;
						else if (playerCards.size() == 2 && dealerCards.size() != 2) //If only the player has blackjack
							cout << endl << endl << "You have blackjack! Congratulations! You win!" << endl << endl;
						else if (playerCards.size() != 2 && dealerCards.size() == 2) //If only the dealer has blackjack
							cout << endl << endl << "I'm sorry. The dealer has blackjack and you lost the game." << endl << endl;
						else //If neither has blackjack
							cout << endl << endl << "You both have the same total. This is a push (tie)." << endl << endl;
						gameOver = true;
					}
					else
					{
						//If you have blackjack and the dealer does not even have 21 . . .
						if (playerCards.size() == 2)
							cout << endl << endl << "You have blackjack! Congratulations! You win!" << endl << endl;
						else
						{
							if (Bust(dealerCards)) //If the dealer busts . . .
								cout << endl << endl << "The dealer busts . . . ";
							else //Otherwise
								cout << endl << endl << "The dealer stands . . . ";
							cout << "Congratulations! You win!" << endl << endl;
						}
						gameOver = true;
					}
				}
				else
				{
					//Run the simulation
					MonteCarlo(playerCards, dealerCards, 1000);

					//Ask the player what he wants to do
					cout << endl << endl << "What do you want to do? (H/S) ";
					cin >> hitStand;
					if (hitStand == 'H' || hitStand == 'h') //The player hits
					{
						cout << endl << "The player hits . . ." << endl;
						playerCards.push_back(DrawCard());
					}
					else if (hitStand == 'S' || hitStand == 's') //The player stands
					{
						//Show the entire board
						cout << endl << "The player stands and the dealer shows his hand . . ." << endl;
						PrintFull(playerCards, dealerCards);
						//Check to see if the dealer will hit
						bool hit = DealerHit(dealerCards);
						while (hit == true) //The dealer keeps hitting as long as the rules allow
						{
							cout << endl << endl << "The dealer hits . . ." << endl;
							dealerCards.push_back(DrawCard());
							hit = DealerHit(dealerCards);
							PrintFull(playerCards, dealerCards);
						}
						if (TwentyOne(dealerCards)) //If the dealer hits 21 . . .
						{
							if (dealerCards.size() == 2) //If only the dealer has blackjack
								cout << endl << endl << "I'm sorry. The dealer has blackjack and you lost the game." << endl << endl;
							else //Otherwise
								cout << endl << endl << "I'm sorry. The dealer has hit 21 and you lost the game." << endl << endl;
							gameOver = true;
						}
						else
						{
							if (Bust(dealerCards)) //If the dealer busts
								cout << endl << endl << "The dealer busts . . . Congratulations! You win!" << endl << endl;
							else
							{
								cout << endl << endl << "The dealer stands . . . ";
								//Check to see who won the game
								int result = GameWin(playerCards, dealerCards);
								if (result == 1)
									cout << "Congratulations! You win!" << endl << endl;
								else if (result == 0)
									cout << "I'm sorry. You lost the game." << endl << endl;
								else
									cout << "You both have the same total. This is a push (tie)." << endl << endl;
							}
							gameOver = true;
						}
					}
					else
						cout << endl << "Please specify H or S." << endl;
				}
			}
		}
		else if (play == 0) //Quit
			quit = true;
		else //Try again
			cout << endl << "I'm sorry, you must respond with \"1\" or \"0\"." << endl;
	}
}

//DrawCard:
//Draws a card for a player
int DrawCard()
{
	int cardNum;
	cardNum = (rand() % 13) + 1;
	return cardNum;
}

//GameWin
//Checks to see if a game is won
//Returns 1 if player wins, 0 if player lost, and -1 if there is a tie
int GameWin(vector<int> playerCards, vector<int> dealerCards)
{
	int playerSum = 0;
	int dealerSum = 0;
	//Add player cards together
	for (int i = 0; i < playerCards.size(); i++)
	{
		playerSum += playerCards[i];
		//Check for Face Cards
		if (playerCards[i] == 11)
			playerSum -= 1;
		else if (playerCards[i] == 12)
			playerSum -= 2;
		else if (playerCards[i] == 13)
			playerSum -= 3;
	}
	//Check for Aces
	for (int i = 0; i < playerCards.size(); i++)
	{
		if (playerCards[i] == 1 && playerSum <= 11)
			playerSum += 10; //Add the extra ten from an ace, if it is helpful
	}
	//Add dealer cards together
	for (int i = 0; i < dealerCards.size(); i++)
	{
		dealerSum += dealerCards[i];
		//Check for Face Cards
		if (dealerCards[i] == 11)
			dealerSum -= 1;
		else if (dealerCards[i] == 12)
			dealerSum -= 2;
		else if (dealerCards[i] == 13)
			dealerSum -= 3;
	}
	//Check for Aces
	for (int i = 0; i < dealerCards.size(); i++)
	{
		if (dealerCards[i] == 1 && dealerSum <= 11)
			dealerSum += 10; //Add the extra ten from an ace, if it is helpful
	}
	if (playerSum > dealerSum)
		return 1; //Win
	else if (dealerSum > playerSum)
		return 0; //Lose
	else
		return -1; //Tie
}

//DealerHit
//Check to see if the dealer would hit according to the rules
bool DealerHit(vector<int> dealerCards)
{
	int dealerSum = 0;
	//Add dealer cards together
	for (int i = 0; i < dealerCards.size(); i++)
	{
		dealerSum += dealerCards[i];
		//Check for Face Cards
		if (dealerCards[i] == 11)
			dealerSum -= 1;
		else if (dealerCards[i] == 12)
			dealerSum -= 2;
		else if (dealerCards[i] == 13)
			dealerSum -= 3;
	}
	//Check for Aces
	for (int i = 0; i < dealerCards.size(); i++)
	{
		if (dealerCards[i] == 1 && dealerSum <= 11)
			dealerSum += 10; //Add the extra ten from an ace, if it is helpful
	}
	//Stop hitting at 17
	if (dealerSum > 16)
		return false;
	else
		return true;
}

//TwentyOne
//Checks to see if a player has 21
bool TwentyOne(vector<int> cards)
{
	int sum = 0;
	//Add cards together
	for (int i = 0; i < cards.size(); i++)
	{
		sum += cards[i];
		//Check for Face Cards
		if (cards[i] == 11)
			sum -= 1;
		else if (cards[i] == 12)
			sum -= 2;
		else if (cards[i] == 13)
			sum -= 3;
	}
	//Check for Aces
	for (int i = 0; i < cards.size(); i++)
	{
		if (cards[i] == 1 && sum <= 11)
			sum += 10; //Add the extra ten from an ace, if it is helpful
	}
	//Return whether or not it equals 21
	if (sum == 21)
		return true;
	else
		return false;
}

//Bust
//Checks to see if a player has busted
bool Bust(vector<int> cards)
{
	int sum = 0;
	//Add cards together
	for (int i = 0; i < cards.size(); i++)
	{
		sum += cards[i];
		//Check for Face Cards
		if (cards[i] == 11)
			sum -= 1;
		else if (cards[i] == 12)
			sum -= 2;
		else if (cards[i] == 13)
			sum -= 3;
	}
	//Check for Aces
	for (int i = 0; i < cards.size(); i++)
	{
		if (cards[i] == 1 && sum <= 11)
			sum += 10; //Add the extra ten from an ace, if it is helpful
	}
	//Return whether or not you bust
	if (sum > 21)
		return true;
	else
		return false;
}

//PrintGame
//Prints the regular game board
void PrintGame(vector<int> playerCards, vector<int> dealerCards)
{
	cout << "Dealer's Cards: (total #)\n\t#\t";
	for (int i = 1; i < dealerCards.size(); i++)
	{
		//Replace numbers with the proper letters
		if (dealerCards[i] == 1)
			cout << "A\t";
		else if (dealerCards[i] == 11)
			cout << "J\t";
		else if (dealerCards[i] == 12)
			cout << "Q\t";
		else if (dealerCards[i] == 13)
			cout << "K\t";
		else //Or print the number
			cout << dealerCards[i] << "\t";
	}
	int playerSum = 0;
	//Add player cards together
	for (int i = 0; i < playerCards.size(); i++)
	{
		playerSum += playerCards[i];
		//Check for Face Cards
		if (playerCards[i] == 11)
			playerSum -= 1;
		else if (playerCards[i] == 12)
			playerSum -= 2;
		else if (playerCards[i] == 13)
			playerSum -= 3;
	}
	//Check for Aces
	for (int i = 0; i < playerCards.size(); i++)
	{
		if (playerCards[i] == 1 && playerSum <= 11)
			playerSum += 10; //Add the extra ten from an ace, if it is helpful
	}
	cout << endl << endl << "Player's Cards: (total " << playerSum << ")\n\t";
	for (int i = 0; i < playerCards.size(); i++)
	{
		//Replace numbers with the proper letters
		if (playerCards[i] == 1)
			cout << "A\t";
		else if (playerCards[i] == 11)
			cout << "J\t";
		else if (playerCards[i] == 12)
			cout << "Q\t";
		else if (playerCards[i] == 13)
			cout << "K\t";
		else //Or print the number
			cout << playerCards[i] << "\t";
	}
}

//PrintFull
//Prints the game board, showing the dealer's cards
void PrintFull(vector<int> playerCards, vector<int> dealerCards)
{
	int dealerSum = 0;
	//Add dealer cards together
	for (int i = 0; i < dealerCards.size(); i++)
	{
		dealerSum += dealerCards[i];
		//Check for Face Cards
		if (dealerCards[i] == 11)
			dealerSum -= 1;
		else if (dealerCards[i] == 12)
			dealerSum -= 2;
		else if (dealerCards[i] == 13)
			dealerSum -= 3;
	}
	//Check for Aces
	for (int i = 0; i < dealerCards.size(); i++)
	{
		if (dealerCards[i] == 1 && dealerSum <= 11)
			dealerSum += 10; //Add the extra ten from an ace, if it is helpful
	}
	cout << "Dealer's Cards: (total " << dealerSum << ")\n\t";
	for (int i = 0; i < dealerCards.size(); i++)
	{
		//Replace numbers with the proper letters
		if (dealerCards[i] == 1)
			cout << "A\t";
		else if (dealerCards[i] == 11)
			cout << "J\t";
		else if (dealerCards[i] == 12)
			cout << "Q\t";
		else if (dealerCards[i] == 13)
			cout << "K\t";
		else //Or print the number
			cout << dealerCards[i] << "\t";
	}
	int playerSum = 0;
	//Add player cards together
	for (int i = 0; i < playerCards.size(); i++)
	{
		playerSum += playerCards[i];
		//Check for Face Cards
		if (playerCards[i] == 11)
			playerSum -= 1;
		else if (playerCards[i] == 12)
			playerSum -= 2;
		else if (playerCards[i] == 13)
			playerSum -= 3;
	}
	//Check for Aces
	for (int i = 0; i < playerCards.size(); i++)
	{
		if (playerCards[i] == 1 && playerSum <= 11)
			playerSum += 10; //Add the extra ten from an ace, if it is helpful
	}
	cout << endl << endl << "Player's Cards: (total " << playerSum << ")\n\t";
	for (int i = 0; i < playerCards.size(); i++)
	{
		//Replace numbers with the proper letters
		if (playerCards[i] == 1)
			cout << "A\t";
		else if (playerCards[i] == 11)
			cout << "J\t";
		else if (playerCards[i] == 12)
			cout << "Q\t";
		else if (playerCards[i] == 13)
			cout << "K\t";
		else //Or print the number
			cout << playerCards[i] << "\t";
	}
}

//MonteCarlo
//Runs the Monte-Carlo simulation
void MonteCarlo(vector<int> playerCards, vector<int> dealerCards, int runs)
{
	int hitWins = 0; //Wins if you hit
	int standWins = 0; //Wins if you stand
	int hitDraws = 0; //Draws if you hit
	int standDraws = 0; //Draws if you stand

	cout << endl << endl << "Monte-Carlo Simulation running . . . " << endl
		<< "Playing " << runs << " games to determine probability of win if you stand or hit . . . " << endl;
	
	//Hit Simulations
	for (int i = 0; i < runs; i++)
	{
		vector<int> tempDealer; //temporary dealer
		tempDealer.push_back(dealerCards[1]); //Only place the card showing into the dealer's hand
		vector<int> tempPlayer; //temporary player
		//Place the player's cards into the temporary player
		for (int j = 0; j < playerCards.size(); j++)
			tempPlayer.push_back(playerCards[j]);
		tempPlayer.push_back(DrawCard()); //Draw an extra card
		if (!Bust(tempPlayer)) //If you bust, you lose
		{
			//Otherwise, the player follows the dealer's rules about hitting
			bool hit = DealerHit(tempPlayer);
			while (hit == true)
			{
				tempPlayer.push_back(DrawCard());
				hit = DealerHit(tempPlayer);
			}
			if (!Bust(tempPlayer)) //If you bust, you lose
			{
				//Otherwise the dealer plays according to his rules
				hit = DealerHit(tempDealer);
				while (hit == true)
				{
					tempDealer.push_back(DrawCard());
					hit = DealerHit(tempDealer);
				}
				if (Bust(tempDealer)) //If he busts, you win
					hitWins++;
				else //Otherwise
				{
					//Check to see if you win
					int win = GameWin(tempPlayer, tempDealer);
					if (win == 1)
						hitWins++;
					else if (win == -1)
						hitDraws++;
				}
			}
		}
	}
	//Stand Simulations
	for (int i = 0; i < runs; i++)
	{
		vector<int> tempDealer; //temporary dealer
		tempDealer.push_back(dealerCards[1]); //Only place the card showing into the dealer's hand
		//The dealer plays according to his rules
		bool hit = DealerHit(tempDealer);
		while (hit == true)
		{
			tempDealer.push_back(DrawCard());
			hit = DealerHit(tempDealer);
		}
		if (Bust(tempDealer)) //If he busts, you win
			standWins++;
		else //Otherwise
		{
			//Check to see if you win
			int win = GameWin(playerCards, tempDealer);
			if (win == 1)
				standWins++;
			else if (win == -1)
				standDraws++;
		}
	}

	//Output the results
	cout << "Monte-Carlo hit resulted in " << hitWins << " wins, " << runs - (hitWins + hitDraws)
		<< " losses, and " << hitDraws << " draws." << endl;
	cout << "Monte-Carlo stand resulted in " << standWins << " wins, " << runs - (standWins + standDraws)
		<< " losses, and " << standDraws << " draws." << endl;
	float hitWinsFloat = hitWins;
	float standWinsFloat = standWins;
	cout << "Your probability of winning if you hit is " << (hitWinsFloat / runs) * 100 << "%" << endl;
	cout << "Your probability of winning if you stand is " << (standWinsFloat / runs) * 100 << "%" << endl << endl;
}