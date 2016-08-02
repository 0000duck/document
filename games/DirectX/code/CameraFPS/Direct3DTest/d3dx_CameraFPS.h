//CameraFPS.h	һ�� ��һ�˷Q �z��C
#ifndef _DARK_D3DX_CAMERA_FPS_
#define _DARK_D3DX_CAMERA_FPS_


namespace d3dx
{
	class CameraFPS
	{
	public:
		CameraFPS(void);
		~CameraFPS(void);

		inline void Create(IDirect3DDevice9* device_ptr)
		{
			device_ptr_	=	device_ptr;
		}
	private:
		IDirect3DDevice9*	device_ptr_;

		//�z��C����
		D3DXVECTOR3 up_;
		D3DXVECTOR3 front_;
		D3DXVECTOR3 right_;
		
		//�z��Cλ��
		D3DXVECTOR3 position_;

	public:
		//Ӌ�� ͶӰ���
		void ComputeViewMatrix(D3DXMATRIX* matrix_ptr);
		inline void Refresh()
		{
			D3DXMATRIX matrix;
			ComputeViewMatrix(&matrix);
			device_ptr_->SetTransform(D3DTS_VIEW,&matrix);
		}
		inline void Refresh(D3DXMATRIX& matrix)
		{
			device_ptr_->SetTransform(D3DTS_VIEW,&matrix);
		}


		//�O�Ôz��Cλ��
		inline void Position(const D3DXVECTOR3& position)
		{
			position_	=	position;
		}
		inline D3DXVECTOR3 Position() const 
		{
			return position_;
		}

		inline void Up(const D3DXVECTOR3& up)
		{
			up_	=	up;
		}
		inline D3DXVECTOR3 Up() const 
		{
			return up_;
		}
		inline void Front(const D3DXVECTOR3& front)
		{
			front_	=	front;
		}
		inline D3DXVECTOR3 Front() const 
		{
			return front_;
		}
		inline void Right(const D3DXVECTOR3& right)
		{
			right_	=	right;
		}
		inline D3DXVECTOR3 Right() const 
		{
			return right_;
		}

		//���D
		void RotateUp(const float angle);		//ƫ��
		void RotateRight(float angle);			//��/��ҕ
		void RotateFront(const float angle);	//�L��

		inline void MoveFront(const float step)		//����
		{
			//������ˮƽ����(height) �Ƅ�
			//position_ += D3DXVECTOR3(front_.x, height, front_.z) * step;

			position_ += front_ * step;
		}

		inline void MoveRight(const float step)	//��ҕ
		{
			position_ += right_ * step;
		}

		inline void MoveUp(const float step)	//����
		{
			//���fpsҕ�� ���ܰl���� ��/�� 
			//����Ȼ��ԓ ֻ��׃ y����
			position_.y	+=	step;
		}
	};
};
#endif //_DARK_D3DX_CAMERA_FPS_