//d3dx.cpp
#include"stdafx.h"
#include"d3dx.h"
#include"Resource.h"

#define MAX_LOADSTRING 100
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������


bool d3dx::Create_d3dx_window(IDirect3DDevice9** ppDevice
	,HINSTANCE hApplication
	,WNDPROC wProc
	,int nWidth
	,int nHeight
	,bool bWindow
	,D3DDEVTYPE devType
	)
{
	// ��ʼ��ȫ���ַ���
	LoadString(hApplication, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hApplication, IDC_DIRECT3DTEST, szWindowClass, MAX_LOADSTRING);

	//�]�Դ����
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= wProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hApplication;
	wcex.hIcon			= LoadIcon(hApplication, MAKEINTRESOURCE(IDI_DIRECT3DTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_DIRECT3DTEST);;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);
	
	//�������� �K�@ʾ
	DWORD style	=	WS_EX_TOPMOST;	//플Ӵ���
	style	|=	WS_SYSMENU | WS_MINIMIZEBOX;	//��С�� �P�] ���o
	HWND hwnd	=	CreateWindow(szWindowClass, szTitle
		,style
		,0, 0, nWidth, nHeight
		,0 /*parent hwnd*/, 0 /* menu */, hApplication, 0 /*extra*/); 
	if(!hwnd)
	{
		return false;
	}
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//�_ʼ��ʼ�� Direct3D
	//1	�@ȡIDirect3D9�ӿ� ָ�
	IDirect3D9* pD3d	=	Direct3DCreate9(D3D_SDK_VERSION);
	
	if(!pD3d)
	{
		return false;
	}

	//2 �@ȡӲ������
	D3DCAPS9 caps;
	pD3d->GetDeviceCaps(D3DADAPTER_DEFAULT, devType, &caps);

	int vp = 0;
	if(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp	=	D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp	=	D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	
	//3	��� D3DPRESENT_PARAMETERS
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = nWidth;
	d3dpp.BackBufferHeight           = nHeight;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = bWindow;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	//4 ���� �O��
	HRESULT hr	=	pD3d->CreateDevice(D3DADAPTER_DEFAULT
		,devType
		,hwnd
		,vp
		,&d3dpp
		,ppDevice
		);
	if(FAILED(hr))
	{
		//ʹ��16λ�� ��ԇ
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		hr	=	pD3d->CreateDevice(D3DADAPTER_DEFAULT
			,devType
			,hwnd
			,vp
			,&d3dpp
			,ppDevice
			);
		if(FAILED(hr))
		{
			pD3d->Release();
			return false;
		}
	}

	//ጷ�IDirect3D9�ӿ� ָ�
	pD3d->Release();

	return true;
}