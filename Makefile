#-----------------------------------------------
# Uncomment exactly one of the lines labelled 1. and 2. below
# to switch between compilation modes.
# 1. Debug Mode
OPT ?= -g
# 2. Release Mode
# OPT ?= -O2 -DNDEBUG
#-----------------------------------------------

CXX = g++
CFLAGS  = -Wall --std=c++11
LDFLAGS = -lpthread

PRG = SnowFlake
SRC = $(PRG).h

TEST_BIN = test_$(PRG)
TEST_SRC = test/test_$(PRG).cc

LOG_SRC = utils/lightlog.h
LOG_URL = https://raw.githubusercontent.com/Shenggan/LightLOG/master/lightlog.h

all: $(TEST_BIN)

$(LOG_SRC):
	mkdir -p utils
	wget $(LOG_URL) -O $(LOG_SRC)

$(TEST_BIN): $(LOG_SRC) $(SRC) $(TEST_SRC)
	$(CXX) -o $@ $(TEST_SRC) $(CFLAGS) $(LDFLAGS) $(OPT)

test: $(TEST_BIN)
	./$(TEST_BIN)

.PHONY : clean
clean:
	-rm -f $(TEST_BIN)

clean_deps:
	-rm -rf utils
