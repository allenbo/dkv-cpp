SRC_DIR := ./src
INCLUDE_DIR := ./include
BUILD_DIR := ./build

TEST_SRC_DIR := ./t
TESTBIN_DIR := $(BUILD_DIR)/t

APP_SRC_DIR := ./app
APPBIN_DIR := ./bin

CPP := g++
CC := gcc
AR := ar

CFLAG := -O2 -Wall -std=c++11 -DNDEBUG
LFLAG := -O2 -lpthread -DNDEBUG

THIRDPARTY_INCLUDE := -I../common/include/ -I../networking/include
THIRDPARTY_LIB := -L../networking/ -lnetworking

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst %.cpp,%.o, $(subst $(SRC_DIR),$(BUILD_DIR), $(SRC)))

APP_SRC := $(wildcard $(APP_SRC_DIR)/*.cpp)
APP_TARGET := $(patsubst %.cpp, %, $(subst $(APP_SRC_DIR), $(APPBIN_DIR), $(APP_SRC)))

TEST_SRC := $(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_TARGET := $(patsubst %.cpp, %, $(subst $(TEST_SRC_DIR),$(TESTBIN_DIR), $(TEST_SRC)))


.PHONY:all target test $(BUILD_DIR) $(TESTBIN_DIR) $(APPBIN_DIR)
all: test target
test: $(TESTBIN_DIR) $(TEST_TARGET) target
target: $(BUILD_DIR) $(OBJ) $(ARCHIVE) $(APPBIN_DIR) $(APP_TARGET)

$(BUILD_DIR):
	mkdir -p $@

$(APPBIN_DIR):
	mkdir -p $@

$(TESTBIN_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CPP) -c $< $(CFLAG) -o $@ -I$(INCLUDE_DIR) $(THIRDPARTY_INCLUDE) $(THIRDPARTY_LIB)

$(TESTBIN_DIR)/%:$(TEST_SRC_DIR)/%.cpp $(OBJ)
	$(CPP) $^ $(CFLAG) $(LFLAG) -o $@ -I$(INCLUDE_DIR) -L$(BUILD_DIR) $(THIRDPARTY_INCLUDE) $(THIRDPARTY_LIB)

$(APPBIN_DIR)/%:$(APP_SRC_DIR)/%.cpp $(OBJ)
	$(CPP) $^ $(CFLAG) $(LFLAG) -o $@ -I$(INCLUDE_DIR) -L$(BUILD_DIR) $(THIRDPARTY_INCLUDE) $(THIRDPARTY_LIB)

clean:
	rm -rf $(BUILD_DIR) $(TESTBIN_DIR) $(ARCHIVE) $(APPBIN_DIR)
