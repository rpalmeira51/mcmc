#include <iostream>
#include <string>

using namespace std;

void get_player_move(int playerValue, int tableValue) // deck cardPool
{

    if (playerValue < 21)
    {
        cout << " Hit \n";
    }
    else
    {
        cout << " Stand \n";
    }
}

int main()
{

    cout << " \n";
    cout << " Welcome to Black Jack \n";
    cout << " Initializing ... \n";

    // Game starts by Defining a cardPool ( requires a int for the number of decks)
    // then shuffles a list of integers from 1 to 52*N to represent the cards.
    // we can find the value of a card by modular arithmetic.

    cout << "Dealing Cards \n";

    // Then, we deal the first 4 cards (2 to the Dealer, 2 to the Player).
    // We are now able to begin a gameState (playerValue, tableValue, cardPool)
    // in cardPool we should probably note the cards that have appeared or sth.

    cout << " Waiting for a player move \n";

    // this is the main game loop!
    // we then update gameState.hit() or gameState.split() or gameState.stand()
    // accordingly.

    // procceed until the end of the turn
    // Dealer then tries to beat the player score;

    // Reveal the game winner

    get_player_move(14, 10);
    get_player_move(20, 10);
    get_player_move(21, 10);
}
