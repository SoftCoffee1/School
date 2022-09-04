#include <iostream>

using namespace std;


class Str{
    private:
        char *str; // string의 내용
        int len; // string의 길이

    public:
        Str(int leng); // leng은 string의 길이
        Str(const char *neyong); // neyong은 초기화할 내용
        ~Str(); // 소멸자

        int length(); // string길이를 리턴하는 함수
        char *contents(); // string의 내용을 리턴하는 함수

        int compare(class Str& a); // 객체 a의 내용을 현재 객체에 strcmp
        int compare(const char *a); // 문자열 a의 내용을 현재 객체에 strcmp

        void operator=(const char *a); // string의 값을 현재 객체에 대입.
        void operator=(class Str& a); // Str의 값을 현재 객체에 대입.
};