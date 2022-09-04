#include "Header.h"

// 행의 크기가 testcase인 이차원 배열을 생성하고, 각각의 행에 그 안에 0~9까지 등장횟수를 저장하는 배열을 생성하여 저장한다.
// 모든 입력을 마친 후 한번에 출력한다.
// naive하게 1부터 올라가며 각각의 숫자를 일일이 세는 것은 가능은 하나, N의 범위가 10억정도이므로 큰 수에 대해 naive알고리즘을 적용하게 되면
// 아주 오랜 세월이 걸릴 수도 있으므로, 더 효율적인 알고리즘이 필요하다.
// 그 알고리즘은 count.c 파일에 설명해놓았다.

int main(){

    int n, i;

    // testcase의 개수를 입력받는다.
    scanf("%d", &n);

    // testcase를 배열로 만들어서 나중에 for loop로 빼서 쓴다.
    int* testcase = (int*)malloc(sizeof(int)*n);

    // test할 testcase들을 입력한다.
    for(i = 0; i < n; i++){
        scanf("%d", testcase+i);
    }

    // arr는 결과값을 담을 이차원 배열이다.
    int** arr;
    arr = (int**)malloc(sizeof(int*)*n);
    for (i = 0; i < n; i++){
        
        // 이차원 배열의 각각의 값을 0으로 초기화해주기 위해 calloc함수를 이용했다.
        *(arr+i) = (int*)calloc(10, sizeof(int));
    }

    // 각각의 경우마다 Count함수를 수행해 값을 arr에 저장한다.
    for (i = 0; i<n; i++){
        Count(arr, *(testcase+i), i);
    }

    // 만들어진 arr배열을 printArr함수를 이용해 출력한다.
    printArr(arr, n);

    return 0;
}
