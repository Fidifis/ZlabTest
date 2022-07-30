SRC_DIR := src
OBJ_DIR := obj
PCH := headers.hpp
LDFLAGS := -pthread
CPPFLAGS := -std=c++17 -Wall -Wextra -pedantic -O2 -pthread
debug: LDFLAGS := -fsanitize=address,leak
debug: CPPFLAGS := -std=c++17 -Wall -Wextra -pedantic -Og -g -fsanitize=address,leak

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

all: zlabtest

debug: zlabtest

zlabtest: $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $<

init:
	mkdir -p $(OBJ_DIR)
	g++ $(CPPFLAGS) $(SRC_DIR)/$(PCH)

clean:
	rm -r $(OBJ_DIR) || true
	rm $(SRC_DIR)/*gch zlabtest || true
