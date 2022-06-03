#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <stdint.h>
#include <chrono>    // std::chrono::system_clock
#include <algorithm> // std::shuffle
#include <map>

using namespace std;

enum tsuit
{
    Diamonds,
    Hearts,
    Clubs,
    Spades
};


class Card{
    public: 
    tsuit suit;
    uint8_t value;

    string svalue() const{
        map<int, string> names = {
            {1, "Ace"},
            {11, "Queen"},
            {12, "Jack"},
            {13, "King"}
        };
        if(names.count(value) == 1){
            return names[value];
        }else{
            return to_string(value);
        }
    }

    string ssuit() const{
        vector<string> vec = {"Diamonds", "Hearts","Clubs","Spades"};
        return vec[suit];
    }


    friend std::ostream& operator<<(std::ostream& os, const Card& c)
        {
            return os << "Card: "<< c.svalue() << " of "<< c.ssuit();
        }    
};

class Deck {
    public:
        vector<Card> cards;
        Deck(){
            auto rd = random_device {}; 
            auto rng = default_random_engine { rd() };
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
    Deck deck;
    for (auto i : deck.cards){
        cout << i<< "\n";
    }
    return 0;
}