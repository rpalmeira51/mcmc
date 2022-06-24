run: main blackjack_number_hash.cpp
	./main
main: blackjack_number_hash.cpp
	g++ -std=c++17 -O3 blackjack_number_hash.cpp -o main
clean: 
	rm main