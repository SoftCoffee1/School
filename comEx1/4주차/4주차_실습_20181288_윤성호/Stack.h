#include "LinkedList.h"


//1. 템플릿 클래스로 확장해야함
//2. Stack형식으로 Delete 함수 재정의해야함
//주의: first, current_size는 class의 멤버 변수이기 때문에 this 포인터를 사용해서 가져와야함

//LinkedList class를 상속받음
template <typename T>
class Stack : public LinkedList<T>{
	public:
		Stack();
		virtual bool Delete (T &element);
};

template <typename T>
Stack<T>::Stack(){
	LinkedList<T>();
}

template <class T>
bool Stack<T>::Delete(T &element)
{
	// first가 0이면 false반환
    if(this->first==0)
        return false;
	
    Node<T>* current = this->first;

	// LinkedList와 달리 Stack은 current가 가리키는 곳을 삭제
    element=current->data;
    this->first=current->link;
    delete[] current;
    (this->current_size)--;
    return true;
}