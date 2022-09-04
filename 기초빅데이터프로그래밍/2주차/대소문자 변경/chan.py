

# 각 문자의 개수를 출력하는 함수
# 대문자는 소문자로, 소문자는 그대로 딕셔너리에 저장하여 출력한다.
def wordCount(word):
    
    # 각 문자의 개수를 저장하는 딕셔너리
    wordDict = {}
    
    for letter in word:
        
        # letter이 대문자인 경우
        if "A" <= letter <= "Z":
            
            # newLetter에 소문자 저장
            newLetter = chr(ord(letter) - ord("A") + ord("a"))
        
        # letter이 대문자가 아닌 경우(소문자이거나 알파벳이 아닌경우 모두 포함)
        else:
            
            # newLetter에 그대로 저장
            newLetter = letter
            
        
        # wordDict에 없으면 item을 1이라고 추가해준다.
        if newLetter not in wordDict:
            wordDict[newLetter] = 1
        
        # wordDict에 있다면 item의 개수를 1 증가시켜준다.
        else:
            wordDict[newLetter] += 1
            
    # 각 문자이 개수를 문제의 출력 형식에 맞게 출력해주기 위한 장치이다.
    # print문에서는 end 옵션을 이용하여 한줄에 모두 출력하였다. 파이썬에서는 print를 한 이후에는 자동으로 개행이 되기 때문이 이를 없애준 것이다.
    for key in wordDict:
        print("(%s : %d)" %(key, wordDict[key]), end="  ")
     
    

# 대소문자 변경하는 함수
def caseSwap(word):
    
    # 결과문자열을 저장하는 변수
    result = ""
    
    for letter in word:
        
        # letter이 대문자인 경우
        if "A" <= letter <= "Z":
            
            # letter과 "A"의 아스키값 차이는 알파벳상 순서를 나타내고, 그 순서를 소문자에서의 순서로 바꿔주는 과정이 "a"의 아스키값을 더해주는 것이다.
            result += chr(ord(letter) - ord("A") + ord("a"))
            
        # letter이 소문자인 경우
        elif "a" <= letter <= "z":
            
            # 위의 방법으로 소문자를 대문자로 변경해준다.
            result += chr(ord(letter) - ord("a") + ord("A"))
            
        # 알파벳 이외의 문자가 입력된경우 
        # 에러 메세지를 result에 저장하고 for문 나오기
        else:
            result = "알파벳만 입력해주세요."
            break
            
    print(result)


while True:
    string = input("Input:")
    
    # STOP을 입력하면 Bye를 출력하고 종료 
    if string == "STOP":
        print("Bye")
        break
    
    # 함수내부에서 return 값을 주지않고 print문을 썼기 때문에 함수가 실행되기만 해도 출력이 된다.
    wordCount(string)
    print()
    caseSwap(string)
