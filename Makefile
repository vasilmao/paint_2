# -------------------------------------Files------------------------------------
SrcDir  = src

BinDir = bin
IntDir = obj
DepsDir = deps
IncludeDir = src/include


CppSrc = $(notdir $(wildcard $(SrcDir)/*.cpp))

Deps = $(wildcard $(SrcDir)/include/*.h)
# Deps = $(addprefix $(IntDir)/, $(CppSrc:.cpp=.d))

Objs = $(addprefix $(IntDir)/, $(CppSrc:.cpp=.o))

# ------------------------------------Options-----------------------------------
LXXFLAGS = $(shell pkg-config --libs sdl2) -lSDL2_ttf -g
CXXFLAGS = -I $(IncludeDir) -std=c++2a -O2 -Wall -g


# ----------------------------------Make rules----------------------------------
.PHONY: programm
programm: $(BinDir)/a.out

# .PHONY: run
# programm: $(BinDir)/a.out
# 	$(BinDir)/a.out


$(BinDir)/a.out: $(Objs)
	$(CXX) -o $(BinDir)/a.out $(Objs) $(LXXFLAGS)

vpath %.cpp $(SrcDir) $(TestDir)
$(IntDir)/%.o: %.cpp $(Deps) Makefile
	$(CXX) -c $< $(CXXFLAGS) -o $@


.PHONY: init
init:
	mkdir -p bin obj

.PHONY: clean
clean:
	rm -rf $(IntDir)/* $(BinDir)/*
# ----------------------------------Make rules----------------------------------