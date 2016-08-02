//d3dx.h	һЩd3dx���̵��}�u����
#ifndef _DARK_D3DX_
#define _DARK_D3DX_


#include"d3dx_utility.h"
#include"d3dx_model_base.h"
#include"d3dx_CameraFPS.h"
#include"ModelX.h"

//���xģ�� id
#define D3DX_MODEL_SCENE_ID			0
#define D3DX_MODEL_TEAPOT_ID		1
#define D3DX_MODEL_BOX_ID			2

#define D3DX_MODEL_XFILE_ID			3

namespace d3dx
{
	//�O�� class
	class Device
		: boost::noncopyable
	{
	private:
		IDirect3DDevice9*		device_ptr_;
		IDirect3DVertexBuffer9*	vertexs_buffer_ptr_;
		IDirect3DIndexBuffer9*	indexs_buffer_ptr_;
		LPD3DXFONT font_ptr;

		int width_;
		int height_;
	public:
		Device();
		~Device();

		//�������� IDirect3DDevice9 �ӿ� ��횵�һ�����{�õĳ�ʼ������
		bool CreateDeviceWindow(HINSTANCE hInstance		//���ó�����
			,WNDPROC window_fun							//���ں���
			,int width = 640							//���ڌ�
			,int height = 480							//���ڸ�
			,bool show_window = true						//���ڻ�
			,D3DDEVTYPE device_type= D3DDEVTYPE_HAL			//ʹ�� Ӳ��/ܛ�� �\��
			);

		//���d -> �Է����{�� IDirect3DDevice9�ӿڵķ���
		inline IDirect3DDevice9* operator->()
		{
			return device_ptr_;
		}
		inline operator bool()
		{
			return device_ptr_ != NULL;
		}

	protected:
		//��ԓ�޸Ĵ˺��� ���e������ ��ʼ������
		//CreateDeviceWindow �ɹ��ᱻ�Ԅ��{��
		bool Initialization();

	public:
		//�M����Ϣѭ�h ��ԓ���{�õĵڶ��� ����
		void EnterMessageLoop();

	protected:
		//��ԓ�޸Ĵ˺��� �Ը�׃�����L�u time_interval �Ǿ��x�ϴ��L�u�� �r�g�g��
		void DrawScene(const DWORD time_interval);
		
		//��ԓ�޸Ĵ˺��� ���e������ �YԴጷŲ���
		void Release();
		
	private:
		//���� ��Դ
		boost::unordered_map<unsigned int,boost::shared_ptr<D3DLIGHT9>> lights_;

	public:
		//���� ģ��
		boost::unordered_map<unsigned int,ModlePtr_t> models_;
		
		//FPS �z��C
		d3dx::CameraFPS camera;


	};	
};

#endif