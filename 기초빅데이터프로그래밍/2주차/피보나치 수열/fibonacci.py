
"""
recursive한 방법

재귀함수를 이용하여 피보나치 수를 구할 수 있다. 이때 피보나치 수열을 리스트에 저장해가며 계산을 하면 훨씬 금방 값을 구할 수 있다.
구했던 피보나치 항을 처음부터 다시 계산하지 않고 저장된 값을 불러오는 방법을 이용하는 것이다.
"""

# n번째 피보나치수를 리턴하는 함수
def fibonacci(n):
    
    # n번째 피보나치 수가 이미 계산되었다면 저장되어 있는 수를 불러온다.
    if fiboList[n] != -1:
        return fiboList[n]
    
    # 재귀적으로 함수를 호출한다.
    fiboList[n] = fibonacci(n-1) + fibonacci(n-2)
    return fiboList[n]

# 몇번째 피보나치 수를 구하고 싶은 지 입력받기.
num = int(input("input: "))

# fiboList[i] : i번째 피보나치 수
fiboList = [-1 for _ in range(num+1)]  # -1로 초기화하고 계산해가면서 값을 업데이트 해준다.
fiboList[0] = 0  # 0번째 피보나치 수는 0
fiboList[1] = 1  # 1번째 피보나치 수는 1

print(fibonacci(num))
