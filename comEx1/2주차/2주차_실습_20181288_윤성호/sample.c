#include <stdio.h>

int main(){
    int i;
    double num;

    // i/2의 값이 정수/정수라서 몫을 구하게 된다.
    // 실제 값을 정확히 구하기 위해서는 i/2.0과 같이 해주면 된다.
    for (i = 0; i < 5; i++){
        num=i/2 + i;
        printf("num is %f\n", num);
    }
    return 0;
}