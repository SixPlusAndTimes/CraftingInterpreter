BUILD_DIR := build
SOURCE_DIR := cpp
CFLAGS :=  -std=c++20
CFLAGS += -Wall -Wextra -Werror -Wno-unused-parameter
CC := g++

clox:
	$(MAKE) -f CPPMake.mk NAME=clox MODE=debug SOURCE_DIR=$(SOURCE_DIR) SNIPPET=true
	@ cp build/clox clox
clean:
	rm -rf $(BUILD_DIR)

ast:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) ./cpp/tools/GenerateAST.cpp -o $(BUILD_DIR)/ASTEXE
	$(BUILD_DIR)/ASTEXE ./$(SOURCE_DIR)/
	

.PHONY: clox