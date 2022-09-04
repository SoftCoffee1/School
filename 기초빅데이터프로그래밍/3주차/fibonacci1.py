

## n번째 까지 출력하기 위해 count 변수를 새롭게 지정하였다.
def Fibo_func(n):
    a,b = 0,1
    count = 0
    while count <= n:
        yield a
        a,b = b, a+b
        count += 1
