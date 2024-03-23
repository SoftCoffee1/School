.SURFFIXES : .cpp .o
CXX = g++

CPPFLAGS = -g

HEDRS = LinkedList.h Stack.h
OBJS = $(SRCS:.cpp=.o)

all : alltype stackQueue

alltype : alltype.o $(HEDRS)
	$(CXX) -o alltype alltype.o

stackQueue : stackQueue.o $(HEDRS)
	$(CXX) -o stackQueue stackQueue.o

.PHONY : clean
clean :
	rm -rf alltype alltype.o stackQueue stackQueue.o