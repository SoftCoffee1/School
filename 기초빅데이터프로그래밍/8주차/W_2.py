
# 1번 문제

import datetime
import sys

DAY = ["월요일", "화요일", "수요일", "목요일", "금요일", "토요일", "일요일"]

if __name__ == "__main__":
    
    try:
        year = int(sys.argv[1])
        month = int(sys.argv[2])
        day = int(sys.argv[3])
        day_num = datetime.date(year, month, day).weekday()
        
    except ValueError:
        print("유효한 날짜를 입력해주세요.")
        
    except IndexError:
        print("년, 월, 일 순으로 모두 입력해주세요")
    
    else:
        print(DAY[day_num])
