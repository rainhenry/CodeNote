/*************************************************************
		�ļ�����:main.cpp
		�ļ�����:ʵ����ѶQQ�Ľ�ͼ�Ĺ���
		���뻷��:VS2015 ����WIN32����Ŀ(û���ز�)
			
			����̨����:�޴��ڳ���,�򵥵��㷨��ģ��.
			win32��Ŀ:WindowsӦ�ó���(���ڳ���) ����ϵͳ��API����.
			MFC��Ŀ:  ΢���ṩһ����,����API��װ,�򻯳���Ա��һЩ����.

		����޸�:
			<2017��3��25��>  <��������>  <������ʦ>
**************************************************************/
//ͷ�ļ�
#include <windows.h>
#include "resource.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

int		g_ScreenW, g_ScreenH;
HDC		g_memDc;
RECT	rect;				//��������
bool    isSelect = false;  //���������Ƿ�ѡȡ.
bool    isDown = false;   //���������ǲ��ǰ��µ�


//���������ͼƬ���浽������
void CopyBitmapToClipboard();
//������������ͼ��
void ScreenCaptrue();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);

//������ win32
int WINAPI WinMain(HINSTANCE hInstance,		//��ǰʵ�����
					HINSTANCE prehInstance, //��ǰʵ�����
					LPSTR lpCmdLine,		//�����в���  cmd
					int nShowCmd)			//��ʾ��ʽ
{	
	//1.win32���ڴ���.
	//98.����ԱҪ���ݳ������Ҫ ��ѡ�񴰿ڲ�ͬ������.
	WNDCLASSEX wnd = { 0 };									//����һ��������
	wnd.cbSize = sizeof(WNDCLASSEX);						//�ֽ���
	wnd.hCursor =  LoadCursor(NULL, IDC_ARROW);		 //��ǰĬ�ϵ������ʽ
	wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));//NULL;										 //�����ͼ��
	wnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);  //���ڵ� ������ɫ
	wnd.lpszClassName = L"��ֲ���ʤ��!";					//������
	wnd.cbClsExtra = 0;										//����ĸ����ڴ�
	wnd.cbWndExtra = 0;										//���ڵĸ����ڴ�
	wnd.lpszMenuName = NULL;								//�˵�
	wnd.style = CS_HREDRAW | CS_VREDRAW|CS_DBLCLKS;					 //���ڵķ��
	wnd.hInstance = hInstance; 
	wnd.lpfnWndProc = WndProc;								//���ڵĹ��̺��� ���� ���� ����
	//ע�ᴰ��
	RegisterClassEx(&wnd);

	//���ھ��
	HWND hWnd = CreateWindow(L"��ֲ���ʤ��!", L"��ͼ����",
			WS_POPUP,  //�����Ĵ���
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL,hInstance,NULL);

	ShowWindow(hWnd,SW_MAXIMIZE);
	UpdateWindow(hWnd);
	//PlaySound((LPCWSTR)IDR_WAVE1, NULL, SND_RESOURCE | SND_LOOP | SND_ASYNC);

	//��������  �����˳� ֻ����ĳһ�������²��˳����� ���������˳�
	MSG msg = { 0 };
	while (msg.message!=WM_QUIT)  //�û��������˳���ָ��
	{
		//������Ϣ
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			//������Ϣ
			TranslateMessage(&msg);
			//������Ϣ
			DispatchMessage(&msg);
		}
	}
	return 0;
}

