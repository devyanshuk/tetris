OUTPUT_PATH = .
GAME_NAME = tetris

SDIR = src
IDIR = include
ODIR = $(OUTPUT_PATH)/obj
TTFDIR = font
IMGDIR = img

CC = g++ -std=c++17
CCFLAGS = -Wall -Wextra
SDLFLAG = -lSDL2
TTFFLAG = -lSDL2_ttf
IMGFLAG = -lSDL2_image

MKDIR_P = mkdir -p
RMDIR = rmdir
RM = rm -f

TARGET = $(OUTPUT_PATH)/$(GAME_NAME)
SRC = $(wildcard $(SDIR)/*.cpp)
TTF = $(wildcard $(TTFDIR)/*.ttf)
IMG = $(wildcard $(IMGDIR)/*.png)
OBJ = $(SRC:$(SDIR)/%.cpp=$(ODIR)/%.o)
DEPS = $(wildcard $(IDIR)/*)


.PHONY: objdir all clean

all: objdir $(TARGET)

objdir:
	$(MKDIR_P) $(ODIR)

clean:
	$(RM) $(OBJ) $(TARGET)
	$(RM) $(OUTPUT_PATH)/*.ttf
	$(RM) $(OUTPUT_PATH)/*.png
	find $(OUTPUT_PATH) -type d -maxdepth 1 -name `basename $(ODIR)` -delete

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(SDLFLAG) $(TTFFLAG) $(IMGFLAG)
	cp $(TTF) $(OUTPUT_PATH)
	cp $(IMG) $(OUTPUT_PATH)


$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) $(CCFLAGS) -c $< -o $@

