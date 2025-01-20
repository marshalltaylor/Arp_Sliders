REPO_ROOT = ../..

SOURCES := \
main.cpp \
../memoryController.cpp \
../pool.cpp \
../poolTest/poolTest.cpp \

UNIT_NAME := output.exe

INCLUDE_PATHS := \
../ \
../poolTest \
.

CC := gcc
CPP := g++
AR = ar
CP = objcopy
OBJD = objdump

#CFLAGS...:= -pedantic-errors -Wall -Wextra -Werror
CPPFLAGS := -v -pedantic-errors -Wall -Wextra -Werror
#LDFLAGS  := -L/usr/lib -lstdc++ -lm
LDFLAGS  := -lstdc++ -lm

BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
OUTPUT_DIR  := $(BUILD)/output

INCLUDE  := $(addsuffix /,$(addprefix -I, $(INCLUDE_PATHS)))

OBJECTS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(SOURCES))))


$(OBJ_DIR)/%.o: %.cpp
	@echo "#### Compiling $< (CPP)"
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) $(INCLUDE) -o $@ -c $<

$(OBJ_DIR)/%.o: %.c
	@echo "#### Compiling $< (C)"
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(INCLUDE) -o $@ -c $<

.PHONY: all build clean debug release

default: $(OUTPUT_DIR)/$(UNIT_NAME)

test: default
	@echo ""
	@echo "Running output file $(OUTPUT_DIR)/$(UNIT_NAME)"
	@echo ""
	-@./$(OUTPUT_DIR)/$(UNIT_NAME)

build:
	@mkdir -p $(OUTPUT_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CPPFLAGS += -DDEBUG -g
debug: all

release: CPPFLAGS += -O2
release: all

clean:
	@echo "#### Clean unit: $(UNIT_NAME)"
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(OUTPUT_DIR)/*
	-@rm -rvf $(BUILD)/*.o

info:
	@echo "$(INCLUDE)"

$(OUTPUT_DIR)/$(UNIT_NAME): build $(OBJECTS)
	@echo "#### Creating executable"
	@mkdir -p $(@D)
	$(CPP) $(CPPFLAGS) $(INCLUDE) $(LDFLAGS) -o $(OUTPUT_DIR)/$(UNIT_NAME) $(OBJECTS)