// Direct3DTest.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Direct3DTest.h"
#include<Mmsystem.h>

#include"d3dx.h"
#include<dark\debug\ConsoleDebug.hpp>

//�z��C���D���� ���L
#define STEP		(2*D3DX_PI/100)
//�z��C ��ʼ����
D3DXVECTOR3 camera_position(0.0f,0.0f,-10.0f);
//�z��C �^��λ��
D3DXVECTOR3 target_position(0.0f,0.0f,0.0f);
//����������
D3DXVECTOR3 world_direction(0.0f,1.0f,0.0f);

//�O��ӿ�
IDirect3DDevice9* pDevice; 

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void SurroundY(const float);
void SurroundX(const float);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{	
	//�������� ��ʼ���O�� 
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
	d3dx::Initialization(pDevice,640,480);
	
	//�M����Ϣѭ�h
	MSG msg;
	memset(&msg,0,sizeof(MSG));
	float lastTime = (float)timeGetTime(); 
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

			//Ӌ��r�g�g��
			float currTime  = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;
			
			d3dx::Draw_scene(pDevice,timeDelta);

			lastTime = currTime;	
		}
	}

	d3dx::Release();

	//ጷ��O��
	pDevice->Release();

	return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hWnd);
		else if( wParam == VK_LEFT )
		{
			SurroundY(STEP);
		}
		else if( wParam == VK_RIGHT )
		{
			SurroundY(-STEP);
		}
		else if( wParam == VK_UP )
		{
			SurroundX(STEP);
		}
		else if( wParam == VK_DOWN )
		{
			SurroundX(-STEP);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void SurroundY(const float step)
{	
	//Ӌ�� ���D ���
	D3DXMATRIX matrix;
	D3DXMatrixRotationY(&matrix,step);
	
	//���D ����	
	D3DXVec3TransformCoord(&camera_position,&camera_position,&matrix);
	
	//Ӌ�� �z��C׃�Q ���
	D3DXMatrixLookAtLH(&matrix,&camera_position,&target_position,&world_direction);

	//���O �z��C
	pDevice->SetTransform(D3DTS_VIEW,&matrix);
}

void SurroundX(const float step)
{	
	//Ӌ�� ���D ���
	D3DXMATRIX matrix;
	D3DXMatrixRotationX(&matrix,step);
	
	//���D ����
	D3DXVec3TransformCoord(&camera_position,&camera_position,&matrix);
	//Ӌ�� �z��C׃�Q ���
	D3DXMatrixLookAtLH(&matrix,&camera_position,&target_position,&world_direction);

	//���O �z��C
	pDevice->SetTransform(D3DTS_VIEW,&matrix);

}