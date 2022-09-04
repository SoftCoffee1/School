
## StudentGradingSystem 클래스를 새로운 파일에 정의하였다.
class StudentGradingSystem:
    
    ######## 아래는 클래스 변수로 클래스에서 유일하게 존재하는 변수들이다. #######
    
    ## 등록된 학생정보가 저장되는 학생 객체 리스트이다.
    stuObjs = []
    
    total = 0      # 총점 반평균을 위한 점수의 총합
    kor_total = 0  # 국어 반평균을 위한 국어점수의 총합
    eng_total = 0  # 영어 반평균을 위한 영어점수의 총합
    math_total = 0 # 수학 반평균을 위한 수학점수의 총합
    
    total_avg = 0  # 총점 반평균
    kor_avg = 0    # 국어점수 반평균
    eng_avg = 0    # 영어점수 반평균
    math_avg = 0   # 수학점수 반평균
    
    kor_max = 0    # 수학점수의 최댓값
    eng_max = 0    # 영어점수의 최댓값
    math_max = 0   # 수학점수의 최댓값
    
    kor_max_list = []  # 국어점수 최댓값 명단
    eng_max_list = []  # 영어점수 최댓값 명단
    math_max_list = [] # 수학점수 최댓값 명단
    
    ###################### End of 클래스변수 정의 #####################
    
    # Student객체를 등록한다. 클래스 변수로 정의된 stuObjs에 객체를 추가해야 하므로 클래스 매서드로 정의하였다.
    @classmethod
    def register_student(cls, stuObj):
        
        ## 등록은 stuObjs에 현재 입력된 학생 객체를 append 하는 것으로 생각할 수 있다.
        cls.stuObjs.append(stuObj)
    
    # 학생들의 등수, 반 전체 성적 정보를 구한다.
    @classmethod
    def process(cls):
        
        
        ############################ 등수 구하기 ################################
        
        
        
        ## lambda 함수를 이용하여 학생 객체 리스트를 내림차순 정렬하였다.
        cls.stuObjs.sort(key = lambda x : -x.total)
        
        ## 처음 등수는 1이다.
        order = 1
        
        ## 동점자 수를 저장하는 변수이다.
        stacked = 1
        
        ## 직전 학생의 총점을 저장하는 변수이다.
        prevTotal = -1
        
        
        ## 학생 객체를 하나씩 살펴보며
        for stuObj in cls.stuObjs:
            
            ## 만약 첫번째 학생이라면
            if prevTotal == -1:
                
                ## 1등이다.
                stuObj.order = order
                
            ## 첫번째 학생이 아니라면
            else:
                
                ## 직전 학생과 총점이 같다면
                if prevTotal == stuObj.total:
                    
                    ## 동점자이므로 직전 학생의 등수와 같은 등수를 set 해준다.
                    stuObj.order = order
                    
                    ## 동점자의 수를 1증가시킨다.
                    stacked += 1
                    
                ## 직전 학생과 총점이 다르다면
                else:
                    
                    ## 등수에 동점자수를 더해 현재 학생의 등수를 구하고,
                    order += stacked
                    
                    ## 그 등수를 할당해준다.
                    stuObj.order = order
                    
                    ## 동점자수는 1로 다시 설정해준다.
                    stacked = 1
            
            ## 현재 학생의 총점을 직전 학생의 총점으로 바꿔준다. 
            prevTotal = stuObj.total
            
            
        
        ## 학생 객체를 하나씩 살펴보며
        for stuObj in cls.stuObjs:
            
            ## 학생의 총점, 국어점수, 영어점수, 수학점수를 클래스 변수에 더해준다.
            cls.total += stuObj.total
            cls.kor_total += stuObj.kor
            cls.eng_total += stuObj.eng
            cls.math_total += stuObj.math
            
            
            ## 만약 새로운 국어점수의 최댓값이 등장한다면
            if cls.kor_max < stuObj.kor:
                
                ##국어점수의 최댓값을 업데이트해주고
                cls.kor_max = stuObj.kor
                
                ## 국어점수 명단 리스트를 초기화해주고
                cls.kor_max_list = []
                
                ## 현재 학생을 국어명단 리스트에 추가해준다.
                cls.kor_max_list.append(stuObj.name)
                
            ## 만약 국어점수의 최댓값과 현재 학생의 국어점수가 같다면
            elif cls.kor_max == stuObj.kor:
                
                ## 현재 학생을 국어명단 리스트에 추가해준다.
                cls.kor_max_list.append(stuObj.name)
                
                
            ## 영어, 수학도 국어 점수를 처리하는 과정과 똑같이 처리해준다.
            if cls.eng_max < stuObj.eng:
                cls.eng_max = stuObj.eng
                cls.eng_max_list = []
                cls.eng_max_list.append(stuObj.name)
            
            elif cls.eng_max == stuObj.eng:
                cls.eng_max_list.append(stuObj.name)
                
                
            if cls.math_max < stuObj.math:
                cls.math_max = stuObj.math
                cls.math_max_list = []
                cls.math_max_list.append(stuObj.name)
                
            elif cls.math_max == stuObj.eng:
                cls.math_max_list.append(stuObj.name)
                
                
        ############################# 반 전체 성적 정보 구하기 ################################
        
        ## 학생수를 구한다.
        studentNum = len(cls.stuObjs)
        
        ## 총점의 평균, 국어점수평균, 영어점수평균, 수학점수평균을 구한다.
        cls.total_avg = cls.total / studentNum
        cls.kor_avg = cls.kor_total / studentNum
        cls.eng_avg = cls.eng_total / studentNum
        cls.math_avg = cls.math_total / studentNum
            
        
    
    
    # 학생들의 정보를 출력한다.
    @classmethod
    def print_students(cls):
        
        for stuObj in cls.stuObjs:
            print(f"번호: {stuObj.num}, 이름: {stuObj.name}, 국어: {stuObj.kor}, 영어: {stuObj.eng}, 수학: {stuObj.math},  총점: {stuObj.total}, 평균: {stuObj.avg:.2f}, 등수 : {stuObj.order}")
    
    
    # 반 성적 정보를 출력한다.
    @classmethod
    def print_class_information(cls):
        
        print()
        print(f"총점 반평균: {cls.total_avg:.2f}")
        print(f"국어 반평균: {cls.kor_avg:.2f}")
        print(f"영어 반평균: {cls.eng_avg:.2f}")
        print(f"수학 반평균: {cls.math_avg:.2f}")
        
        print()
        print()
        print("각 과목의 최고점과 명단")
        print(f"국어의 최고점 {cls.kor_max},", end=" ")
        print(*cls.kor_max_list)
        print(f"영어의 최고점 {cls.eng_max},", end=" ")
        print(*cls.eng_max_list)
        print(f"수학의 최고점 {cls.math_max},", end=" ")
        print(*cls.math_max_list)
    
    
