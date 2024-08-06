clean:
	-rm -rf main *.o Device_manager tap tapbin
Device_manager.o: Device_manager.cpp
	-g++ -c -o Device_manager.o Device_manager.cpp -lm -w -lncurses
tap.o: tap.cpp
	-g++ -c -o tap.o tap.cpp -lm -w -lncurses
main.o: main.cpp
	-g++ -c -o main.o main.cpp -lm -w -lncurses
build_and_run: main.o Device_manager.o tap.o 
	-g++ -o tapbin main.o Device_manager.o tap.o -lm -w -lncurses
	-rm -rf main *.o Device_manager tap
	-sudo ./tapbin
install_dependencies:
	sudo apt update
	sudo apt install net-tools
	sudo apt install -y gcc
	sudo apt install -y g++
	sudo apt install bridge-utils -y
	sudo apt install libncurses5-dev libncursesw5-dev -y
