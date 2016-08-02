//d3dx.h	һЩd3dx���̵��}�u����
#ifndef _DARK_D3DX_
#define _DARK_D3DX_

#include<boost\noncopyable.hpp>
#include<boost\tuple\tuple.hpp>
#include<boost\smart_ptr.hpp>
#include<boost\typeof\typeof.hpp>
#include<boost\unordered_map.hpp>

#define D3DX_INDEX_MODEL	0
#define D3DX_INDEX_MATRIX	1
#define D3DX_INDEX_MATERIAL 2

namespace d3dx
{
	//D3DX ģ�� ����ָ�
	typedef boost::shared_ptr<ID3DXMesh>			ID3DXMeshPtr;
	//�y�� ����ָ�
	typedef boost::shared_ptr<IDirect3DTexture9>	ID3DXTexturePtr;
	
	//���x�c�Y��
	struct Vertex
	{
		//�c����
		D3DXVECTOR3	pos_;
	
		//�c����
		D3DXVECTOR3 vector_;

		//�y������
		D3DXVECTOR2 texture_;
	
		static const DWORD FVF;
		Vertex()
		{
		}
		Vertex(const D3DXVECTOR3& pos,const D3DXVECTOR3& vector,const D3DXVECTOR2& texture)
		{
			pos_		=	pos;
			vector_		=	vector;
			texture_	=	texture;
		}
	};

	//�O�� class
	class Device
		: boost::noncopyable
	{
	private:
		IDirect3DDevice9*		device_ptr_;
		IDirect3DVertexBuffer9*	vertexs_buffer_ptr_;
		IDirect3DIndexBuffer9*	indexs_buffer_ptr_;

		int width_;
		int height_;
	public:
		Device();
		~Device();

		//�������� IDirect3DDevice9 �ӿ� ��횵�һ�����{�õĳ�ʼ������
		bool CreateDeviceWindow(HINSTANCE hInstance		//���ó�����
			,WNDPROC window_fun							//���ں���
			,int width = 800							//���ڌ�
			,int height = 640							//���ڸ�
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
		
		//����ģ�� �c λ��
		class ModelIndex
		{
		public:
			D3DPRIMITIVETYPE primitive_type;
			INT		base_point_;
			UINT	point_count_;
			UINT	primitive_count_;

			ModelIndex()
			{
				primitive_type		=	D3DPT_TRIANGLELIST;
				base_point_			=	0;
				point_count_		=	0;
				primitive_count_	=	0;
			}
		};
		//���� �c ģ��
		boost::unordered_map<unsigned int,boost::tuple<ModelIndex,D3DXMATRIX,D3DMATERIAL9>> models_;
		

		//���� D3DX �ṩ�� D3DXCreate* ģ��
		boost::unordered_map<unsigned int,boost::tuple<ID3DXMeshPtr,D3DXMATRIX,D3DMATERIAL9>> models_d3dx_;
	
	public:
		//�ṩ����� ID3DXMeshPtr ����
		ID3DXMeshPtr CreateTeapot(__out  LPD3DXBUFFER *ppAdjacency = NULL);

	private:
		//���� ��Դ
		boost::unordered_map<unsigned int,boost::shared_ptr<D3DLIGHT9>> lights_;
		//���� �y��
		boost::unordered_map<unsigned int,ID3DXTexturePtr> textures_;

	public:
		//�����y��
		ID3DXTexturePtr CreateTexture(LPCWSTR file_path);
	};
	
	//���� ���ι���
	//���������
	boost::shared_ptr<D3DLIGHT9> CreateDirectionLight(D3DXVECTOR3& direction, D3DXCOLOR color);


	//shared_ptr �h����
	template<typename T>
	void SharedPtrRelease(T* t)
	{
		t->Release();
		t	=	NULL;
	}

	//�������ǅ^��� ��c����
	void ComputeNormal(D3DXVECTOR3* out,D3DXVECTOR3& p0,D3DXVECTOR3& p1,D3DXVECTOR3& p2);

	
};

#endif