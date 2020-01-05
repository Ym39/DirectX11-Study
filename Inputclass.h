#pragma once
class Inputclass
{
public:
	Inputclass();
	Inputclass(const Inputclass&);
	~Inputclass();

	void Initialize();

	void KeyDonw(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};

