//d3dx.cpp
#include"stdafx.h"
#include"d3dx.h"
#include"Resource.h"

//�Զ��x �c ����
const DWORD d3dx::Vertex::FVF	=	D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

//D3DXģ�� ���x
#define MODEL_D3DX_TEAPOT_ID	0

//ģ�� ���x
#define MODEL_TEST_ID	0

//��Դ ���x
#define USING_LIGHT_ONE	0

//�y�� ���x
#define TEXTURES_DEFAULT	0

//����
#define D3DX_WINDOW_TITLE	L"D3DX Window"
#define D3DX_WINDOW_CLASS	L"D3DX Window Class"

d3dx::Device::Device()
{
	device_ptr_		=	NULL;
	vertexs_buffer_ptr_	=	NULL;
	vertexs_buffer_ptr_		=	NULL;
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
	//���� ��c ����
	device_ptr_->CreateVertexBuffer(4 * sizeof(Vertex)
		,D3DUSAGE_WRITEONLY
		,Vertex::FVF
		,D3DPOOL_MANAGED
		,&vertexs_buffer_ptr_
		,0
		);
	
	//�O��ʹ�õ��c��Ϣ
	Vertex* vertexs_ptr;
	vertexs_buffer_ptr_->Lock(0
		,0
		,(void**)&vertexs_ptr
		,0);
	D3DXVECTOR3 point0(-1.0f,-1.0f,0)
		,point1(-1.0f,1.0f,0)
		,point2(1.0f,1.0f,0)
		,point3(1.0f,-1.0f,0)
		,vector
		;
	//Ӌ���c ����
	d3dx::ComputeNormal(&vector,point0,point1,point2);
	
	//Ӌ��y������ �K�O���c����
	vertexs_ptr[0]	=	Vertex(point0,vector,D3DXVECTOR2(0,1));
	vertexs_ptr[1]	=	Vertex(point1,vector,D3DXVECTOR2(0,0));
	vertexs_ptr[2]	=	Vertex(point2,vector,D3DXVECTOR2(1,0));
	vertexs_ptr[3]	=	Vertex(point3,vector,D3DXVECTOR2(1,1));


	vertexs_buffer_ptr_->Unlock();
	
	//���� ���� ����
	device_ptr_->CreateIndexBuffer(2 * 3 * sizeof(WORD)
		,D3DUSAGE_WRITEONLY
		,D3DFMT_INDEX16
		,D3DPOOL_MANAGED
		,&indexs_buffer_ptr_
		,0
		);
	//�������� ��ģ
	WORD* indexs_ptr;
	indexs_buffer_ptr_->Lock(0
		,0
		,(void**)&indexs_ptr
		,0
		);

	indexs_ptr[0]	=	0;
	indexs_ptr[1]	=	1;
	indexs_ptr[2]	=	2;

	indexs_ptr[3]	=	0;
	indexs_ptr[4]	=	2;
	indexs_ptr[5]	=	3;

	indexs_buffer_ptr_->Unlock();

	//��ģ�� �惦�� map
	d3dx::Device::ModelIndex model_index;
	model_index.base_point_			=	0;
	model_index.point_count_		=	4;
	model_index.primitive_count_	=	2;
	
	D3DXMATRIX matrix;
	D3DXMatrixTranslation(&matrix,0,0,0);

	//���|
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = D3DXCOLOR(D3DCOLOR_XRGB(255,0,0));
	mtrl.Diffuse  = D3DXCOLOR(D3DCOLOR_XRGB(255,0,0));
	mtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB(255,0,0));
	mtrl.Emissive = D3DXCOLOR(D3DCOLOR_XRGB(0,0,0));
	mtrl.Power    = 2.0f;

	models_[MODEL_TEST_ID]	=	boost::make_tuple(model_index,matrix,mtrl);

	//�������
	d3dx::ID3DXMeshPtr	mesh_ptr	=	CreateTeapot();
	if(mesh_ptr)
	{
		//��С λ��
		D3DXMATRIX matrix;
		D3DXMatrixTranslation(&matrix,4,0,0);

		//���|
		D3DMATERIAL9 mtrl;
		mtrl.Ambient  = D3DXCOLOR(D3DCOLOR_XRGB(255,0,0));
		mtrl.Diffuse  = D3DXCOLOR(D3DCOLOR_XRGB(255,0,0));
		mtrl.Specular = D3DXCOLOR(D3DCOLOR_XRGB(255,0,0));
		mtrl.Emissive = D3DXCOLOR(D3DCOLOR_XRGB(0,0,0));
		mtrl.Power    = 2.0f;
		
		models_d3dx_[MODEL_D3DX_TEAPOT_ID]	=	boost::make_tuple(mesh_ptr,matrix,mtrl);
	}

	//���üy��
	BOOST_AUTO(texture,CreateTexture(L"dx5_logo.bmp"));
	if(texture)
	{
		textures_[TEXTURES_DEFAULT]	= texture;

		device_ptr_->SetTexture(0,texture.get());
	}

	//�z��C λ��
	D3DXVECTOR3 camera_position(0.0f,0.0f,-5.0f);
	//�z��C �^��λ��
	D3DXVECTOR3 target_position(0.0f,0.0f,0.0f);
	//����������
	D3DXVECTOR3 world_direction(0.0f,1.0f,0.0f);

	//�O�Ôz��C
	matrix;
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
	BOOST_AUTO(light_ptr,d3dx::CreateDirectionLight(D3DXVECTOR3(1.0f, -0.0f, 0.25f),D3DCOLOR_XRGB(255,255,255)));
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
	device_ptr_->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0xffffffff,1.0f,0);
	
	//�L�u����
	device_ptr_->BeginScene();
	
		//�O�� ����Դ
		device_ptr_->SetStreamSource(0,vertexs_buffer_ptr_,0,sizeof(Vertex));
		device_ptr_->SetIndices(indexs_buffer_ptr_);
		device_ptr_->SetFVF(Vertex::FVF);

		//�L�u �c ģ��
		BOOST_AUTO(find,models_.find(MODEL_TEST_ID));
		if(find != models_.end())
		{
			//�O�� ��С λ��
			device_ptr_->SetTransform(D3DTS_WORLD,&find->second.get<D3DX_INDEX_MATRIX>());

			//�O�� ���|
			device_ptr_->SetMaterial(&find->second.get<D3DX_INDEX_MATERIAL>());

			//�L�u
			device_ptr_->DrawIndexedPrimitive(find->second.get<D3DX_INDEX_MODEL>().primitive_type
				,find->second.get<D3DX_INDEX_MODEL>().base_point_
				,0
				,find->second.get<D3DX_INDEX_MODEL>().point_count_
				,0
				,find->second.get<D3DX_INDEX_MODEL>().primitive_count_
				);
		}

		//�L�u ID3DXMesh ģ��
		BOOST_AUTO(d3dx_find,models_d3dx_.find(MODEL_D3DX_TEAPOT_ID));
		if(d3dx_find != models_d3dx_.end())
		{
			//�O�� λ�� ��С
			device_ptr_->SetTransform(D3DTS_WORLD,&d3dx_find->second.get<D3DX_INDEX_MATRIX>());
			
			//�O�ò��|
			device_ptr_->SetMaterial(&d3dx_find->second.get<D3DX_INDEX_MATERIAL>());

			//�L�u
			d3dx_find->second.get<D3DX_INDEX_MODEL>()->DrawSubset(0);
		}
		
		
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

