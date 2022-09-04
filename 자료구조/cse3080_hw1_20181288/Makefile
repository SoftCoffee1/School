all : hw1_kmp hw1_naive

hw1_kmp : hw1_kmp.o
	gcc -o hw1_kmp hw1_kmp.o

hw1_naive : hw1_naive.o
	gcc -o hw1_naive hw1_naive.o

.PHONY : clean
clean:
	rm hw1_kmp hw1_naive hw1_kmp.o hw1_naive.o result_kmp.txt result_naive.txt