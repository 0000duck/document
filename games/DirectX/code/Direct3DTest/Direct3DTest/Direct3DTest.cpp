// Direct3DTest.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Direct3DTest.h"

#include"d3dx.h"
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{	
	//�������� ��ʼ���O��
	IDirect3DDevice9* pDevice; 
	if(!d3dx::Create_d3dx_window(&pDevice
		,hInstance
		,WndProc
		,640,480
		,true
		,D3DDEVTYPE_HAL
		)
		)
	{
		return FALSE;
	}

	//������ʼ������

	//�M����Ϣѭ�h
	MSG msg;
	memset(&msg,0,sizeof(MSG));
	while(msg.message != WM_QUIT)
	{
		if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			//Idle time �L�u����
			pDevice->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0,10.f,0);

			//���ȴ� ���n�^ ���Q����Ļ
			pDevice->Present(0,0,0,0);
		}
	}

	//ጷ��O��
	pDevice->Release();

	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT: 
			//DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hWnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

