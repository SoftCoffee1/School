#include "Header.h"


// 어떤 수를 구성하는 숫자를 다 arr 배열에 반영해주는 함수
// Count함수에서 루프를 돌떄마다 십진수 수를 오른쪽으로 밀어주기때문에(일의자리를 삭제)
// 실제 세야할 숫자의 개수는 10배로 늘어난다.
// 이를 반영해준 변수가 times이다.
void countEach(int** arr, int page, int n, int times){

    // 페이지수를 now변수에 옮겨준다.
    int now = page;
    
    // 현재 수를 구성하는 수를 다 체크할때까지 루프를 돈다.
    while (now != 0){

        // temp에는 현재 수의 일의자리를 저장한다.
        int temp = now%10;

        // 일의자리가 temp인 수는 현재 times개가 추가되어야한다.
        arr[n][temp] += times;

        // 다음 루프를 위해 수를 오른쪽으로 밀어주는 과정(일의자리 삭제과정)
        now /= 10;
    }

    return;
}

// 책 페이지가 page인 책을 구성하는 각 숫자들의 갯수를 알아내는 함수
// 끝 번호를 9로 맞춘다면, 일의자리를 구성하는 숫자들은 십의자리까지의 정보로 쉽게 구할 수 있다.
// 예를들어 1부터 19까지라고 한다면, 일의자리를 구성하는 숫자들은 1~9까지 2개씩있는데 이는 ((19의 십의자리까지의 수 = 1) + 1) = 2로 구할 수 있다. 
// 이때 0은 1개가 있으므로 0정보를 담는 arr의 부분은 times를 빼주면 된다.
// times를 빼주는 이유는 루프를 돎에따라 수를 오른쪽으로 밀게되면 0의 개수는 10배씩 더해지기때문에 그를 빼주어야 원하는 값을 구할 수 있다. 
void Count(int** arr, int page, int n){
    
    // 현재 페이지 수를 now변수에 옮겨준다.
    int now = page;

    // 매 루프를 돌때 가장 작은 자리수의 값을 확인해준다. 
    int times = 1;

    while ( now >= 1){

        // 일의자리 수를 9로 맞춰주는 과정
        if (now%10 != 9){

            // 일의자리가 9가 아니라면 그 수는 일일이 arr변수에 반영해준다.
            countEach(arr, now, n, times);

            // 페이지수의 일의자리가 9가 나올때까지 그 과정을 반복한다.
            now--;

            // 루프의 처음으로 돌아가서 다시 과정을 수행한다.
            continue;
        }


        // now가 줄어들면서 1보다 작게된다면 과정이 끝났으므로 루프를 빠져나온다.
        // 한자리수의 경우에도 9가 아닌이상 이 부분에서 루프가 종료된다.
        if (now<1) break;

        // 일의자리수를 없애주는 과정이다.
        now /= 10;

        // 0~9의 개수는 (십의자리까지의 수 + 1) * times인데
        // times는 수를 오른쪽으로 민 수의 지수로 하는 10의 거듭제곱이라고 생각하면 된다.
        for (int i = 0; i < 10; i++){
            arr[n][i] += (now+1)*times;
        }

        // 0은 한번 더 arr에 추가해줬기 때문에 정확한 값을 구하기 위해서 빼주었다.
        arr[n][0] -= times;

        // 수를 오른쪽으로 한번 밀었기 때문에 times에 10을 곱한 후 과정을 진행한다.
        times *= 10;
    }
    return;
}