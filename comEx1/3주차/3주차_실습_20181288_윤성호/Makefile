CC = g++
TARGET = main
OBJECTS = main.o array.o rangearray.o

$(TARGET) : $(OBJECTS)
		$(CC) -o $(TARGET) $(OBJECTS)

$(OBJECTS) : array.h rangearray.h

.PHONY : clean
clean :
		rm $(TARGET) $(OBJECTS)