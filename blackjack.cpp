#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;
unsigned num = chrono::system_clock::now().time_since_epoch().count();
auto rng = default_random_engine(num);

vector<int> shuffle_decks(int number_of_decks)
{
    int N = 52 * number_of_decks;
    vector<int> decks(N);
    iota(decks.begin(), decks.end(), 1);

    shuffle(decks.begin(), decks.end(), rng);

    return decks;
}

int card(int n) // Função que leva x -> -1, se x = As; x- > x; se x < J,Q,K; 10 c.c
{
    int x = (n % 52) % 13;
    if ((2 <= x) and (x <= 9))
    {
        return x;
    }
    else if (x == 1)
    {
        return -1; // Ace
    }
    else
    {
        return 10;
    }
}

int handpoints(vector<int> hand) // Função pra calcular a pontuação total
{

    int points;
    points = 0;
    int aces;
    aces = 0;
    for (int i : hand)
    {
        if (i > 0)
        {
            points = points + i;
        }
        else
        {
            aces = aces + 1;
        }
    }
    for (int i = 0; i < aces; i++)
    {
        if (points > 11)
        {
            points = points + 1;
        }
        else
        {
            points = points + 11;
        }
    }
    return points;
}

class gameState
{
public:
    vector<int> cardPool;
    vector<int> playerhand;
    int playervalue;
    vector<int> tablehand;
    int tablevalue;
    bool is_stand;
    bool bust;
    int winner; // -1 if table, 0 if tie, 1 if player

    gameState(vector<int> cards)
    {
        cardPool = cards;
        tablehand.push_back(cardPool[0]);
        playerhand.push_back(cardPool[1]);
        tablehand.push_back(cardPool[2]);
        playerhand.push_back(cardPool[3]);

        // Como Remover cards[:4] i.e. refresh cardPool

        playervalue = handpoints(playerhand);
        tablevalue = handpoints(tablehand);
    }

    void hit()
    {
        playerhand.push_back(cardPool[0]);

        // Falta fazer um refresh em cardPool aqui tambem

        playervalue = handpoints(playerhand);

        if (playervalue > 21)
        {
            bust = true;
            is_stand = true;
        }
    }

    void stand()
    {
        is_stand = true;
    }

    int get_winner()
    {
        if (playervalue > 21)
        {
            return -1;
        }
        if (abs(tablevalue - 21) < abs(playervalue))
        {
            return -1;
        }
        if (tablevalue == playervalue)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
};

void get_player_move(int playerValue, int tableValue) //  playerValue && tableValue <-- gameState
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
    cout << "Welcome to Black Jack \n";
    cout << " \n";
    cout << "    Initializing ... \n";

    vector<int> cardPool;
    cardPool = shuffle_decks(1);
    gameState BlackJack(cardPool);

    // Game starts by Defining the cardPool ( requires a int for the number of decks)
    // then Knuth shuffles a list of integers from 1 to 52*N to represent the cards.
    // we can find the value of a card by modular arithmetic.

    cout << "Dealing Cards \n";

    cout << "Player Hand \n";

    for (int i : BlackJack.playerhand)
    {
        cout << i << " ";
    }

    cout << '\n';
    cout << " Table Hand \n";

    for (int i : BlackJack.tablehand)
    {
        cout << i << " ";
    }
    cout << "\n";

    // Then, we deal the first 4 cards (2 to the Dealer, 2 to the Player).
    // We are now able to begin a gameState (playerValue, tableValue, cardPool)
    // in cardPool we should probably note the cards that have appeared or sth.

    cout << " Waiting for a player move \n";

    // this is the main game loop!
    // we then update gameState.hit() or gameState.split() or gameState.stand()
    // accordingly.

    // proceed until the end of the turn
    // Dealer then tries to beat the player score;

    // Reveal the game winner

    get_player_move(14, 10);
    get_player_move(20, 10);
    get_player_move(21, 10);
}
