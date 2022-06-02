run: main main.cpp
	./main


main: main.cpp
	g++ -std=c++17 main.cpp -o main