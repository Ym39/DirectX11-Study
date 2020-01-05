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
	//키가 눌렸다면 그 상태를 배열에 저장
	m_keys[input] = true;
	return;
}

void Inputclass::KeyUp(unsigned int input)
{
	//키가 떼어졌다면 그 상태를 배열에 저장
	m_keys[input] = false;
	return;
}

bool Inputclass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}

