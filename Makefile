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

$(TEST_BIN): $(SRC) $(TEST_SRC) $(LOG_SRC)
	$(CXX) -o $@ $(TEST_SRC) $(CFLAGS) $(LDFLAGS) $(OPT)

.PHONY : clean
clean:
	rm $(TEST_BIN)
