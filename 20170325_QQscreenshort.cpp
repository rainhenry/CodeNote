/*************************************************************
		文件名称:main.cpp
		文件描述:实现腾讯QQ的截图的功能
		编译环境:VS2015 创建WIN32的项目(没有素材)
			
			控制台程序:无窗口程序,简单的算法的模拟.
			win32项目:Windows应用程序(窗口程序) 调用系统的API函数.
			MFC项目:  微软提供一个库,大量API封装,简化程序员的一些工作.

		最后修改:
			<2017年3月25日>  <创建工程>  <九夏老师>
**************************************************************/
//头文件
#include <windows.h>
#include "resource.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

int		g_ScreenW, g_ScreenH;
HDC		g_memDc;
RECT	rect;				//矩形区域
bool    isSelect = false;  //矩形区域是否被选取.
bool    isDown = false;   //鼠标的做事是不是按下的


//矩形区域的图片保存到剪贴板
void CopyBitmapToClipboard();
//获得整个桌面的图像
void ScreenCaptrue();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);

//主函数 win32
int WINAPI WinMain(HINSTANCE hInstance,		//当前实例句柄
					HINSTANCE prehInstance, //先前实例句柄
					LPSTR lpCmdLine,		//命令行参数  cmd
					int nShowCmd)			//显示格式
{	
	//1.win32窗口创建.
	//98.程序员要根据程序的需要 来选择窗口不同的属性.
	WNDCLASSEX wnd = { 0 };									//定义一个窗口类
	wnd.cbSize = sizeof(WNDCLASSEX);						//字节数
	wnd.hCursor =  LoadCursor(NULL, IDC_ARROW);		 //当前默认的鼠标样式
	wnd.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));//NULL;										 //程序的图标
	wnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);  //窗口的 背景颜色
	wnd.lpszClassName = L"坚持才是胜利!";					//类名称
	wnd.cbClsExtra = 0;										//程序的附加内存
	wnd.cbWndExtra = 0;										//窗口的附加内存
	wnd.lpszMenuName = NULL;								//菜单
	wnd.style = CS_HREDRAW | CS_VREDRAW|CS_DBLCLKS;					 //窗口的风格
	wnd.hInstance = hInstance; 
	wnd.lpfnWndProc = WndProc;								//窗口的过程函数 声明 定义 调用
	//注册窗口
	RegisterClassEx(&wnd);

	//窗口句柄
	HWND hWnd = CreateWindow(L"坚持才是胜利!", L"截图工具",
			WS_POPUP,  //弹出的窗口
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			NULL, NULL,hInstance,NULL);

	ShowWindow(hWnd,SW_MAXIMIZE);
	UpdateWindow(hWnd);
	//PlaySound((LPCWSTR)IDR_WAVE1, NULL, SND_RESOURCE | SND_LOOP | SND_ASYNC);

	//窗口闪退  正常退出 只有在某一个条件下才退出窗口 否则永不退出
	MSG msg = { 0 };
	while (msg.message!=WM_QUIT)  //用户发出了退出的指令
	{
		//接收消息
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			//处理消息
			TranslateMessage(&msg);
			//发送消息
			DispatchMessage(&msg);
		}
	}
	return 0;
}

//窗口的过程函数 负责处理所有的操作
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
	case WM_LBUTTONDOWN:  //鼠标左键按下
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

	case WM_LBUTTONUP:  //鼠标左键弹起
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

	case WM_MOUSEMOVE:  //鼠标滑动
		
		break;

	case WM_PAINT:  //绘图
		hdc = BeginPaint(hWnd, &ps);

		SelectObject(hdc, hBrush);
		SelectObject(hdc, hPen);
		BitBlt(hdc, 0, 0, g_ScreenW, g_ScreenH, g_memDc, 0, 0, SRCCOPY);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		
		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:
		
		//获得整个桌面的图像
		ScreenCaptrue();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);//结束进程
		break;
	}
	return DefWindowProc(hWnd, message, wparam, lparam);
}
//C语言有多少知识点吗?
//C++有多少知识点吗?
//视屏播放器需要那些知识吗?

//获得整个桌面的图像
void ScreenCaptrue()
{
	HDC disDc = CreateDC(L"DISPLAY", 0, 0, 0);//disDC 桌面的绘图环境句柄
	//桌面的分辨率 1920*1080 (1920.1080) 只能在1920 1080
	g_ScreenW = GetDeviceCaps(disDc, HORZRES);
	g_ScreenH = GetDeviceCaps(disDc, VERTRES);
	//需要用一个东西来装 大小 图像
	g_memDc = CreateCompatibleDC(disDc);
	//模拟一块画布
	HBITMAP bmp = CreateCompatibleBitmap(disDc, g_ScreenW, g_ScreenH);
	SelectObject(g_memDc, bmp);
	BitBlt(g_memDc, 0, 0, g_ScreenW, g_ScreenH, disDc, 0, 0, SRCCOPY);
}

//矩形区域的图片保存到剪贴板
void CopyBitmapToClipboard()
{
	HDC disDc = CreateDC(L"DISPLAY", 0, 0, 0);//disDC 桌面的绘图环境句柄
	//桌面的分辨率 1920*1080 (1920.1080) 只能在1920 1080
	int rectW = rect.right - rect.left;//GetDeviceCaps(disDc, HORZRES);
	int rectH = rect.bottom - rect.top;//GetDeviceCaps(disDc, VERTRES);
	//需要用一个东西来装 大小 图像
	HDC memdc = CreateCompatibleDC(disDc);
	//模拟一块画布
	HBITMAP bmp = CreateCompatibleBitmap(disDc, rectW, rectH);

	HBITMAP oldmap = (HBITMAP)SelectObject(memdc, bmp);
	BitBlt(memdc, 0, 0, rectW, rectH, disDc, rect.left, rect.top, SRCCOPY);

	HBITMAP newmap = (HBITMAP)SelectObject(memdc, oldmap);

	OpenClipboard(0);		//大家你们是被老一代程序员坑了
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, newmap);
	CloseClipboard();
}

//8880  14期正式开班.报一期送2期的名额.试行.
// 8个月 大学生. 8个月 不可能去工作.
// 8个月  送你8个月  再送8个月. 三年.
// 50个人.
// 京东白条  蚂蚁花呗  蚂蚁借呗  分期乐  课栈网.
// 300 13.
// 20:30-23:00  1 3 5上正课  246补课,作业.
// 白天 QQ 微信 电话  短信  远程  一对一讲课.
//