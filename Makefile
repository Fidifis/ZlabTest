SRC_DIR := src
OBJ_DIR := obj
PCH := headers.hpp
LDFLAGS :=
CPPFLAGS := -std=c++17 -Wall -Wextra -pedantic -Wno-unused-result -O2

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

zlabtest: $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $<

init:
	mkdir -p $(OBJ_DIR)
	g++ $(CPPFLAGS) $(SRC_DIR)/$(PCH)

clean:
	rm -r $(OBJ_DIR)
	rm $(SRC_DIR)/*gch
	rm zlabtest config.json 2> /dev/null || true
