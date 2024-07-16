#include "GameServer.h"

#include <iostream>



class Base
{
public:
	virtual void show() { std::cout << "Base\n" << std::endl; }
};

class Derived : public Base
{
public:
	virtual void show() { std::cout << "Derived\n" << std::endl; }
};

class Unrelated
{
public:
	void show() { std::cout << "Unrelated\n" << std::endl; }

};

int main()
{
	Base* base = new Base();
	Derived* derived = new Derived();
	Unrelated* unrelated = new Unrelated();

	//static_cast 안전한 타입 변환 
	// upcast
	auto baseFromDerived = static_cast<Base*>(derived);

	// reinterpret 
	//특히, 변환된 포인터를 통해 원래 객체의 메서드를 호출하려고 할 때, 
	//객체의 메모리 레이아웃이 일치하지 않으면 정의되지 않은 동작이 발생할 수 있습니다.
	//Base의 vtable(가상 함수 테이블)이나 다른 내부 구조를 참조하려고 할 때 예기치 않은 결과가 발생합니다.
	auto unrelatedFromBase = reinterpret_cast<Base*>(unrelated);

	// C++에서 가상 함수의 동작 방식 때문입니다. 
	// C++에서 가상 함수는 런타임에 동적 바인딩(dynamic binding)을 통해 호출되므로, 
	// 포인터 타입이 변환되더라도 실제 객체의 타입에 따라 적절한 함수가 호출됩니다.
	baseFromDerived->show();

	unrelatedFromBase->show();

	//안되는 case static_cast로 다른 포인터 형변환 
	// auto baseFromDerived = static_cast<Unrelated*>(derived); -> 컴파일 조차안된다.
	auto baseFromDerived = reinterpret_cast<Unrelated*>(derived); // 컴파일 가능하다 

	//boost::asio::io_context io_context;

	//GameServer gameServer(io_context);
	//
	//gameServer.Init();
	//gameServer.Start();

	//// 비동기 작업을 담당하는 io_context 시작 
	//io_context.run();


	//형변환 궁금 

	return 0;
}