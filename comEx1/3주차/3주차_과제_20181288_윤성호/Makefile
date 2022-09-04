CC = g++
TARGET = main
OBJECTS = main.o Str.o

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)


$(OBJECTS) : Str.h

.PHONY : clean
clean:
	rm $(TARGET) $(OBJECTS)