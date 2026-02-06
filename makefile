CXX      := g++
CXXFLAGS := -O2 -std=c++20 -Wall -Wextra -I.
LDLIBS   := -lgmpxx -lgmp


LIB_SRC  := src/conjunctive_grammar/grammar.cc \
            src/conjunctive_grammar/grammar_io.cc \
            src/conjunctive_grammar/symbol_table.cc \
            src/conjunctive_grammar/symbol_names_generator.cc


SOLVER_SRC := src/solver_main.cc \
              src/solver/convolution.cc \
              src/solver/grammar_solver.cc


GRAMMAR_SRC := src/normaliser_main.cc

LIB_OBJ     := $(LIB_SRC:.cc=.o)
SOLVER_OBJ  := $(SOLVER_SRC:.cc=.o)
GRAMMAR_OBJ := $(GRAMMAR_SRC:.cc=.o)


TARGET_SOLVER  := solver
TARGET_GRAMMAR := normaliser


all: $(TARGET_SOLVER) $(TARGET_GRAMMAR)


$(TARGET_SOLVER): $(SOLVER_OBJ) $(LIB_OBJ)
	$(CXX) $(SOLVER_OBJ) $(LIB_OBJ) -o $@ $(LDLIBS)


$(TARGET_GRAMMAR): $(GRAMMAR_OBJ) $(LIB_OBJ)
	$(CXX) $(GRAMMAR_OBJ) $(LIB_OBJ) -o $@ $(LDLIBS)


%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	rm -f $(TARGET_SOLVER) $(TARGET_GRAMMAR) \
	      $(SOLVER_OBJ) $(GRAMMAR_OBJ) $(LIB_OBJ)

.PHONY: all clean