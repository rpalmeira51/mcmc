#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <stdint.h>
#include <chrono>    // std::chrono::system_clock
#include <algorithm> // std::shuffle

using namespace std;

enum tsuit
{
    Diamonds,
    Hearts,
    Clubs,
    Spades
};

struct Card
{
    uint8_t value;
    tsuit suit;
};


class Deck {
    public:
        vector<Card> cards;
        Deck(default_random_engine rng){
            cout << "flag";
            cards.resize(52);
            for(int i =0; i< 4 ; i++){
                for (int j =0; j<13; j++){
                    cards[i*13 +j ].value = j+1;
                    cards[i*13 +j ].suit = (tsuit) i;
                    //shuffle(cards.begin(),cards.end(),e);
                }
            }
            shuffle(std::begin(cards), std::end(cards), rng);
        }
    };

int main(){
    
    cout << "Hello world \n";    
    auto rd = random_device {}; 
    auto rng = default_random_engine { rd() };

    Deck deck(rng);
    for (auto i : deck.cards){
        cout << +i.value<< " " << +i.suit << "\n";
    }
    return 0;
}