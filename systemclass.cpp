#include "systemclass.h"

SystemClass::SystemClass()
{
}

SystemClass::SystemClass(const SystemClass& other)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	//�Լ��� ���̿� �ʺ� �����ϱ� �� ������ 0���� �ʱ�ȭ
	screenWidth = 0;
	screenHeight = 0;

	//������ api�� ����Ͽ� �ʱ�ȭ
	InitializeWindows(screenWidth, screenHeight);

	//input ��ü�� ����, �� ��ü�� �����κ��� ������ Ű���� �Է��� ó���ϱ� ���� ����
	m_Input = new Inputclass;
	if (!m_Input)
	{
		return false;
	}
	
	//input ��ü�� �ʱ�ȭ�մϴ�.
	m_Input->Initialize();

	//graphics ��ü�� �����մϴ�. �� ��ü�� �� ���ø����̼��� ��� �׷��� ��Ҹ� �׸��� ���� ��
	m_Graphics = new GraphicsClass;
	if (!m_Graphics)
	{
		return false;
	}

	//graphics ��ü�� �ʱ�ȭ
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = NULL;
	}

	if (m_Input)
	{
		delete m_Input;
		m_Input = NULL;
	}

	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	//�޽��� ����ü�� �ʱ�ȭ
	ZeroMemory(&msg, sizeof(MSG));

	//�����κ��� ���� �޽����� ���� ������ ������ ��
	done = false;
	while (!done)
	{
		//������ �޽��� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//�����쿡�� ���ø��ɼ��� ���Ḧ ��û�ϴ� ��� ���������ϴ�.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//�� �ܿ��� Frame �Լ��� ó���մϴ�.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}
	return;
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
     case WM_KEYDOWN:
	 {
		 //Ű�� ���ȴٸ� input��ü�� �� ����� �����Ͽ� ���
		 m_Input->KeyDonw((unsigned int)wparam);
		 return 0;
	 }
	 case WM_KEYUP:
	 {
		 //Ű�� �������ٸ� input ��ü�� �� ����� �����Ͽ� �� Ű�� ������� �մϴ�.
		 m_Input->KeyUp((unsigned int)wparam);
		 return 0;
	 }

	 default:
	 {
		 return DefWindowProc(hwnd, umsg, wparam, lparam);
	 }
	}
}

bool SystemClass::Frame()
{
	bool result;

	//������ esc�� ���� ���ø����̼��� �����ϱ⸦ ���ϴ��� Ȯ���մϴ�.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	//graphics��ü�� �۾��� ó���մϴ�.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::InitializeWindows(int&, int&)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//�ܺ� �����͸� �� ��ü�� ����
	ApplicationHandle = this;

	//�� ���ø����̼��� �ν��Ͻ��� �����ɴϴ�.
	m_hinstance = GetModuleHandle(NULL);

	//���ø����̼��� �̸��� �����մϴ�.
	m_applicationName = L"Engine";

	//������ Ŭ������ �⺻ �������� ����ϴ�.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc; wc.cbClsExtra = 0;
	wc.cbWndExtra = 0; wc.hInstance = m_hinstance; 
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
	wc.lpszMenuName = NULL; wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//������ Ŭ���� ���
	RegisterClassEx(&wc);

	//����� ȭ���� �ػ󵵸� �˾ƿɴϴ�.
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//Ǯ ��ũ�� ��� ������ ���� ���� ȭ�� ������ �մϴ�.
	if (FULL_SCREEN)
	{
		//���� Ǯ��ũ�� ����� ȭ�� ũ�⸦ ����ũ�� ũ�⿡ ���߰� ������ 32bit�� �մϴ�.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSWIDTH;

		//Ǯ��ũ���� �´� ���÷��� ������ �մϴ�.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//�������� ��ġ�� ȭ���� ���� ���� ����ϴ�.
		posX = posY = 0;
	}
	else
	{
		//������ ����� 800*600
		screenWidth = 800;
		screenHeight = 600;

		//â�� ������� �߾ӿ� ������ �մϴ�.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	//������ ���� ������ â�� ����� �� �ڵ��� �����ɴϴ�.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	//�����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� �ݴϴ�.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	//���콺 Ŀ���� ǥ������ �ʽ��ϴ�.
	ShowCursor(false);
	
	return;
}

void SystemClass::ShutdownWindows()
{
	//���콺 Ŀ���� ǥ���մϴ�.
	ShowCursor(true);

	//Ǯ��ũ�� ��带 �������� �� ���÷��� ������ �ٲߴϴ�.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	//â�� �����մϴ�.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//���ø����̼� �ν��Ͻ��� �����մϴ�.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	//�� Ŭ������ ���� �ܺ� ������ ���� ����
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	    case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}

	  
	}
}