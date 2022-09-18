CPP = g++
FLAGS = -c -Werror -Wextra -Wall
BOOST = -lboost_thread -lboost_system
THREAD = -lpthread
OBJ = obj
BIN = bin
SRC = src

all: directory_check $(BIN)/dashboard
run: all
	./$(BIN)/dashboard
directory_check:
	@if [ ! -d $(BIN) ]; then \
		mkdir -p $(BIN); \
	fi
	@if [ ! -d $(OBJ) ]; then \
		mkdir -p $(OBJ); \
	fi
$(OBJ)/emulator.o: $(SRC)/emulator.cpp
	$(CPP) $(FLAGS) $(SRC)/emulator.cpp -o $(OBJ)/emulator.o
$(OBJ)/dashboard.o: $(SRC)/dashboard.cpp
	$(CPP) $(FLAGS) $(SRC)/dashboard.cpp -o $(OBJ)/dashboard.o
$(BIN)/dashboard: $(OBJ)/dashboard.o $(OBJ)/emulator.o
	$(CPP) $(OBJ)/dashboard.o $(OBJ)/emulator.o -o $(BIN)/dashboard $(BOOST) $(THREAD)
clean:
	rm $(OBJ)/*.o $(BIN)/dashboard