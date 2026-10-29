CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC = src/main.cpp src/question.cpp src/question_bank.cpp src/test_session.cpp
TARGET = test_engine

all: $(TARGET)

$(TARGET): $(SRC) include/*.h
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) results.csv

run: $(TARGET)
	./$(TARGET) questions.txt

.PHONY: all clean run
