//driver.h
#pragma once


#ifdef __cplusplus	//����c++���� ��c���ŷ�ʽ����
extern "C"
{
#endif

#include <NTDDK.h>

#ifdef __cplusplus
}
#endif 

#define PAGE_MEMORY code_seg("PAGE")		//��ҳ�ڴ�
#define LOCKED_MEMORY code_seg()			//�Ƿ�ҳ
#define INIT_MEMORY code_seg("INIT")		//INIT�ڴ� ���ڴ�ָ��ֻ�ڼ�������ʱ���� �������سɹ����Զ�ж��

//����Ա�Զ�����չ�豸�ṹ ��������ͬ�豸����Ϣ
typedef struct _DEVICE_EXTENSION {
	PDEVICE_OBJECT pDevice;
	UNICODE_STRING ustrDeviceName;	//�豸����
	UNICODE_STRING ustrSymLinkName;	//����������
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

// ��������

//�����豸���� ��������
NTSTATUS CreateDevice (IN PDRIVER_OBJECT pDriverObject);
//ж�����̺��� �豸ж��ʱI/O�������еĵ���
VOID UnloadDevice (IN PDRIVER_OBJECT pDriverObject);

//irp��ǲ����
NTSTATUS DispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);


NTSTATUS CreateDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
NTSTATUS CloseDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
NTSTATUS ControlDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);


//IDT ��ṹ����
#pragma pack(push)
#pragma pack(1) //
typedef struct _IDT //IDT��ַ
{
	USHORT limit; //��Χ ռ8λ
	ULONG base; //����ַ ռ32λ _IDT_ENTRY����ָ��
}IDT,*PIDT;

typedef struct _IDT_ENTRY
{
	USHORT offset_low; //�жϴ�������ַ��16λ
	USHORT selector;
	UCHAR  reserved;
	UCHAR  type:4; 
    UCHAR  always0:1;
	UCHAR  dpl:2;
	UCHAR  present:1;
	USHORT offset_high;//�жϴ�������ַ��16λ
}IDT_ENTRY,*PIDT_ENTRY;
#pragma pack(pop)

void IdtHookInt3();
void IdtUnhookInt3();
void MyInt3();
void int3Hookcode();