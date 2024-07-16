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

	//static_cast ������ Ÿ�� ��ȯ 
	// upcast
	auto baseFromDerived = static_cast<Base*>(derived);

	// reinterpret 
	//Ư��, ��ȯ�� �����͸� ���� ���� ��ü�� �޼��带 ȣ���Ϸ��� �� ��, 
	//��ü�� �޸� ���̾ƿ��� ��ġ���� ������ ���ǵ��� ���� ������ �߻��� �� �ֽ��ϴ�.
	//Base�� vtable(���� �Լ� ���̺�)�̳� �ٸ� ���� ������ �����Ϸ��� �� �� ����ġ ���� ����� �߻��մϴ�.
	auto unrelatedFromBase = reinterpret_cast<Base*>(unrelated);

	// C++���� ���� �Լ��� ���� ��� �����Դϴ�. 
	// C++���� ���� �Լ��� ��Ÿ�ӿ� ���� ���ε�(dynamic binding)�� ���� ȣ��ǹǷ�, 
	// ������ Ÿ���� ��ȯ�Ǵ��� ���� ��ü�� Ÿ�Կ� ���� ������ �Լ��� ȣ��˴ϴ�.
	baseFromDerived->show();

	unrelatedFromBase->show();

	//�ȵǴ� case static_cast�� �ٸ� ������ ����ȯ 
	// auto baseFromDerived = static_cast<Unrelated*>(derived); -> ������ �����ȵȴ�.
	auto baseFromDerived = reinterpret_cast<Unrelated*>(derived); // ������ �����ϴ� 

	//boost::asio::io_context io_context;

	//GameServer gameServer(io_context);
	//
	//gameServer.Init();
	//gameServer.Start();

	//// �񵿱� �۾��� ����ϴ� io_context ���� 
	//io_context.run();


	//����ȯ �ñ� 

	return 0;
}