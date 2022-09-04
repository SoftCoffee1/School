#include "Header.h"

// 결과 값이 저장되어있는 이차원 배열을 출력해주는 함수
void printArr(int** arr, int n)
{
    int i,j;
    for(j=0; j<n; j++)
    {
        for(i=0; i<10; i++)
            // 0~9 등장횟수를 출력해준다.
            printf("%d ",*(*(arr+j)+i));
        // 출력 이후에는 다음 testcase출력을 위해 줄바꿈을 해준다.
        printf("\n");
    }
}
