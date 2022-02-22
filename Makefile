SRC_DIR := src
OBJ_DIR := obj
PCH := headers.hpp
LDFLAGS :=
CPPFLAGS := -Wall -pedantic

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
PCH_FILE := $(OBJ_DIR)/$(PCH).gch

zlabtest: $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) -c -o $@ $<

init:
	mkdir -p $(OBJ_DIR)
	g++ $(CPPFLAGS) -o $(PCH_FILE) $(SRC_DIR)/$(PCH)

clean:
	rm -r $(OBJ_DIR)
	rm zlabtest config.json 2> /dev/null || true
