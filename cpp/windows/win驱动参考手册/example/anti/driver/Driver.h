//driver.h
#pragma once
#include"all.h"


//����Ա�Զ�����չ�豸�ṹ ��������ͬ�豸����Ϣ
typedef struct _DEVICE_EXTENSION {
	PDEVICE_OBJECT pDevice;
	UNICODE_STRING ustrDeviceName;	//�豸����
	UNICODE_STRING ustrSymLinkName;	//����������
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

//ֱ�Ӷ�д
#define IOCTL_DIRECT_IN  CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_IN_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_DIRECT_OUT CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_OUT_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)



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

NTSTATUS WriteDirectIn(IN PIRP pIrp,ULONG inSize,ULONG outSize);
NTSTATUS ReadDirectOut(IN PIRP pIrp,ULONG inSize,ULONG outSize);

//��ʼ������
VOID initData();


