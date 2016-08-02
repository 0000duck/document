#pragma once
#include"ui_base.h"

#include<deque>
// CTabImageDraw

class CTabImageDraw : public CStatic
	,public ui_base
{
	DECLARE_DYNAMIC(CTabImageDraw)

public:
	typedef struct _TABIMAGE_ITEM_
	{
		std::wstring text;
		boost::shared_ptr<Gdiplus::Bitmap> image_ptr;
		DWORD data;
		
		_TABIMAGE_ITEM_()
		{
			data	=	0;
		}
		_TABIMAGE_ITEM_(const _TABIMAGE_ITEM_& copy)
		{
			text		=	copy.text;
			image_ptr	=	copy.image_ptr;
			data		=	copy.data;
		}
		_TABIMAGE_ITEM_ operator=(const _TABIMAGE_ITEM_& copy)
		{
			text		=	copy.text;
			image_ptr	=	copy.image_ptr;
			data		=	copy.data;
			return *this;
		}
	}TABIMAGE_ITEM,LTABIMAGE_ITEM;

	CTabImageDraw();
	virtual ~CTabImageDraw();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void init_default();
	BOOL create(CWnd* pParentWnd,const RECT& rect,LPCTSTR lpszCaption,UINT nID=0xffff,DWORD dwStyle=0);
	
protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void draw_item(Gdiplus::Graphics& graphics,const UINT index);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	
protected:
	typedef std::deque<TABIMAGE_ITEM> container_t;
	container_t	items;
	UINT scroll;

public:
	inline container_t& get_container()
	{
		return items;
	}
	

	//������� ��С
	UINT get_item_size()const;
	//���خ�ǰ�����@ʾ 헔���
	UINT get_max_show()const;
	//���خ�ǰ�L��λ�� �܉��@ʾ 헔���
	UINT get_need_show()const;
	//�����Ŀλ�� ̎춿͑�^�� ����false
	bool get_item_rect(const UINT index,CRect& rect) const;
	//�����Ŀ �Ƿ�̎춿͑�^��
	bool is_item_draw(const UINT index)const;

	//�����Ŀ���{����B �����Ƿ���Ҫ���L
	bool insert_revision(const UINT _where	//����λ������
		,const UINT count	=	1			//�����˶����
		,const bool is_draw = true			//�����Ҫ���L �Ƿ��Ԅ����L (false ���Ñ��{�����L����)
		);
	//�h���Ŀ���{����B �����Ƿ���Ҫ���L
	bool erase_revision(const UINT _where	//�h��λ������
		,const UINT count	=	1			//�h���˶����
		,const bool is_draw = true			//�����Ҫ���L �Ƿ��Ԅ����L (false ���Ñ��{�����L����)
		);

	UINT get_max_scroll()const;
	UINT scroll_down(const UINT count,const bool is_draw = true);
	UINT scroll_up(const UINT count,const bool is_draw = true);
	UINT scroll_to(const UINT _where);
	inline UINT get_scroll()const
	{
		return scroll;
	}
protected:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//��ǰ�x���
	UINT select;
	//��˸��w�
	UINT hover;
	void hover_revision(const UINT index);

	UINT item_padding;
	bool is_tracking;

	UINT drag_index;
	bool track_left;
public:
	inline void set_item_padding(const UINT padding)
	{
		item_padding	=	padding;
	}
	inline UINT get_item_padding() const
	{
		return item_padding;
	}
	UINT get_point_index(const POINT& point)const;

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);


protected:
	
};


