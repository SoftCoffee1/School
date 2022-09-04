
"""
iterative한 방법

for문과 변수 3개(aResult, aFront, aEnd)만으로 메모리를 최대한 아껴서 피보나치 수열을 구할 수도 있다.
매 loop마다 aFront(앞의 항), aEnd(뒤의 항)를 더하여 aResult(결과 항)를 구하게 된다.
"""

def fibo(number):

    # 0번째 피보나치 수는 0
    if number == 0:
        return 0

    # 1번째 피보나치 수는 1
    if number == 1:
        return 1

    aFront = 0
    aEnd = 1

    # 앞의 두 수를 더해서 aResult에 저장 후
    # 그 다음 항을 구하기 위해 앞의 두 수를 한칸씩 밀어준다.
    for _ in range(number-1):
        aResult = aFront + aEnd
        aFront = aEnd
        aEnd = aResult

    return aResult
