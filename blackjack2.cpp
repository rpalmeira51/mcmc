#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <chrono>
#include <map>
#include <cmath>

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
    int cardCount = 52;
    bool deckout = false;

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

    void reshuffle(){

        vector<Card> cards; 
        int cardCount = 52;
        bool deckout = false;

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
    virtual bool play()
    {
        auto handvalue = handpoints(playerhand);
        if (handvalue <= 15)
        {
            return true;
        }
        return false;
    }
};

class Game;

class MCPlayer: public Player
{
public:
    Game& game;
    MCPlayer(Game& g): game(g){

    }
    bool play() override
    {
        cout << "Test" << endl;
        auto handvalue = handpoints(playerhand);
        if (handvalue <= 15)
        {
            return true;
        }
        return false;
    }
};

class Dealer
{
public:
    vector<Card> tablehand;
    bool play( int pvalue)
    {
        auto dealervalue = handpoints(tablehand);
        if(dealervalue > 21 || dealervalue > pvalue || pvalue > 21){
            return false;
        }
        return true;
    }
};

class Game
{
public:
    // vector<Card> cardPool;
    Deck Cards;
    // vector<Card> cardPool =  Cards.cards;

    bool is_stand = false;
    bool bust = false;
    bool dealer_is_stand = false;
    bool dealer_bust = false;
    int tablevalue;
    int playervalue;

    Player player;
    Dealer dealer;

    Game(Deck& Cards) // dealing first hands
    { 
        if (Cards.cardCount < 4)Cards.reshuffle();
        hit(player, Cards);
        hit(dealer, Cards);
        hit(player, Cards);
        hit(dealer, Cards);
        Cards.cardCount -= 4;

        // cout << " Player Hand " << endl;
        // for(auto i: player.playerhand){
        //     cout << i << ", ";
        // }
        // cout << endl;

        // cout << " Dealer Hand " << endl;
        // for(auto i: dealer.tablehand){
        //     cout << i << ", ";
        // }
        // cout << endl;   

        playervalue = handpoints(player.playerhand);
        if (playervalue == 21){
            is_stand = true;
            dealer_is_stand = true;
            };// black jack = player insta win 
        tablevalue = handpoints(dealer.tablehand);
    }


    void hit(Player& player, Deck& Cards)
    {
        vector<Card> cardPool = Cards.cards;
        Card last;
        last = cardPool.back();
        cardPool.pop_back();
        player.playerhand.push_back(last);

        playervalue = handpoints(player.playerhand);

        if (playervalue > 21)
        {
            bust = true;
            is_stand = true;
        }
    }
    void hit(Dealer& dealer, Deck& Cards)
    
    {
        vector<Card> cardPool = Cards.cards;
        Card last;
        last = cardPool.back();
        cardPool.pop_back();
        dealer.tablehand.push_back(last);

        tablevalue = handpoints(dealer.tablehand);

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

    int run(bool Verbose = false)
    {
        while ((bust == false) && (is_stand == false))
        {
            //cout<< "Begin player moves" << endl;
            if( player.play()){
                //cout<< "Hit" << endl;
                if (Cards.cardCount == 0)Cards.reshuffle();
                hit(player, Cards);
            }else{
                //cout<< "Stand" << endl;
                stand(player);
            }
            // for(auto i: player.playerhand){
            //     cout << i << "," << endl;
            // }
        }   
        int pvalue = handpoints(player.playerhand) ;
        while ((dealer_bust == false) && (dealer_is_stand == false))
        {
            //cout<< "Begin Dealer moves" << endl;
            if( dealer.play(pvalue)){
                //cout<< "Hit" << endl;  
                if (Cards.cardCount == 0)Cards.reshuffle();
                hit(dealer, Cards);
            }else{
                //cout<< "Stand" << endl;
                stand(dealer);
            }
            // for(auto i: dealer.tablehand){
            //     cout << i << "," << endl;
            // }
        }   
        int dvalue = handpoints(dealer.tablehand);
        if (Verbose == true){
        cout << "Player Score: " << pvalue << " Dealer Score: " << dvalue << endl;
        cout << " Winner is " << playerWin(dvalue, pvalue) << endl;
        }
        return playerWin(dvalue, pvalue);
    }

    int playerWin(int dvalue, int pvalue)
    {
        if (bust == true)return -1;
        else if( dealer_bust == true)return 1;
        else if( pvalue < dvalue) return -1;
        else if (pvalue > dvalue) return 1;
        else return 0;
    }
};



int main()
{

    //cout << " \n";
    //cout << "Welcome to Black Jack \n";
    //cout << " \n";
    //cout << "    Initializing ... \n";

    // Game starts by Defining the cardPool ( requires a int for the number of decks)
    // then Knuth shuffles a list of integers from 1 to 52*N to represent the cards.
    // we can find the value of a card by modular arithmetic.

    //cout << "Dealing Cards \n";
    //cout << "Player Hand \n";

    //for (Card i : bj.player.playerhand)
    //{
    //    //cout << i << ", ";
    //}
//
    ////cout << '\n';
    ////cout << " Table Hand \n";
//
    //for (auto i : bj.dealer.tablehand)
    //{
    //    //cout << i << ", ";
    //}
    //cout << "\n";

    // Then, we deal the first 4 cards (2 to the Dealer, 2 to the Player).
    // We are now able to begin a Game (playerValue, tableValue, cardPool)
    // in cardPool we should probably note the cards that have appeared or sth.

    //cout << " Waiting for a player move \n";

    // this is the main game loop!
    // we then update Game.hit() or Game.split() or Game.stand()
    // accordingly.

    // proceed until the end of the turn
    // Dealer then tries to beat the player score;

    // Reveal the game winner
    int Niter = pow(5,1);
    int NofDecks = 2;
    int wp = 0;
    int wd = 0;
    int wtie = 0;

    Deck cardDeck;

    for(int i= 0; i < Niter; i++){
        Game bj(cardDeck);
        MCPlayer p(bj);
        int gamescore = bj.run(true);
        if( gamescore == -1)wd++;
        else if (gamescore == 0) wtie++;
        else{wp ++;}

    }
    cout << "Player wins: " << wp << " Dealer wins: " << wd << " Ties: " << wtie << endl;

    int counter = 0;

    for (auto i: cardDeck.cards){
    cout << i << endl;
    counter ++;
    }

    cout << counter << endl;
    return 0;
}
