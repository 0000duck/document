//d3dx.cpp
#include"stdafx.h"
#include"d3dx.h"
#include"Resource.h"

#define MAX_LOADSTRING 100
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

//�c ���� ���n�^
IDirect3DVertexBuffer9 *pVertex_buffer	=	0;
IDirect3DIndexBuffer9 *pIndex_buffer	=	0;

extern D3DXVECTOR3 camera_position;
extern D3DXVECTOR3 target_position;
extern D3DXVECTOR3 world_direction;

//D3DX�ṩ�Ď׺��w
LPD3DXMESH pMeshs	[5]		={0,0,0,0,0};
D3DXMATRIX matrix_meshs[5];
D3DXMATRIX matrix_vertex;

struct Vertex
{
	float x_;
	float y_;
	float z_;

	static const DWORD FVF;
	Vertex()
	{
		x_	=	y_	=	z_	=	0.0f;
	}
	Vertex(const float x,const float y,const float z)
	{
		x_	=	x;
		y_	=	y;
		z_	=	z;
	}
};
const DWORD Vertex::FVF	=	D3DFVF_XYZ;

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

void d3dx::Initialization(IDirect3DDevice9* pDevice,int nWidth,int nHeight)
{
	
	//������c���� ��������
	pDevice->CreateVertexBuffer(8 * sizeof(Vertex)
		,D3DUSAGE_WRITEONLY
		,Vertex::FVF
		,D3DPOOL_MANAGED
		,&pVertex_buffer
		,0
		);
	pDevice->CreateIndexBuffer(36 * sizeof(WORD)
		,D3DUSAGE_WRITEONLY
		,D3DFMT_INDEX16
		,D3DPOOL_MANAGED
		,&pIndex_buffer
		,0
		);
	
	//�����c����
	Vertex* pVertexs;
	pVertex_buffer->Lock(0
		,0
		,(void**)&pVertexs
		,0
		);
	pVertexs[0]	=	Vertex(-1.0f,-1.0f,-1.0f);
	pVertexs[1]	=	Vertex(-1.0f,1.0f,-1.0f);
	pVertexs[2]	=	Vertex(1.0f,1.0f,-1.0f);
	pVertexs[3]	=	Vertex(1.0f,-1.0f,-1.0f);
	pVertexs[4]	=	Vertex(-1.0f,-1.0f,1.0f);
	pVertexs[5]	=	Vertex(-1.0f,1.0f,1.0f);
	pVertexs[6]	=	Vertex(1.0f,1.0f,1.0f);
	pVertexs[7]	=	Vertex(1.0f,-1.0f,1.0f);
	pVertex_buffer->Unlock();

	WORD* pIndexs;
	pIndex_buffer->Lock(0
		,0
		,(void**)&pIndexs
		,0
		);
	//front
	pIndexs[0]	=	0;
	pIndexs[1]	=	1;
	pIndexs[2]	=	2;
	
	pIndexs[3]	=	0;
	pIndexs[4]	=	2;
	pIndexs[5]	=	3;

	//back
	pIndexs[6]	=	4;
	pIndexs[7]	=	6;
	pIndexs[8]	=	5;
	
	pIndexs[9]	=	4;
	pIndexs[10]	=	7;
	pIndexs[11]	=	6;
	
	//left
	pIndexs[12]	=	4;
	pIndexs[13]	=	5;
	pIndexs[14]	=	1;
	
	pIndexs[15]	=	4;
	pIndexs[16]	=	1;
	pIndexs[17]	=	0;

	//right
	pIndexs[18]	=	3;
	pIndexs[19]	=	2;
	pIndexs[20]	=	6;
	
	pIndexs[21]	=	3;
	pIndexs[22]	=	6;
	pIndexs[23]	=	7;

	//top
	pIndexs[24]	=	1;
	pIndexs[25]	=	5;
	pIndexs[26]	=	6;
	
	pIndexs[27]	=	1;
	pIndexs[28]	=	6;
	pIndexs[29]	=	2;

	//bottom
	pIndexs[30]	=	4;
	pIndexs[31]	=	0;
	pIndexs[32]	=	3;
	
	pIndexs[33]	=	4;
	pIndexs[34]	=	3;
	pIndexs[35]	=	7;

	pIndex_buffer->Unlock();
	
	//��������׃�Q
	D3DXMatrixTranslation(&matrix_vertex,  0.0f, 0.0f,  0.0f);

	//�O�Ôz��C
	D3DXMATRIX matrix;
	D3DXMatrixLookAtLH(&matrix,&camera_position,&target_position,&world_direction);
	pDevice->SetTransform(D3DTS_VIEW,&matrix);

	//ͶӰ���
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj
		,D3DX_PI * 0.5f
		,(float)nWidth/(float)(nHeight)
		,1.0f
		,1000.0f
		);
	pDevice->SetTransform(D3DTS_PROJECTION,&proj);

	//�O���L�uģʽ
	pDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);


	//D3DXCreate*
	Create(pDevice);
}
void d3dx::Create(IDirect3DDevice9* pDevice)
{
	D3DXCreateTeapot(pDevice
		,&pMeshs[0]
		,0);

	D3DXCreateBox(pDevice
		,2.0f	// width
		,2.0f	// height
		,2.0f	// depth
		,&pMeshs[1]
		,0);

	// cylinder is built aligned on z-axis
	D3DXCreateCylinder(pDevice
		,1.0f	// radius at negative z end
		,1.0f	// radius at positive z end
		,3.0f	// length of cylinder
		,10		// slices
		,10		// stacks
		,&pMeshs[2]
		,0);

	D3DXCreateTorus(pDevice
		,1.0f // inner radius
		,3.0f // outer radius
		,10   // sides
		,10   // rings
		,&pMeshs[3]
		,0);

	D3DXCreateSphere(pDevice
		,1.0f // radius
		,10   // slices
		,10   // stacks
		,&pMeshs[4]
		,0);

	//ƽ��
	D3DXMatrixTranslation(&matrix_meshs[0],  0.0f, 0.0f,  0.0f);
	D3DXMatrixTranslation(&matrix_meshs[1], -5.0f, 0.0f,  5.0f);
	D3DXMatrixTranslation(&matrix_meshs[2],  5.0f, 0.0f,  5.0f);
	D3DXMatrixTranslation(&matrix_meshs[3], -5.0f, 0.0f, -5.0f);
	D3DXMatrixTranslation(&matrix_meshs[4],  5.0f, 0.0f, -5.0f);

}
void d3dx::Release()
{
	if(pVertex_buffer)
	{
		pVertex_buffer->Release();
		pVertex_buffer	=	0;
	}
	if(pIndex_buffer)
	{
		pIndex_buffer->Release();
		pIndex_buffer	=	0;
	}
	for(int i=0;i<_countof(pMeshs);++i)
	{
		pMeshs[i]->Release();
		pMeshs[i]	=	0;
	}
}

void d3dx::Draw_scene(IDirect3DDevice9* pDevice,float time)
{
	//�L�u����
	pDevice->Clear(0,0,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0xffffffff,1.0f,0);

	pDevice->BeginScene();

	pDevice->SetStreamSource(0,pVertex_buffer,0,sizeof(Vertex));
	pDevice->SetIndices(pIndex_buffer);
	pDevice->SetFVF(Vertex::FVF);

	pDevice->SetTransform(D3DTS_WORLD, &matrix_vertex);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);
	int i=0;
	//for(int i=0;i<_countof(pMeshs);++i)
	{
		
		pDevice->SetTransform(D3DTS_WORLD, &matrix_meshs[i]);

		pMeshs[i]->DrawSubset(0);
	}


	pDevice->EndScene();

	pDevice->Present(0,0,0,0);

}