CXX	= g++

AR = ar -rc

NAME = librtk.a

CXXFLAGS = -std=c++11 -W -Wall -Wextra -I./include -g3

SRC = $(shell find src/ -type f -name '*.cpp' 2>/dev/null)

TEST_SRC = $(shell find tests/ -type f -name '*.cpp' 2>/dev/null)

OBJ = $(SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

DEPENDS := $(patsubst %.cpp,%.d,$(SRC))

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
-include $(DEPENDS)

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $@ $(OBJ)

debug: CXXFLAGS += -g3
debug: all

clean:
	rm -rf $(OBJ)
	rm -rf $(TEST_OBJ)
	rm -rf **/*.gcda
	rm -rf **/*.gcno

fclean: clean
	rm -rf $(NAME)
	rm -rf unit_tests

re: fclean all

tests_run: CXXFLAGS += -lcriterion --coverage -lpthread
tests_run: re $(TEST_OBJ)
	$(CXX) -o unit_tests $(TEST_OBJ) $(CXXFLAGS) -L./ -lrtk
	./unit_tests