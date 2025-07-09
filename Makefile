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
	$(CC) $(CFLAGS) ./cpp/tools/GenerateAST.cpp -o $(BUILD_DIR)/ASTEXE -g
	$(BUILD_DIR)/ASTEXE ./$(SOURCE_DIR)/
	
test: clox
	@GREEN='\033[92m'; RESET='\033[0m'; \
	echo  "$$GREEN================ Lox Test Begin ================\n$$RESET"; \
	./run_lox_tests.py $(ARGS); \
	echo  "$$GREEN\n================= Lox Test End =================$$RESET"
.PHONY: clox test