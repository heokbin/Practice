main.o : main.cpp
	g++ -Werror -c main.cpp

main.exe: main.o
	g++ -o main.exe main.o