//���ڵĹ��̺��� ���������еĲ���
LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wparam,LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	LOGPEN pen;
	POINT pt;
	pt.x = 2;
	pt.y = 2;

	pen.lopnColor = RGB(255, 0, 0);
	pen.lopnStyle = PS_SOLID;
	pen.lopnWidth = pt;
	HPEN hPen = CreatePenIndirect(&pen);

	LOGBRUSH brush;
	brush.lbStyle = BS_NULL;
	HBRUSH hBrush = CreateBrushIndirect(&brush);

	switch (message)
	{
	case WM_LBUTTONDOWN:  //����������
	{
		if (isSelect == false)
		{
			POINT pt;
			GetCursorPos(&pt);
			rect.left = pt.x; //left top
			rect.top = pt.y;
			rect.right = pt.x;
			rect.bottom = pt.y;
			InvalidateRgn(hWnd, 0, true);

			isDown = true;
		}
	}
		break;

	case WM_LBUTTONUP:  //����������
	{
		if (isSelect == false&&isDown==true)
		{
			POINT pt;
			GetCursorPos(&pt);
			rect.right = pt.x;
			rect.bottom = pt.y;
			InvalidateRgn(hWnd, 0, true);

			isSelect = true;
			isDown = false;
		}
	}
		break;
	case WM_LBUTTONDBLCLK:
		if (isSelect == true)
		{
			CopyBitmapToClipboard();
			isSelect = false;

		}
		PostQuitMessage(0);

		break;

	case WM_MOUSEMOVE:  //��껬��
		
		break;

	case WM_PAINT:  //��ͼ
		hdc = BeginPaint(hWnd, &ps);

		SelectObject(hdc, hBrush);
		SelectObject(hdc, hPen);
		BitBlt(hdc, 0, 0, g_ScreenW, g_ScreenH, g_memDc, 0, 0, SRCCOPY);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		
		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:
		
		//������������ͼ��
		ScreenCaptrue();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);//��������
		break;
	}
	return DefWindowProc(hWnd, message, wparam, lparam);
}
//C�����ж���֪ʶ����?
//C++�ж���֪ʶ����?
//������������Ҫ��Щ֪ʶ��?

//������������ͼ��
void ScreenCaptrue()
{
	HDC disDc = CreateDC(L"DISPLAY", 0, 0, 0);//disDC ����Ļ�ͼ�������
	//����ķֱ��� 1920*1080 (1920.1080) ֻ����1920 1080
	g_ScreenW = GetDeviceCaps(disDc, HORZRES);
	g_ScreenH = GetDeviceCaps(disDc, VERTRES);
	//��Ҫ��һ��������װ ��С ͼ��
	g_memDc = CreateCompatibleDC(disDc);
	//ģ��һ�黭��
	HBITMAP bmp = CreateCompatibleBitmap(disDc, g_ScreenW, g_ScreenH);
	SelectObject(g_memDc, bmp);
	BitBlt(g_memDc, 0, 0, g_ScreenW, g_ScreenH, disDc, 0, 0, SRCCOPY);
}

//���������ͼƬ���浽������
void CopyBitmapToClipboard()
{
	HDC disDc = CreateDC(L"DISPLAY", 0, 0, 0);//disDC ����Ļ�ͼ�������
	//����ķֱ��� 1920*1080 (1920.1080) ֻ����1920 1080
	int rectW = rect.right - rect.left;//GetDeviceCaps(disDc, HORZRES);
	int rectH = rect.bottom - rect.top;//GetDeviceCaps(disDc, VERTRES);
	//��Ҫ��һ��������װ ��С ͼ��
	HDC memdc = CreateCompatibleDC(disDc);
	//ģ��һ�黭��
	HBITMAP bmp = CreateCompatibleBitmap(disDc, rectW, rectH);

	HBITMAP oldmap = (HBITMAP)SelectObject(memdc, bmp);
	BitBlt(memdc, 0, 0, rectW, rectH, disDc, rect.left, rect.top, SRCCOPY);

	HBITMAP newmap = (HBITMAP)SelectObject(memdc, oldmap);

	OpenClipboard(0);		//��������Ǳ���һ������Ա����
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, newmap);
	CloseClipboard();
}

//8880  14����ʽ����.��һ����2�ڵ�����.����.
// 8���� ��ѧ��. 8���� ������ȥ����.
// 8����  ����8����  ����8����. ����.
// 50����.
// ��������  ���ϻ���  ���Ͻ���  ������  ��ջ��.
// 300 13.
// 20:30-23:00  1 3 5������  246����,��ҵ.
// ���� QQ ΢�� �绰  ����  Զ��  һ��һ����.
//