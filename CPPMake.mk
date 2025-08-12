# variables to be passed in for:
#
# MODE         "debug" or "release".
# NAME         Name of the output executable (and object file directory).
# SOURCE_DIR   Directory where source files and headers are found.

CFLAGS :=  -std=c++20
CFLAGS += -Wall -Wextra -Werror -Wno-unused-parameter
CC := g++
# If we're building at a point in the middle of a chapter, don't fail if there
# are functions that aren't used yet.
ifeq ($(SNIPPET),true)
	CFLAGS += -Wno-unused-function 
endif


#ModeConfig
ifeq ($(MODE), debug)
	CFLAGS += -O0 -DEBUG -g
	BUILD_DIR := build/debug
else 
	CFLAGS += -O3 -flto
	BUILD_DIR := build/release
endif

HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS := $(addprefix $(BUILD_DIR)/$(NAME)/, $(notdir $(SOURCES:.cpp=.o)))

#Target:

# link
# $@：Makefile 中的自动变量，表示当前规则的目标文件名（例如 build/main.o）
# $^: 所有依赖
# $<: 第一个依赖
build/$(NAME): $(OBJECTS)
	mkdir -p build
	$(CC) $(CFLAGS) -lstdc++ $^ -o $@

# complier
$(BUILD_DIR)/$(NAME)/%.o: $(SOURCE_DIR)/%.cpp $(HEADERS)
	@ mkdir -p $(BUILD_DIR)/$(NAME)
	$(CC) -c $(C_LANG) $(CFLAGS) -o $@ $<