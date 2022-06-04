run: main blackjack2.cpp
	./main
main: blackjack2.cpp
	g++ -std=c++17 blackjack2.cpp -o main