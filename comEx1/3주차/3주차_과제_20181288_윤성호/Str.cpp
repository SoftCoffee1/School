#include "Str.h"

// Str의 길이가 입력되었다면,
Str::Str(int leng)
{   
    // 만약 입력된 수가 0보다 작거나 같다면 에러메세지를 출력하고 객체 생성을 멈춘다.
    if (leng <= 0){
        cout << "문자열의 길이는 0보다 커야 합니다."<<endl;
        return;
    }

    // len변수에 대입하여 초기화해준다.
    len = leng;

    // 문자열을 저장할 char배열 str을 동적할당해준다(NULL문자까지 고려해야하므로 len+1만큼의 크기의 배열을 생성해준다).
    str = new char[len+1];
}

// Str의 내용이 입력되었다면, 
Str::Str(const char *neyong)
{   
    //길이를 len에 대입하여 초기화해주고, 
    len = strlen(neyong);

    // 문자열을 저정한 char배열 str을 동적할당해주고(문자열을 저장할 배열은 NULL문자를 고려해야하므로 문자열의 길이+1만큼의 크기야아한다.),
    str = new char[len+1];

    //str도 neyong으로 초기화해준다.
    strcpy(str, neyong);
}


// Str의 소멸자
Str::~Str()
{
    // str의 메모리를 해제해준다(배열 메모리 해제이므로 []부분을 추가한다).
    // main함수에서 new를 사용하여 객체 포인터를 만든게 아니므로, main함수가 종료될때 만들어진 객체를 소멸시킨다.
    delete [] str;
}

// Str의 길이를 리턴해주는 함수
int Str::length()
{
    // 리턴값으로 문자열의 길이 len을 준다.
    return len;
}

// Str의 내용을 리턴해주는 함수
char* Str::contents()
{   
    // 리턴값으로 문자열의 내용 str을 준다. 
    return str;
}

// 현재 객체의 문자열과, 입력된 객체 a의 문자열을 비교하기 위한 함수
int Str::compare(class Str& a) 
{
    return strcmp(str,a.contents());
}

// 현재 객체의 문자열과, 입력된 문자열 a를 비교하기 위한 함수
int Str::compare(const char *a)
{
    return strcmp(str,a);
}

// 연산자 오버라이딩과정이다. = 연산자를 사용하여 문자열 a를 현재 객체에 대입해주는 과정을 재정의해준다.
// this = a; --> 이 과정에서 해야할 행동을 정의해준다.
void Str::operator=(const char *a)
{   
    // 현재 str를 삭제해주고(메모리를 해제해주고)
    delete [] str;

    //크기를 맞춰서 새롭게 str을 동적할당해서 생성해준다. 
    str = new char[strlen(a) + 1];

    // 새롭게 대입한 문자열의 크기를 len변수에 대입해주고
    len = strlen(a);

    // 현재 객체에 a객체의 내용을 대입해준다.
    strcpy(str,a);
}

// 연산자 오버라이딩과정이다. = 연산자를 사용하여 객체 a를 현재 객체에 대입해주는 과정을 재정의해준다.
// a = this; --> 이 과정에서 해야할 행동을 정의해준다.
void Str::operator=(class Str& a)
{
    // 현재 str를 삭제해주고
    delete [] str;

    //크기를 맞춰서 새롭게 str을 동적할당해서 생성해준다. 
    str = new char[a.length()+1];

    // 새롭게 대입한 문자열의 크기를 len변수에 대입해주고
    len=a.length();

    // 현재 객체에 a객체의 내용을 대입해준다.
    strcpy(str,a.contents());
    
}