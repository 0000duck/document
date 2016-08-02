//d3dx.cpp
#include"stdafx.h"
#include"d3dx.h"
#include"Resource.h"

#include"model_scene.h"
#include"model_teapot.h"
	
//��Դ ���x
#define USING_LIGHT_ONE	0


//����
#define D3DX_WINDOW_TITLE	L"D3DX Window"
#define D3DX_WINDOW_CLASS	L"D3DX Window Class"


d3dx::Device::Device()
{
	device_ptr_			=	NULL;
	vertexs_buffer_ptr_	=	NULL;
	vertexs_buffer_ptr_	=	NULL;
}
d3dx::Device::~Device()
{
	Release();
}

bool d3dx::Device::CreateDeviceWindow(HINSTANCE hInstance
	,WNDPROC window_fun
	,int width
	,int height
	,bool show_window
	,D3DDEVTYPE device_type		
	)
{
	width_		=	width;
	height_	=	height;

	//�]�Դ����
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= window_fun;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECT3DTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_DIRECT3DTEST);;
	wcex.lpszClassName	= D3DX_WINDOW_CLASS;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);
	
	//�������� �K�@ʾ
	DWORD style	=	WS_EX_TOPMOST;				//플Ӵ���
	style	|=	WS_SYSMENU | WS_MINIMIZEBOX;	//��С�� �P�] ���o
	HWND hwnd	=	CreateWindow(D3DX_WINDOW_CLASS, D3DX_WINDOW_TITLE
		,style
		,0, 0, width, height
		,0 /*parent hwnd*/, 0 /* menu */, hInstance, 0 /*extra*/); 
	if(!hwnd)
	{
		return false;
	}
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//�_ʼ��ʼ�� Direct3D
	//1	�@ȡIDirect3D9�ӿ� ָ�
	IDirect3D9* direct_ptr	=	Direct3DCreate9(D3D_SDK_VERSION);
	
	if(!direct_ptr)
	{
		return false;
	}

	//2 �@ȡӲ������
	D3DCAPS9 caps;
	direct_ptr->GetDeviceCaps(D3DADAPTER_DEFAULT, device_type, &caps);

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
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = show_window;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	//4 ���� �O��
	HRESULT hr	=	direct_ptr->CreateDevice(D3DADAPTER_DEFAULT
		,device_type
		,hwnd
		,vp
		,&d3dpp
		,&device_ptr_
		);
	if(FAILED(hr))
	{
		//ʹ��16λ�� ��ԇ
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		hr	=	direct_ptr->CreateDevice(D3DADAPTER_DEFAULT
			,device_type
			,hwnd
			,vp
			,&d3dpp
			,&device_ptr_
			);
		if(FAILED(hr))
		{
			direct_ptr->Release();
			return false;
		}
	}

	//ጷ�IDirect3D9�ӿ� ָ�
	direct_ptr->Release();

	//�M��������ʼ������
	return Initialization();
	
}

bool d3dx::Device::Initialization()
{
	D3DXMATRIX matrix;

	//��������
	d3dx::ModlePtr_t modle_ptr = boost::make_shared<ModelScene>();
	modle_ptr->Create(device_ptr_);
	D3DXMatrixTranslation(&matrix,0,1,0);
	modle_ptr->Matrix(matrix);
	models_[D3DX_MODEL_SCENE_ID]	=	modle_ptr;
	
	//�������
	modle_ptr =	boost::make_shared<ModelTeapot>();
	modle_ptr->Create(device_ptr_);
	models_[D3DX_MODEL_TEAPOT_ID]	=	modle_ptr;



	//�z��C λ��
	D3DXVECTOR3 camera_position(0.0f,0.0f,-5.0f);
	//�z��C �^��λ��
	D3DXVECTOR3 target_position(0.0f,0.0f,0.0f);
	//����������
	D3DXVECTOR3 world_direction(0.0f,1.0f,0.0f);

	//�O�Ôz��C
	D3DXMatrixLookAtLH(&matrix,&camera_position,&target_position,&world_direction);
	device_ptr_->SetTransform(D3DTS_VIEW,&matrix);

	//ͶӰ���
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj
		,D3DX_PI * 0.25f		//45�� �^��A��
		,(float)width_/(float)(height_)
		,1.0f
		,1000.0f
		);
	device_ptr_->SetTransform(D3DTS_PROJECTION,&proj);
	
	//�O���L�uģʽ
	//device_ptr_->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);	//ֻ�L�u����
	
	//�O�ù���
	BOOST_AUTO(light_ptr,d3dx::CreateDirectionLight(D3DXVECTOR3(1,-1,1),D3DCOLOR_XRGB(255,255,255)));
	device_ptr_->SetLight(USING_LIGHT_ONE,light_ptr.get());
	device_ptr_->LightEnable(USING_LIGHT_ONE,TRUE);
	lights_[USING_LIGHT_ONE]	=	light_ptr;

	//device_ptr_->SetRenderState(D3DRS_LIGHTING,FALSE);		//���ù���
	device_ptr_->SetRenderState(D3DRS_NORMALIZENORMALS,TRUE);	//Ҏ�������з���
	device_ptr_->SetRenderState(D3DRS_SPECULARENABLE,TRUE);		//�����R���

	return true;
}

void d3dx::Device::EnterMessageLoop()
{
	MSG msg;
	memset(&msg,0,sizeof(MSG));
	DWORD time_begin = GetTickCount(); 
	DWORD time_now;
	DWORD time_interval;

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

			//Ӌ�� �����L�u �r�g�g��
			time_now		=	GetTickCount(); 
			time_interval	=	time_now - time_begin;
			
			DrawScene(time_interval);

			time_begin = time_now;	
		}
	}

}

void d3dx::Device::DrawScene(const DWORD time_interval)
{
	//�������
	device_ptr_->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER ,0xffffffff,1.0f,0);
	
	//�L�u����
	device_ptr_->BeginScene();

		//�L�u ����
		models_[D3DX_MODEL_SCENE_ID]->DrawModel();
		
		//�L�u ���
		models_[D3DX_MODEL_TEAPOT_ID]->DrawModel();

		//�L�u ��� ��Ӱ
		models_[D3DX_MODEL_SCENE_ID]->DrawMirror(*models_[D3DX_MODEL_TEAPOT_ID]);


		//�L�u ��� �Ӱ
		models_[D3DX_MODEL_SCENE_ID]->DrawShadow(*models_[D3DX_MODEL_TEAPOT_ID],D3DXVECTOR4(-1,1,-1,0));

	device_ptr_->EndScene();

	//���Q���n�^�� ��Ļ
	device_ptr_->Present(0,0,0,0);
}

void d3dx::Device::Release()
{

	if(vertexs_buffer_ptr_)
	{
		vertexs_buffer_ptr_->Release();
		vertexs_buffer_ptr_	=	NULL;
	}

	if(indexs_buffer_ptr_)
	{
		indexs_buffer_ptr_->Release();
		indexs_buffer_ptr_	=	NULL;
	}

	if(device_ptr_)
	{
		device_ptr_->Release();
		device_ptr_	=	NULL;
	}


}




