# -------------------------------------Files------------------------------------
SrcDir  = src

BinDir = bin
IntDir = obj
DepsDir = deps
IncludeDir = src/include


#CppSrc = $(notdir $(wildcard $(SrcDir)/*.cpp))
CppSrc = $(patsubst src/%, %, $(shell find $(SrcDir)/ -name '*.cpp'))

Deps = $(wildcard $(SrcDir)/include/*.h)
# Deps = $(addprefix $(IntDir)/, $(CppSrc:.cpp=.d))

Objs = $(addprefix $(IntDir)/, $(CppSrc:.cpp=.o))

# ------------------------------------Options-----------------------------------
LXXFLAGS = $(shell pkg-config --libs sdl2) -lSDL2_ttf -ldl # -fsanitize=address -g 
CXXFLAGS = -I $(IncludeDir) -std=c++2a -O2 -Wall # -fsanitize=address -g 


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
	@mkdir -p $(@D)
	$(CXX) -c $< $(CXXFLAGS) -o $@


.PHONY: init
init:
	mkdir -p bin obj

.PHONY: clean
clean:
	rm -rf $(IntDir)/* $(BinDir)/*
# ----------------------------------Make rules----------------------------------