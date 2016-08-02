//driver.h
//�����ĳ�ʼ����ж�ص� ��Ǳ�Ҫ��Ӧ���޸�
#pragma once
#include"all.h"


//����Ա�Զ�����չ�豸�ṹ ��������ͬ�豸����Ϣ
typedef struct _DEVICE_EXTENSION {
	PDEVICE_OBJECT pDevice;
	UNICODE_STRING ustrDeviceName;		//�豸����
	UNICODE_STRING ustrSymLinkName;		//����������
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


// ��������

//�����豸���� ��������
#pragma INIT_MEMORY
NTSTATUS CreateDevice (IN PDRIVER_OBJECT pDriverObject);
//ж�����̺��� �豸ж��ʱI/O�������еĵ���
#pragma PAGE_MEMORY
VOID UnloadDevice (IN PDRIVER_OBJECT pDriverObject);

//irp��ǲ����
//NTSTATUS DispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
#pragma PAGE_MEMORY
NTSTATUS CreateDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
#pragma PAGE_MEMORY
NTSTATUS CloseDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
#pragma PAGE_MEMORY
NTSTATUS ControlDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);

#pragma PAGE_MEMORY
NTSTATUS WriteDirectIn(IN PIRP pIrp,ULONG inSize,ULONG outSize);
#pragma PAGE_MEMORY
NTSTATUS ReadDirectOut(IN PIRP pIrp,ULONG inSize,ULONG outSize);




