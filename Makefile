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

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(OBJ_DIR) $(PCH_FILE)
	g++ $(CPPFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $@

$(PCH_FILE): $(SRC_DIR)/$(PCH) $(OBJ_DIR)
	g++ $(CPPFLAGS) -o $@ $<

clean:
	rm -r $(OBJ_DIR)
	rm zlabtest config.json 2> /dev/null
