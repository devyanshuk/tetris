SDIR = src
IDIR = include
ODIR = obj

CC = g++ -std=c++17
CCFLAGS = -Wall -Wextra
SDLFLAG = -lSDL2

TARGET = tetris
SRC = $(wildcard $(SDIR)/*.cpp)
OBJ = $(SRC:$(SDIR)/%.cpp=$(ODIR)/%.o)
DEPS = $(wildcard $(IDIR)/*)


.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(OBJ) $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(SDLFLAG)

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) $(CCFLAGS) -c $< -o $@ $(SDLFLAG)

