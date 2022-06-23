#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <algorithm>
#include <chrono>
#include <map>
#include <cmath>
#include <memory>
#include <set>
#include <bits/stdc++.h>

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
public:
    tsuit suit;
    uint8_t pvalue;
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

    string str (){
        return svalue() + " of " + ssuit();
    }
    
    
};

class Deck
{
private:
    auto select_random(const map<int,Card> &s, size_t n) {
        auto it = std::begin(s);
        // 'advance' the iterator n times
        std::advance(it,n);
        return it;
    }

public:
    //vector<Card> cards;
    int cardCount = 52;
    bool deckout = false;
    default_random_engine rng;
    map<int,Card> cards;

    Deck()
    {
        auto rd = random_device{};
        rng = default_random_engine{rd()};
        //cards.resize(52);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 13; j++)
            {
                //cards[i * 13 + j] = Card(j + 1, (tsuit)i);
                // shuffle(cards.begin(),cards.end(),e);
                cards.insert(make_pair(i*13+j,Card(j + 1, (tsuit)i)));
            }
        }

        //shuffle(std::begin(cards), std::end(cards), rng);
    }

    Card NextCard(){
        uniform_int_distribution<int> distrib(0, cards.size()-1);
        int i = distrib(rng);
        auto c =select_random(cards,i);
        Card r = ((*c).second);
        cards.erase(c);
        return r;
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

class Dealer
{
public:
    vector<Card> tablehand;
    bool play(int pvalue)
    {
        auto dealervalue = handpoints(tablehand);
        if (dealervalue >= 17 || dealervalue > pvalue || pvalue > 21)
        {
            return false;
        }
        return true;
    }
};

class Game
{
public:
    Deck Cards;

    bool is_stand = false;
    bool bust = false;
    bool dealer_is_stand = false;
    bool dealer_bust = false;
    int tablevalue;
    int playervalue;

    Player &player;
    Dealer dealer;

    void copy(Game g)
    {
        Cards = g.Cards;

        is_stand = g.is_stand;
        bust = g.bust;
        dealer_is_stand = g.dealer_is_stand;
        dealer_bust = g.dealer_bust;
        tablevalue = g.tablevalue;
        playervalue = g.playervalue;
        dealer = g.dealer;
    }

    // Game (const Game &g) : player(g.player)
    // {
    //     Cards = g.Cards;

    //     is_stand = g.is_stand;
    //     bust = g.bust;
    //     dealer_is_stand = g.dealer_is_stand;
    //     dealer_bust = g.dealer_bust;
    //     tablevalue = g.tablevalue;
    //     playervalue = g.playervalue;
    //     dealer = g.dealer;
    // }

    Game(Player &p, Game g, bool Verbose = true) : player(p)
    {
        Cards = g.Cards;
        is_stand = g.is_stand;
        bust = g.bust;
        dealer_is_stand = g.dealer_is_stand;
        dealer_bust = g.dealer_bust;
        tablevalue = g.tablevalue;
        playervalue = g.playervalue;
        dealer = g.dealer;
        // if (Verbose == true)
        // {
        //     cout << " Player Hand " << endl;
        //     for (auto i : player.playerhand)
        //     {
        //         cout << "\t" << i << ", ";
        //     }
        //     cout << endl;

        //     cout << " Dealer Hand " << endl;
        //     for (auto i : dealer.tablehand)
        //     {
        //         cout << "\t" << i << ", ";
        //     }
        //     cout << endl;
        // }

        playervalue = handpoints(player.playerhand);
        if (playervalue == 21)
        {
            is_stand = true;
            dealer_is_stand = true;
        }; // black jack = player insta win
        tablevalue = handpoints(dealer.tablehand);
    }

    Game(Player &p, bool Verbose = false) : player(p) // dealing first hands
    {
        hit(player);
        hit(dealer);
        hit(player);
        // hit(dealer);

        if (Verbose == true)
        {
            cout << " Player Hand " << endl;
            for (auto i : player.playerhand)
            {
                cout << "\t" << i << ", ";
            }
            cout << endl;

            cout << " Dealer Hand " << endl;
            for (auto i : dealer.tablehand)
            {
                cout << "\t" << i << ", ";
            }
            cout << endl;
        }

        playervalue = handpoints(player.playerhand);
        if (playervalue == 21)
        {
            is_stand = true;
            dealer_is_stand = true;
        }; // black jack = player insta win
        tablevalue = handpoints(dealer.tablehand);
    }

    void hit(Player &player)
    {
        // cout << "Player Hit" << endl;
        Card last;
        last = Cards.NextCard();
        player.playerhand.push_back(last);

        playervalue = handpoints(player.playerhand);

        if (playervalue > 21)
        {
            bust = true;
            is_stand = true;
        }
        // cout << " Player Hand " << endl;
        // for (auto i : player.playerhand)
        // {
        //     cout << "\t" << i << "," << endl;
        // }
    }
    void hit(Dealer &dealer)

    {
        // cout << "Dealer Hit" << endl;
        Card last;
        last = Cards.NextCard();
        dealer.tablehand.push_back(last);

        tablevalue = handpoints(dealer.tablehand);

        if (tablevalue > 21)
        {
            dealer_bust = true;
            dealer_is_stand = true;
        }
        // cout << "Dealer Hand " << endl;
        // for (auto i : dealer.tablehand)
        // {
        //     cout << "\t" << i << "," << endl;
        // }
    }

    void stand(Player player)
    {
        // cout << "Player Stand" << endl;
        is_stand = true;
        // cout << " Player Hand " << endl;
        // for (auto i : player.playerhand)
        // {
        //     cout << "\t" << i << "," << endl;
        // }
    }
    void stand(Dealer dealer)
    {
        // cout << "Dealer Stand" << endl;
        dealer_is_stand = true;
        // cout << "Dealer Hand " << endl;
        // for (auto i : dealer.tablehand)
        // {
        //     cout << "\t" << i << "," << endl;
        // }
    }

    int run(bool Verbose = false)
    {
        while ((bust == false) && (is_stand == false))
        {
            // cout << "Begin player moves" << endl;
            if (player.play())
            {
                hit(player);
            }
            else
            {
                stand(player);
            }
        }
        int pvalue = handpoints(player.playerhand);

        hit(dealer);

        while ((dealer_bust == false) && (dealer_is_stand == false))
        {
            // cout << "Begin Dealer moves" << endl;
            if (dealer.play(pvalue))
            {
                hit(dealer);
            }
            else
            {
                stand(dealer);
            }
        }
        int dvalue = handpoints(dealer.tablehand);
        if (Verbose == true)
        {
            // cout << "Player Score: " << pvalue << " Dealer Score: " << dvalue << endl;
            // cout << " Winner is " << playerWin(dvalue, pvalue) << endl;
        }
        return playerWin(dvalue, pvalue);
    }

    int playerWin(int dvalue, int pvalue)
    {
        if (pvalue > 21)
            return -1;
        else if (dvalue > 21)
            return 1;
        else if (pvalue < dvalue)
            return -1;
        else if (pvalue > dvalue)
            return 1;
        else
            return 0;
    }
};

map<string,bool> scoremap;
bool compareCards(Card a, Card b){
    if(a.suit < b.suit){
        return true;
    }else if(a.suit > b.suit){
        return false;
    }else{
        return a.pvalue < b.pvalue;
    }
}

class MCPlayer : public Player
{
public:
    Player p;
    unique_ptr<Game> game;
    int Niter = 10;
    bool keep_map;
    MCPlayer(bool keep_map=false): keep_map(keep_map)
    {

    }
    
    void setGame(Game g)
    {
        game = make_unique<Game>((*this), g, true);
    }

    string playhand(){
        auto hand = playerhand;
        sort(hand.begin(), hand.end(),compareCards);
        auto dealerhand = game->dealer.tablehand;
        sort(dealerhand.begin(), dealerhand.end(),compareCards);
        string s = "";
        for( auto c: hand){
            s+= c.str();
        }
        s+= "D";
        for(auto dh: dealerhand){
            s+= dh.str();
        }
        return s;
    }

    bool play() override
    {
        
        int hw = 0; // losses with hit
        int sw = 0; // losses with stand
        // cout << "Chega" << endl;

        string sh = "";
        if(keep_map){
            sh = playhand();
            if(scoremap.count(sh) == 1){
                return scoremap[sh];
            }
        }

        for (int i = 0; i < Niter; i++)
        {
            // cout << "======================" << endl;
            MCPlayer ptest;
            ptest.playerhand = playerhand;
            ptest.setGame((*game));
            ptest.game->hit(ptest);
            int gamescore = ptest.game->run(true);
            if (gamescore == 1)
                hw++;
            // cout << "======================" << endl;
        }

        for (int i = 0; i < Niter; i++)
        {
            // cout << "++++++++++++++++++++++++++++++++" << endl;
            MCPlayer ptest;
            ptest.playerhand = playerhand;
            ptest.setGame((*game));
            ptest.game->stand(ptest);
            int gamescore = ptest.game->run(true);
            if (gamescore == 1)
                sw++;
            // cout << "++++++++++++++++++++++++++++++++" << endl;
        }

        // cout << handpoints(playerhand) << endl;
        if (hw > sw)
        {
            if(keep_map &&scoremap.count(sh) == 0 && scoremap.size() < 100000 && playerhand.size() < 3){
                //size_t found = sh.rfind("9 of Diamonds10 of Spades"); 
                if (handpoints(playerhand) > 18){
                    cout << " Player Hand " << endl;
                    for (auto i : playerhand)
                    {
                        cout << "\t" << i << "," << endl;
                    }
                    cout << "Dealer Hand " << endl;
                    for (auto i : game->dealer.tablehand)
                    {
                        cout << "\t" << i << "," << endl;
                    }
                    cout << "hw: " << hw << "sw: " <<" "<< sw<< endl;
                }
                scoremap[sh] = true;
            }    
            return true;
        }
        // cout << " Player chose stand " << endl;
        if( keep_map && scoremap.count(sh) == 0 && scoremap.size() < 100000 && playerhand.size() < 3){
            scoremap[sh] = false;
        }
        return false;
    }
};

int main()
{

    // cout << " \n";
    // cout << "Welcome to Black Jack \n";
    // cout << " \n";
    // cout << "    Initializing ... \n";

    // Game starts by Defining the cardPool ( requires a int for the number of decks)
    // then Knuth shuffles a list of integers from 1 to 52*N to represent the cards.
    // we can find the value of a card by modular arithmetic.

    // cout << "Dealing Cards \n";
    // cout << "Player Hand \n";

    // for (Card i : bj.player.playerhand)
    //{
    //     //cout << i << ", ";
    // }
    //
    ////cout << '\n';
    ////cout << " Table Hand \n";
    //
    // for (auto i : bj.dealer.tablehand)
    //{
    //    //cout << i << ", ";
    //}
    // cout << "\n";

    // Then, we deal the first 4 cards (2 to the Dealer, 2 to the Player).
    // We are now able to begin a Game (playerValue, tableValue, cardPool)
    // in cardPool we should probably note the cards that have appeared or sth.

    // cout << " Waiting for a player move \n";

    // this is the main game loop!
    // we then update Game.hit() or Game.split() or Game.stand()
    // accordingly.

    // proceed until the end of the turn
    // Dealer then tries to beat the player score;

    // Reveal the game winner
    int Niter = pow(10, 2);
    int NofDecks = 100;
    int wp = 0;
    int wd = 0;
    int wtie = 0;
    for (int i = 0; i < Niter; i++)
    {
        //MCPlayer player(true);
        MCPlayer player(true);
        Game bj(player);
        player.setGame(bj);

        int gamescore = bj.run(false);
        if (gamescore == -1)
            wd++;
        else if (gamescore == 0)
            wtie++;
        else
        {
            wp++;
        }
        if (i % 100 == 0)
        {
            cout << " Progress: " << i / 100 << endl;
        }

        // cout << " Final Hands " << endl;
        // cout << " Player Hand " << endl;
        // for (auto i : bj.player.playerhand)
        // {
        //     cout << "\t" << i << "," << endl;
        // }
        // cout << "Dealer Hand " << endl;
        // for (auto i : bj.dealer.tablehand)
        // {
        //     cout << "\t" << i << "," << endl;
        // }
    }
    cout << "Player wins: " << wp << " Dealer wins: " << wd << " Ties: " << wtie << endl;

    double ratio = 1 - wd / (double)Niter;

    cout << " Winrate: " << ratio << endl;

    for (auto const &pair: scoremap) {
        cout << "{" << pair.first << "  :  " << pair.second << "}\n";
    }
    return 0;
}
