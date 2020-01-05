#include "Inputclass.h"

Inputclass::Inputclass()
{
}

Inputclass::Inputclass(const Inputclass&)
{
}

Inputclass::~Inputclass()
{
}

void Inputclass::Initialize()
{
	int i;

	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void Inputclass::KeyDonw(unsigned int input)
{
	//Ű�� ���ȴٸ� �� ���¸� �迭�� ����
	m_keys[input] = true;
	return;
}

void Inputclass::KeyUp(unsigned int input)
{
	//Ű�� �������ٸ� �� ���¸� �迭�� ����
	m_keys[input] = false;
	return;
}

bool Inputclass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}

