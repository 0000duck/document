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


#define IOCTL_DIRECT_IN  CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_IN_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_DIRECT_HOOK CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_OUT_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_DIRECT_UNHOOK CTL_CODE(FILE_DEVICE_UNKNOWN,0x802,METHOD_OUT_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)

// ��������

//�����豸���� ��������
NTSTATUS CreateDevice (IN PDRIVER_OBJECT pDriverObject);
//ж�����̺��� �豸ж��ʱI/O�������еĵ���
VOID UnloadDevice (IN PDRIVER_OBJECT pDriverObject);


NTSTATUS CreateDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
NTSTATUS CloseDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);
NTSTATUS ControlDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp);

NTSTATUS WriteDirectIn(IN PIRP pIrp,ULONG inSize,ULONG outSize);
void Hook();
void UnHook();

ULONG GetCurFunctionAddr(ULONG index);
ULONG SetCurFunctionAddr(ULONG index,ULONG addr);

//����һ������ָ������
extern "C" typedef NTSTATUS  (__stdcall *HookNtOpenProcess)( 
	OUT	PHANDLE ProcessHandle, 
	IN	ACCESS_MASK DesiredAccess, 
	IN	POBJECT_ATTRIBUTES ObjectAttributes, 
	IN	PCLIENT_ID ClientId ) ;


//HOOK ��������
extern "C" NTSTATUS __stdcall MyNtOpenProcess( 
	OUT	PHANDLE ProcessHandle, 
	IN	ACCESS_MASK DesiredAccess, 
	IN	POBJECT_ATTRIBUTES ObjectAttributes, 
	IN	PCLIENT_ID ClientId );