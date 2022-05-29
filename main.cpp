#include <iostream>
#include<string>
#include<vector>
#include<random>
#include <stdint.h>
#include <chrono>       // std::chrono::system_clock
#include <algorithm>    // std::shuffle

using namespace std;

enum tsuit {Diamonds, Hearts, Clubs, Spades};

struct Card
{
    uint8_t value;
    tsuit suit;
};


class Deck {
    public:
        vector<Card> cards;
        Deck(){
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            default_random_engine e(seed);
            cards.resize(52);
            for(int i =0; i< 4 ; i++){
                for (int j =0; j<13; j++){
                    cards[i*13 +j ].value = j+1;
                    cards[i*13 +j ].suit = (tsuit) i;
                    //shuffle(cards.begin(),cards.end(),e);
                }
            }
        }
};

int main(){
    
    cout << "Hello world \n";    
    Deck deck;
    for (auto i = deck.cards.begin(); i != deck.cards.end(); ++i){
        cout << +i->value<< " " <<i->suit << "\n";
    }
    //delete deck;
    return 0;
}