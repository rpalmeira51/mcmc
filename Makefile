run: main2 blackjack2.cpp
	./main2
main2: blackjack2.cpp
	g++ -std=c++17 -O3 blackjack2.cpp -o main2
clean: 
	rm main