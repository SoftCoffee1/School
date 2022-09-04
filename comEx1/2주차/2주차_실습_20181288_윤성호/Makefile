CC = gcc # gcc로 컴파일을 진행한다.
TARGET = animal.exe # 만들어질 파일은 animal.exe이다.
OBJECTS = dog.o blackcow.o turtle.o main.o

# .o 파일이 없으면 .c파일을 자동으로 만들어준다.
$(TARGET) : $(OBJECTS)
	#(CC) -o $(TARGET) $(OBJECTS)

# animal.h 파일이 바뀌어도 오브젝트 파일에 다 반영이 된다.
$(OBJECTS) : animal.h

# .PHONY 를 통해 clean이라는 이름의 파일이 있어도 make clean 명령어를 실행가능하게 만든다.
.PHONY : clean
clean: 
	rm $(OBJECTS) $(TARGET)	