d3dx::ID3DXTexturePtr d3dx::Device::CreateTexture(LPCWSTR file_path)
{
	d3dx::ID3DXTexturePtr rs;

	IDirect3DTexture9* texture_ptr	=	NULL;
	D3DXCreateTextureFromFile(device_ptr_,file_path,&texture_ptr);

	if(texture_ptr)
	{
		rs	=	d3dx::ID3DXTexturePtr(texture_ptr,d3dx::SharedPtrRelease<IDirect3DTexture9>);
	}

	return rs;
}

d3dx::ID3DXMeshPtr d3dx::Device::CreateTeapot(__out  LPD3DXBUFFER *ppAdjacency)
{
	d3dx::ID3DXMeshPtr rs;

	LPD3DXMESH mesh_ptr	=	NULL;
	D3DXCreateTeapot(device_ptr_
		,&mesh_ptr
		,ppAdjacency);

	if(mesh_ptr)
	{
		rs	=	d3dx::ID3DXMeshPtr(mesh_ptr,d3dx::SharedPtrRelease<ID3DXMesh>);
	}

	return rs;
}


boost::shared_ptr<D3DLIGHT9> d3dx::CreateDirectionLight(D3DXVECTOR3& direction, D3DXCOLOR color)
{
	boost::shared_ptr<D3DLIGHT9> light_ptr	=	boost::make_shared<D3DLIGHT9>();
	memset(light_ptr.get(),0,sizeof(D3DLIGHT9));
		
	light_ptr->Type			=	D3DLIGHT_DIRECTIONAL;
	light_ptr->Ambient		=	color * 0.6f;
	light_ptr->Diffuse		=	color;
	light_ptr->Specular		=	color * 0.6f;
	light_ptr->Direction	=	direction;

	return light_ptr;
}
void d3dx::ComputeNormal(D3DXVECTOR3* out,D3DXVECTOR3& p0,D3DXVECTOR3& p1,D3DXVECTOR3& p2)
{
	D3DXVECTOR3 u = p1 - p0;
	D3DXVECTOR3 v = p2 - p0;

	D3DXVec3Cross(out,&u,&v);
	D3DXVec3Normalize(out,out);
}