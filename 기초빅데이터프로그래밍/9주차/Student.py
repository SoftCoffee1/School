
class Student:
    
    def __init__(self, num, name, kor, eng, math):
        self._num = num
        self._name = name
        self._kor = int(kor)
        self._eng = int(eng)
        self._math = int(math)
        
        self._total = int(kor) + int(eng) + int(math)
        self._avg = (int(kor) + int(eng) + int(math)) / 3
        self._order = 0
        
        
    
    def get_num(self):
        return self._num
    
    def get_name(self):
        return self._name
    
    def get_kor(self):
        return self._kor
    
    def get_eng(self):
        return self._eng
    
    def get_math(self):
        return self._math
    
    def get_total(self):
        return self._total
    
    def get_avg(self):
        return self._avg
    
    def get_order(self):
        return self._order
    
    def set_order(self, value):
        self._order = value
    
    num = property(get_num)
    name = property(get_name)
    kor = property(get_kor)
    eng = property(get_eng)
    math = property(get_math)
    total = property(get_total)
    avg = property(get_avg)
    order = property(get_order, set_order)
    
