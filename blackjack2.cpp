#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <chrono>
#include <map>

using namespace std;
unsigned num = chrono::system_clock::now().time_since_epoch().count();
auto rng = default_random_engine(num);

enum tsuit
{
    Diamonds,
    Hearts,
    Clubs,
    Spades
};

class Card
{
private:
    uint8_t pvalue;

public:
    tsuit suit;
    Card(){};
    Card(u_int8_t value, tsuit suit) : pvalue(value), suit(suit)
    {
    }

    int value() const
    {
        if (pvalue >= 10)
        {
            return 10;
        }
        else
        {
            return (int)pvalue;
        }
    }

    string svalue() const
    {
        map<int, string> names = {
            {1, "Ace"},
            {11, "Queen"},
            {12, "Jack"},
            {13, "King"}};
        if (names.count(pvalue) == 1)
        {
            return names[pvalue];
        }
        else
        {
            return to_string(pvalue);
        }
    }

    string ssuit() const
    {
        vector<string> vec = {"Diamonds", "Hearts", "Clubs", "Spades"};
        return vec[suit];
    }

    friend std::ostream &operator<<(std::ostream &os, const Card &c)
    {
        return os << "Card: " << c.svalue() << " of " << c.ssuit();
    }
};

class Deck
{
public:
    vector<Card> cards;
    Deck()
    {
        auto rd = random_device{};
        auto rng = default_random_engine{rd()};
        cards.resize(52);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 13; j++)
            {
                cards[i * 13 + j] = Card(j + 1, (tsuit)i);
                // shuffle(cards.begin(),cards.end(),e);
            }
        }
        shuffle(std::begin(cards), std::end(cards), rng);
    }
};

int handpoints(vector<Card> hand) // Função pra calcular a pontuação total
{

    int points;
    points = 0;
    int aces;
    aces = 0;
    for (Card c : hand)
    {
        int i = c.value();
        if (i > 1)
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

class Player
{
public:
    vector<Card> playerhand;
    int playervalue;

    bool play(Game &gs)
    {

        auto handvalue = gs.playervalue;

        if (handvalue > 15)
        {
            gs.stand(*this);
        }
        else
        {
            gs.hit(*this);
        }
    }
};

class Dealer
{
public:
    vector<Card> tablehand;
    int tablevalue;

    void play(Game &gs)
    {

        auto dealervalue = gs.tablevalue;
        auto playervalue = gs.playervalue;

        if (dealervalue < playervalue)
        {
            gs.hit(*this);
        }
    }
};

class Game
{
public:
    vector<Card> cardPool;
    bool is_stand;
    bool bust;
    bool dealer_is_stand;
    bool dealer_bust;
    int winner; // -1 if table, 0 if tie, 1 if player

    Player player;
    Dealer dealer;

    Game(vector<Card> cards)
    {
        cardPool = cards;
        Card last;
        last = cardPool.back();
        cardPool.pop_back();
        tablehand.push_back(last);
        last = cardPool.back();
        cardPool.pop_back();
        playerhand.push_back(last);
        last = cardPool.back();
        cardPool.pop_back();
        tablehand.push_back(last);
        last = cardPool.back();
        cardPool.pop_back();
        playerhand.push_back(last);

        playervalue = handpoints(playerhand);
        tablevalue = handpoints(tablehand);
    }

    void hit(Player player)
    {
        Card last;
        last = cardPool.back();
        cardPool.pop_back();
        playerhand.push_back(last);

        playervalue = handpoints(playerhand);

        if (playervalue > 21)
        {
            bust = true;
            is_stand = true;
        }
    }
    void hit(Dealer dealer)
    {
        Card last;
        last = cardPool.back();
        cardPool.pop_back();
        tablehand.push_back(last);

        tablevalue = handpoints(tablehand);

        if (tablevalue > 21)
        {
            dealer_bust = true;
            dealer_is_stand = true;
        }
    }

    void stand(Player player)
    {
        is_stand = true;
    }
    void stand(Dealer dealer)
    {
        dealer_is_stand = true;
    }

    void run()
    {

        while ((bust != false) && (is_stand != false))
        {

            player.play(this);
        }

        while ((dealer_bust != false) && (dealer_is_stand != false))
        {

            dealer.play(this);
        }
    }

    int get_winner()
    {
        if (bust == true)
        {
            return -1;
        }
        if (abs(tablevalue - 21) < abs(playervalue - 21))
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

int main()
{

    cout << " \n";
    cout << "Welcome to Black Jack \n";
    cout << " \n";
    cout << "    Initializing ... \n";

    Deck cardPool;
    Game bj(cardPool.cards);

    // Game starts by Defining the cardPool ( requires a int for the number of decks)
    // then Knuth shuffles a list of integers from 1 to 52*N to represent the cards.
    // we can find the value of a card by modular arithmetic.

    cout << "Dealing Cards \n";
    cout << "Player Hand \n";

    for (Card i : bj.playerhand)
    {
        cout << i << ", ";
    }

    cout << '\n';
    cout << " Table Hand \n";

    for (auto i : bj.tablehand)
    {
        cout << i << ", ";
    }
    cout << "\n";

    // Then, we deal the first 4 cards (2 to the Dealer, 2 to the Player).
    // We are now able to begin a Game (playerValue, tableValue, cardPool)
    // in cardPool we should probably note the cards that have appeared or sth.

    cout << " Waiting for a player move \n";

    // this is the main game loop!
    // we then update Game.hit() or Game.split() or Game.stand()
    // accordingly.

    // proceed until the end of the turn
    // Dealer then tries to beat the player score;

    // Reveal the game winner

    bj.run();
}
