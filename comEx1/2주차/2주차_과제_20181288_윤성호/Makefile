# gcc를 이용하여 컴파일힌다.
CC = gcc

# 오브젝트파일을 변수로 저장하여 한번에 관리한다.
OBJECTS = main.o count.o printArr.o

# 실행 파일을 TARGET이라는 변수에 저장하여 관리한다.
TARGET = main

# TARGET은 OBJECTS와 header,h에 의존해 있으며, 이들이 있어야 실행 파일을 만든다.
$(TARGET): $(OBJECTS) Header.h
	$(CC) -o $(TARGET) $(OBJECTS)


# 오브젝트파일과 TARGET을 모두 지운다.
clean:
	rm $(OBJECTS) $(TARGET)
