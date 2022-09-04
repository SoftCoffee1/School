
# 3번 문제

## sys.argv를 이용하여 인자를 받아오기 위해 필요한 모듈이다.
import sys

## 정수형 입력 여부 체크를 위한 함수
def isNum(string):
    
    ## for loop로 각각의 char를 확인하며 아스키코드상 "0"과 "9" 사이에 있는지 체크한다.
    for letter in string:
        if not ("0" <= letter <= "9"):
            return False
        
    return True

## 유효한 년도를 입력했는지 여부 판별 함수
def is_valid_year(year):
    
    ## 정수가 아니라면 유효하지 않다.
    if not isNum(year):
        return False
    
    ## year은 1이상의 정수만 유효하다.
    if 1 <= int(year):
        return True
    
    else:
        return False
    
## 유효한 달을 입력했는지 여부 판별 함수
def is_valid_month(month):
    
    ## 정수가 아니라면 유효하지 않다.
    if not isNum(month):
        return False
    
    ## month는 1이상 12이하만 유효하다.
    if 1<= int(month) <= 12:
        return True
    
    else:
        return False
    

## 유효한 일을 입력했는지 여부 판별 함수
## year과 month에 따라서 같은 day가 유효할 수도, 아닐수도 있기때문에 파라미터로 셋다 집어넣어주어야 한다.
def is_valid_day(year, month, day):
    
    ## 정수가 아니라면 유효하지 않다.
    if not isNum(day):
        return False
    
    ## 윤년일때와 아닐때는 2월의 날이 다르다. 
    ## 각 리스트의 첫번째 인덱스값은 dummy value이다.
    DaysInMonth = [0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    DaysInMonthLeap = [0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    
    
    ## 달이 유효하고
    if is_valid_month(month):
        
        ## 윤년이면 
        if is_leap(year):
            
            ## 해당 달에서 나올 수 있는 날이면 유효하다. 
            ## 달을 DaysInMonthLeap 리스트로 체크한다.
            if 1 <= int(day) <= DaysInMonthLeap[int(month)]:
                return True
        
        ## 윤년이 아니면
        else:
            
            ## 해당 달에서 나올 수 있는 날이면 유효하다.
            ## 달을 DaysInMonth 리스트로 체크한다.
            if 1 <= int(day) <= DaysInMonth[int(month)]:
                return True
        
        
    return False

## 입력 날짜의 유효성 검사 함수
def is_valid_date(year, month, day):
    
    ## 년도, 월, 일이 모두 유효하면 유효한 날짜이다.
    if is_valid_year(year) and is_valid_month(month) and is_valid_day(year, month, day):
        return True
    
    else:
        return False



## 유효한 날짜가 입력되면 요일의 이름을 반환하주는 함수
def get_day_name(year, month, day):
    
    ## 유효한 입력이 아니면 False를 반환한다.
    if not is_valid_date(year, month, day):
        return "유효한 입력이 아닙니다."
    
    
    ## 윤년일때와 아닐때는 2월의 날이 다르다. 
    ## 각 리스트의 첫번째 인덱스값은 dummy value이다.
    DaysInMonth = [0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    DaysInMonthLeap = [0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    
    year = int(year)
    month = int(month)
    day = int(day)
    
    
    ######### 년도를 일수로 바꾸기 #########
    
    ## 입력한 년은 모두 지난 것이 아니므로 년도를 일수로 바꿀때는 전년도(year-1)까지만 생각한다.
    year4 = (year-1) // 4
    year100 = (year-1) // 100
    year400 = (year-1) // 400
    
    leapCount = year4 - year100 + year400 ## 윤년을 세서 저장하는 변수
    ordiCount = (year-1) - leapCount      ## 윤년이 아닌 년도를 세서 저장하는 변수
    
    ## 윤년은 2월 29일까지 포함하므로 366일
    ## 윤년이 아니면 365일이다.
    year2Day = leapCount*366 + ordiCount*365
    
    
    ######### 월을 일수로 바꾸기 #########
    month2Day = 0
    
    ## 윤년이면 윤년기준 달로 보기
    if is_leap(year):
        for m in range(month):
            month2Day += DaysInMonthLeap[m]
    
    ## 윤년이 아니면 윤년 아닌 기준 달로 보기
    else:
        for m in range(month):
            month2Day += DaysInMonth[m]
        
    
    ## 서기 1년 1월 1일부터 입력된 날까지의 날 수를 모두 더한 값
    convertedDay = year2Day + month2Day + day
    
    ## 그 값을 7로 나누면 요일이 나온다.
    day_name = convertedDay % 7
    
    
    ## 1년 1월 1일이 월요일이므로, 인덱스 1값을 월요일로 주었다.
    DAYNAME = ["일", "월", "화", "수", "목", "금", "토"]
    
    return DAYNAME[day_name] + "요일"
    


## 윤년 체크 함수
def is_leap(year):
    
    ## 계산하기 위해 정수형으로 바꿔준다.
    year = int(year)
    
    ## 400의 배수이면 윤년이다.
    if year % 400 == 0:
        return True
    
    ## 400의 배수가 아닌 수중에
    else:
        
        ## 100의 배수이면 윤년이 아니다.
        if year % 100 == 0:
            return False
        
        ## 100의 배수도 아닌 수중에
        else:
            
            ## 4의 배수이면 윤년이다.
            if year % 4 == 0:
                return True
    
    ## 나머지는 모두 윤년이 아니다.
    return False


## import해서 사용하는 것이 아니라 직접 호출한다.
if __name__ == "__main__":
    
    try:
        year = sys.argv[1]
        month = sys.argv[2]
        day = sys.argv[3]
    
        result = get_day_name(year, month, day)
        
    except IndexError:
        print("년, 월, 일 순으로 모두 입력해주세요")
        
    else:
        print(result)
        
    
