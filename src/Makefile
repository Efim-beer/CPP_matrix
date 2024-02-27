CC = g++ -g --std=c++17   -Wall -Werror -Wextra
# -fsanitize=address
GCOV=-fprofile-arcs -ftest-coverage
OS=$(shell uname)
ifeq ($(OS), Linux)
  LIBS=-lgtest -lpthread -lrt -lm -lsubunit
else
  LIBS=-lgtest
endif

all: clean s21_matrix_oop.a test 

test: 
	$(CC) s21_test.cc s21_matrix_oop.a $(LIBS) -o matrix_test.out
	./matrix_test.out

s21_matrix_oop.a: 
	$(CC) -c s21_matrix_oop.cc -o s21_matrix_oop.o
	ar rcs s21_matrix_oop.a s21_matrix_oop.o

clean:
	rm -f *.o *.a *.gcno *.gcda *.info *.out
	rm -rf ./report

leaks:
	make test
	CK_FORK=no leaks --atExit -- ./matrix_test.out
	make clean

check:
	cp ../materials/linters/.clang-format ./
	clang-format -n *.cc
	clang-format -n *.h
	rm .clang-format

# stylefix:
# 	cp ../materials/linters/.clang-format ./
# 	clang-format -i *.cc
# 	clang-format -i *.h
# 	rm .clang-format

# 	clang-format -i ./*.h ./*.cc
# 	clang-format -n ./*.h ./*.cc

cppcheck:
	cppcheck *.cc
