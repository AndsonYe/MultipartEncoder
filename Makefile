PROJECT := multipart_parser_test
CXX := g++

SRC_DIR := .
SRC := $(wildcard $(SRC_DIR)/*.cpp)

HEADERS := $(wildcard $(SRC_DIR)/*.h)

CXXFLAGS := -Wall -Werror -std=c++11
CXXFLAGS += -I./cpprestsdk/Release/include
ifdef DEBUG
CXXFLAGS += -g -DDEBUG
FLAVOR := debug
else
CXXFLAGS += -O3
FLAVOR := release
endif

BUILD_DIR := build/$(FLAVOR)
OBJ_DIR := obj/$(FLAVOR)/$(PROJECT)
BIN := $(BUILD_DIR)/$(PROJECT)


LDFLAGS := -L/usr/local/lib \
		   -L./cpprestsdk/Release/build/Binaries \
           -lcpprest -lboost_system -lssl -lcrypto

SRC_OBJS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: $(BIN)

$(BIN): $(SRC_OBJS)
	mkdir -p $(dir $@)
	$(CXX) $^ $(CPPFLAGS) $(LDFLAGS) -o $@

$(SRC_OBJS): $(OBJ_DIR)/%.o : %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $(CPPFLAGS) $< -o $@


.PHONY: clean
clean:
	-rm -r $(OBJ_DIR)
	-rm $(BIN)
