# ---- CONFIGURAZIONE ----
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -Iinclude
LDFLAGS = -L/opt/homebrew/lib

# --- MODIFICA QUI ---
# Aggiungi -lSDL2_ttf per collegare la libreria dei font.
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/PixelWarrior

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# ---- REGOLE ----

all: $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

run: $(TARGET)
	@echo "Running PixelWarrior..."
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)

.PHONY: all clean run
