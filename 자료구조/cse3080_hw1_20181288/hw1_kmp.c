#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE_STRING 10000000 // 문자열의 최대 크기
#define MAX_SIZE_PATTERN 3000 // 패턴의 최대 크기

// string과 pattern을 받을 char 배열을 선언
// segmentation fault 에러를 방지하기 위해 global로 선언했다.
// failure 배열을 pattern 최대크기만큼 배정하였고, -1로 초기화를 하였다.
// result배열은 문자열에서의 패턴의 등장 인덱스를 저장하는 배열이다.
char string[MAX_SIZE_STRING];
char pattern[MAX_SIZE_PATTERN];
int failure[MAX_SIZE_PATTERN] = { -1 };
int result[MAX_SIZE_STRING];


// KMP 알고리즘을 진행하기 위한 전처리 과정인 패턴 분석 함수이다.
// ex017을 이용하여 작성하였다.
void fail(char *pat, int *failure){
    int i, n = strlen(pat);
    failure[0] = -1;
    for (int j = 1;j<n;j++){
        i = failure[j-1];
        while (pat[j] != pat[i+1] && (i >= 0)) i = failure[i];
        if (pat[j] == pat[i+1]) failure[j] = i+1;
        else failure[j] = -1;
    }
}

// KMP 알고리즘을 이용하여 문자열 매칭하는 함수이다. ------> time complexity : O(m+n)
void KMP(FILE *resultFp, char *string, char *pat, int *failure){
    int i=0, j=0;
    int lens = strlen(string);
    int lenp = strlen(pat);

    int count = 0; // 패턴 매칭이 일어난 횟수를 저장하는 변수이다.
    
    while (i < lens && j < lenp){
        if (string[i] == pat[j]) { i++; j++; }
        else if (j == 0) i++;
        else j =  failure[j-1]+1;

        // 패턴 매칭이 일어났다면 i-lenp이 매칭 일어난 인덱스이므로 resul배열에 저장해준다.
        if (j == lenp) {
            result[count++] = i-lenp;
            j =  failure[j-1]+1;
        }
    }

    fprintf(resultFp, "%d\n", count);
    for (int i = 0; i < count; i++) fprintf(resultFp, "%d ", result[i]);
    fprintf(resultFp, "\n");
    
}


int main(){

    // 파일 포인터를 선언해준다.
    FILE* stringFp;
    FILE* patternFp;
    FILE* resultFp;

    // string.txt 와 pattern.txt 파일을 연다.
    stringFp = fopen("string.txt", "r");
    patternFp = fopen("pattern.txt", "r");

    //string.txt와 pattern.txt가 모두 존재하지 않는다면 에러메세지를 출력하고 프로그램을 종료시킨다.
    if (stringFp == NULL && patternFp == NULL){
        printf("Error : Both string.txt, pattern.txt don't exist\n");
        exit(1);
    }

    // string.txt가 파일안에 존재하지 않는다면 에러메세지를 출력하고 프로그램을 종료시킨다.
    else if (stringFp == NULL){
        printf("Error : string.txt file doens't exist\n");
        exit(1);
    }

    // pattern.txt가 파일안에 존재하지 않는다면 에러메세지를 출력하고 프로그램을 종료시킨다.
    else if (patternFp == NULL){
        printf("Error : pattern.txt file doens't exist\n");
        exit(1);
    }

    // string과 pattern이 있다면 result_naive.txt 파일을 만들고 쓰기모드로 설정해준다.
    resultFp = fopen("result_kmp.txt", "w");

    // result_kmp.txt가 제대로 생성되지 않았다면 에러메세지를 출력하고 프로그램을 종료시킨다.
    if (resultFp == NULL){
        printf("Error : result_kmp.txt file not made\n");
        exit(1);
    }

    // fgets 함수를 통해 문자열을 string배열에, 패턴을 pattern배열에 저장한다.
    fgets(string, MAX_SIZE_STRING, stringFp);
    fgets(pattern, MAX_SIZE_PATTERN, patternFp);

    // 문자열 매칭 KMP 알고리즘을 수행한다.
    fail(pattern, failure);
    KMP(resultFp, string, pattern, failure);
    printf("Program complete. Result saved to result_kmp.txt\n");

    // 모든 파일을 닫아준다.
    // fclose함수는 성공하면 0을 리턴해주고 실패하면 0이 아닌값을 리턴해준다.
    if (fclose(stringFp) != 0){
        printf("Error : string.txt file close error\n");
        exit(1);
    }
    if (fclose(patternFp) != 0){
        printf("Error : pattern.txt file close error\n");
        exit(1);
    }

    if (fclose(resultFp) != 0){
        printf("Error : result_kmp.txt file close error\n");
        exit(1);
    }


    return 0;
}
