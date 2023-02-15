build_main:
	rm -f main.exe
	g++ -g -std=c++11 -Wall main.cpp -o main.exe
	
run:
	./main.exe

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./main.exe