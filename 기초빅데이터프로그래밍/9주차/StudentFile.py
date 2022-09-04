
## Student 클래스를 새로운 파일에 정의하였다.
class Student:
    
    ## 번호, 이름, 국어점수, 영어점수, 수학점수를 입력받아서 인스턴스 변수로 받는다.
    def __init__(self, num, name, kor, eng, math):
        self._num = num   # 번호
        self._name = name # 이름
        self._kor = kor   # 국어점수
        self._eng = eng   # 영어점수
        self._math = math # 수학점수
        
        self._total = kor + eng + math     # 총점
        self._avg = (kor + eng + math) / 3 # 평균
        self._order = 0                    # 등수
        
        
    ## property 클래스를 이용하여 인스턴스 변수에 접근하기 위해 정의한 매서드들이다.
    
    @property
    def num(self):
        return self._num
    
    @property
    def name(self):
        return self._name
    
    @property
    def kor(self):
        return self._kor
    
    @property
    def eng(self):
        return self._eng
    
    @property
    def math(self):
        return self._math
    
    @property
    def total(self):
        return self._total
    
    @property
    def avg(self):
        return self._avg
    
    # 등수의 경우 StudentGradingSystem 클래스의 process 매서드에서 계산하여 새롭게 지정해주어야 하기 때문에 get, set 매서드를 둘다 구현하였다.
    @property
    def order(self):
        return self._order
    
    @order.setter
    def order(self, value):
        self._order = value
