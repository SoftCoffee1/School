
# 문자열 입력 받기
sentence = input("Input :")

# 문자열을 역순으로 배치하기
sentence = sentence[::-1]

# 매번 i번째 문자부터 끝까지 출력하는 반복문
# 매 loop마다 i는 문자열의 길이부터 0까지 1씩 작아진다.
for i in range(len(sentence), -1, -1):
    
    # 파이썬에서 print문은 실행 후 한줄 띄기가 default이다. 띄어쓰기 하지 않기 위해서는 print(sentence, end="")로 해주어야 한다.
    print(sentence[i:])
