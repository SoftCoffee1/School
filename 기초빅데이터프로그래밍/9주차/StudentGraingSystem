
class StudentGradingSystem:
    
    ## 등록된 학생정보가 저장되는 학생 객체 리스트이다. 이는 클래스 변수로 정의하여 클래스에서 유일하게 존재한다.
    stuObjs = []
    
    total = 0
    kor_total = 0
    eng_total = 0
    math_total = 0
    
    total_avg = 0
    kor_avg = 0
    eng_avg = 0
    math_avg = 0
    
    kor_max = 0
    eng_max = 0
    math_max = 0
    
    kor_max_list = []
    eng_max_list = []
    math_max_list = []
    
    # Student객체를 등록한다. 클래스 변수로 정의된 stuObjs에 객체를 추가해야 하므로 클래스 매서드로 정의하였다.
    @classmethod
    def register_student(cls, stuObj):
        
        ## 등록은 stuObjs에 현재 입력된 학생 객체를 append 하는 것으로 생각할 수 있다.
        cls.stuObjs.append(stuObj)
    
    # 학생들의 등수, 반 전체 성적 정보를 구한다.
    @classmethod
    def process(cls):
        
        cls.stuObjs.sort(key = lambda x : -x.total)
        
        order = 1
        stacked = 1
        prevTotal = -1
        
        for stuObj in cls.stuObjs:
            
            if prevTotal == -1:
                stuObj.order = order
                
            else:
                
                if prevTotal == stuObj.total:
                    stuObj.order = order
                    stacked += 1
                    
                else:
                    order += stacked
                    stuObj.order = order
                    stacked = 1
            
            prevTotal = stuObj.total
            
            
        
        for stuObj in cls.stuObjs:
            
            cls.total += stuObj.total
            cls.kor_total += stuObj.kor
            cls.eng_total += stuObj.eng
            cls.math_total += stuObj.math
            
            
            if cls.kor_max < stuObj.kor:
                cls.kor_max = stuObj.kor
                cls.kor_max_list = []
                cls.kor_max_list.append(stuObj.name)
                
            elif cls.kor_max == stuObj.kor:
                cls.kor_max_list.append(stuObj.name)
                
                
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
                
        
        studentNum = len(cls.stuObjs)
        
        cls.total_avg = cls.total / studentNum
        cls.kor_avg = cls.kor_total / studentNum
        cls.eng_avg = cls.eng_total / studentNum
        cls.math_avg = cls.math_total / studentNum
            
        
    
    
    # 학생들의 정보를 출력한다.
    @classmethod
    def print_students(cls):
        
        for stuObj in cls.stuObjs:
            print(f"번호: {stuObj.num}, 이름: {stuObj.name}, 국어: {stuObj.kor}, 영어: {stuObj.eng}, 수학: {stuObj.math},  총점: {stuObj.total}, , 평균: {stuObj.avg:.2f}, 등수 : {stuObj.order}")
    
    
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
        print(f"국어의 최고점 {cls.kor_max}", end=" ")
        print(*cls.kor_max_list)
        print(f"영어의 최고점 {cls.eng_max}", end=" ")
        print(*cls.eng_max_list)
        print(f"수학의 최고점 {cls.math_max}", end=" ")
        print(*cls.math_max_list)
    
    
