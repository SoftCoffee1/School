#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE_STRING 10000000 // 문자열의 최대 크기
#define MAX_SIZE_PATTERN 3000 // 패턴의 최대 크기

// string과 pattern을 받을 char 배열을 선언
// segmentation fault 에러를 방지하기 위해 global로 선언했다.
// result배열은 문자열에서의 패턴의 등장 인덱스를 저장하는 배열이다.
char string[MAX_SIZE_STRING];
char pattern[MAX_SIZE_PATTERN];
int result[MAX_SIZE_STRING];


// 문자열 매칭의 naive한 알고리즘을 구현한 함수.  ------> time complexity : O(mn)
// ex016를 이용하여 작성하였다.
void naive(FILE *resultFp, char *string, char *pat){
    int i, j, start = 0;
    int lasts = strlen(string) - 1;
    int lastp = strlen(pat) - 1;
    int endmatch = lastp;
    
    int count = 0; // 패턴 매칭이 일어난 횟수를 저장하는 변수이다.

    for (i = 0; endmatch <= lasts; endmatch++, start++){
        if (string[endmatch] == pat[lastp])
            for (j = 0, i = start; j < lastp && string[i] == pat[j]; i++, j++);
        
        // 패턴 매칭이 일어났다면 start가 매칭 일어난 인덱스이므로 resul배열에 저장해준다.
        if (j == lastp) {
            result[count++] = start;
            j = 0;
        }
    }
    fprintf(resultFp, "%d\n", count); // result_naive.txt에 패턴 매칭 등장 횟수를 출력해준다.
    for (int i = 0; i < count; i++) fprintf(resultFp, "%d ", result[i]); // result_naive.txt에 패턴 매칭 인덱스를 공백문자와 함께 출력한다.
    fprintf(resultFp, "\n"); // 마지막 줄바꿈문자를 입력해주어 다음 명령어를 입력하기 좋게 만들어준다.
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
    if (stringFp == NULL){
        printf("Error : string.txt file doens't exist\n");
        exit(1);
    }

    // pattern.txt가 파일안에 존재하지 않는다면 에러메세지를 출력하고 프로그램을 종료시킨다.
    if (patternFp == NULL){
        printf("Error : pattern.txt file doens't exist\n");
        exit(1);
    }

    // string과 pattern이 있다면 result_naive.txt 파일을 만들고 쓰기모드로 설정해준다.
    resultFp = fopen("result_naive.txt", "w");

    // result_naive.txt가 제대로 생성되지 않았다면 에러메세지를 출력하고 프로그램을 종료시킨다.
    if (resultFp == NULL){
        printf("Error : result_naive.txt file not made\n");
        exit(1);
    }

    
    // fgets 함수를 통해 문자열을 string배열에, 패턴을 pattern배열에 저장한다.
    fgets(string, MAX_SIZE_STRING, stringFp);
    fgets(pattern, MAX_SIZE_PATTERN, patternFp);

    // 문자열매칭 naive 알고리즘을 수행한다.
    naive(resultFp, string, pattern);
    printf("Program complete. Result saved to result_naive.txt\n");

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
        printf("Error : result_naive.txt file close error\n");
        exit(1);
    }

    return 0;

}
