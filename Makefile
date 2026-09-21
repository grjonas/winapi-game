FILES   = \
	main.cpp \
	chess.cpp

OPTIONS = \
	-g \
	-Wall \
	-Wextra \
	-Wno-sign-compare \
	-std=c++17

EXEC = main

all: $(EXEC)
	g++ \
	$(FILES) \
	$(OPTIONS) \
	-o $(EXEC)
