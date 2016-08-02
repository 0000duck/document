//driver.cpp

#include "Driver.h"
#include<utility.hpp>

#pragma PAGE_MEMORY
ULONG int3_function_address;
ULONG int3_function_address_9;
#pragma INIT_MEMORY
extern "C" NTSTATUS DriverEntry (IN PDRIVER_OBJECT pDriverObject,IN PUNICODE_STRING pRegistryPath	) 
{
	KdPrint(("DriverEntry begin\n"));

	NTSTATUS status;
	
	//��ȡidt����ַ
	IDT idt;
	__asm sidt idt;
	PIDT_ENTRY idt_base=(PIDT_ENTRY)idt.base; 
	ULONG idt_function_address;
	//��ȡ�����жϺ�����ַ
	for (int i=0;i<0x100;i++)
	{
		idt_function_address=idt_base[i].offset_high;
		idt_function_address<<=16;
		idt_function_address|=idt_base[i].offset_low;
		KdPrint(("index=%3X hight=%4X low=%4X address=%8X\n",i,idt_base[i].offset_high,idt_base[i].offset_low,idt_function_address));
	}	

	//���� int 3 �жϵ�ַ
	int3_function_address=idt_base[3].offset_high;
	int3_function_address<<=16;
	int3_function_address|=idt_base[3].offset_low;
	
	int3_function_address_9=int3_function_address+9;
	//hook int 3
	IdtHookInt3();
	


	//ע���豸ж������
	pDriverObject->DriverUnload = UnloadDevice;

	////ע��irp����
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = DispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_READ] = DispatchRoutine;
	
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ControlDispatchRoutine;
	//���������豸����
	status = CreateDevice(pDriverObject);

	KdPrint(("DriverEntry end\n"));
	return status;
}
#pragma INIT_MEMORY
NTSTATUS CreateDevice (IN PDRIVER_OBJECT pDriverObject) 
{
	KdPrint(("CreateDevice begin\n"));

	NTSTATUS status;
	//�豸���� ָ��
	PDEVICE_OBJECT pDevObj;
	//�豸������չ�ṹ ָ��
	PDEVICE_EXTENSION pDevExt;
	
	//�豸����
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName,L"\\Device\\MyDDKDevice");
	
	//�����豸
	status = IoCreateDevice( pDriverObject,			//��������
						sizeof(DEVICE_EXTENSION),	//�豸��չ�ṹ��С
						&(UNICODE_STRING)devName,	//�豸�� �� NULL
						FILE_DEVICE_UNKNOWN,		//�豸���� FILE_DEVICE_UNKNOWN δ֪�����豸����Ϊ��ռ(��ֻ�ܱ�һ��Ӧ�ó���ʹ��) 
						0, TRUE,
						&pDevObj );					//�豸��ַ out

	if (!NT_SUCCESS(status))
		return status;

	//��ֱ�ӵķ�ʽ��д(�Ȳ�ʹ�û�����)
	pDevObj->Flags |= DO_DIRECT_IO;

	//�����չ�ṹ����
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;
	pDevExt->ustrDeviceName = devName;
	
	//����������
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName,L"\\??\\HelloDDK");
	
	//������������
	pDevExt->ustrSymLinkName = symLinkName;
	status = IoCreateSymbolicLink( &symLinkName,&devName );
	
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice( pDevObj );
		return status;
	}
	
	KdPrint(("CreateDevice sucess and end\n"));
	return STATUS_SUCCESS;
}



#pragma PAGE_MEMORY
VOID UnloadDevice (IN PDRIVER_OBJECT pDriverObject) 
{
	KdPrint(("UnloadDevice begin\n"));
	
	//��ȡ�豸���� ���ص��豸���Զ�������һ��������
	PDEVICE_OBJECT	pNextObj = pDriverObject->DeviceObject;
	
	//�����豸����
	while (pNextObj) 
	{
		//��ȡ�豸��չ�ṹ
		PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)pNextObj->DeviceExtension;

		//ɾ����������
		UNICODE_STRING pLinkName = pDevExt->ustrSymLinkName;
		IoDeleteSymbolicLink(&pLinkName);
		
		//ɾ���豸
		IoDeleteDevice(pNextObj);

		//ָ��������һ�ڵ�
		pNextObj = pNextObj->NextDevice;
	}
	IdtUnhookInt3();
	KdPrint(("UnloadDevice end\n"));
}




#pragma PAGE_MEMORY
NTSTATUS DispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp) 
{
	KdPrint(("DispatchRoutine begin\n"));
	
	//...����Ҫʵ�ֵĹ���
	//...���һЩ����
	//...����ʡ��

	NTSTATUS status = STATUS_SUCCESS;
	//�������irp״̬ �Ƿ�ɹ���
	pIrp->IoStatus.Status = status;
	//����irpʵ�ʲ����ֽ���
	pIrp->IoStatus.Information = 0;	// bytes xfered
	//ָʾ��ɴ�irp
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	
	KdPrint(("DispatchRoutine end\n"));
	return status;
}
#pragma PAGE_MEMORY
NTSTATUS CreateDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp)
{
	KdPrint(("CreateDispatchRoutine begin\n"));
	
	NTSTATUS status = STATUS_SUCCESS;
	//...����Ҫʵ�ֵĹ���
	//...���һЩ����
	//...����ʡ��

	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("CreateDispatchRoutine end\n"));
	return status;
}
#pragma PAGE_MEMORY
NTSTATUS CloseDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp)
{
	KdPrint(("CloseDispatchRoutine begin\n"));
	
	NTSTATUS status = STATUS_SUCCESS;
	//...����Ҫʵ�ֵĹ���
	//...���һЩ����
	//...����ʡ��

	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("CloseDispatchRoutine end\n"));
	return status;
}
#pragma PAGE_MEMORY
NTSTATUS ControlDispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp)
{
	KdPrint(("ControlDispatchRoutine begin\n"));
	
	NTSTATUS status = STATUS_NOT_SUPPORTED;
	pIrp->IoStatus.Information=0; //���÷��ص�ʵ�ʲ����ֽ��� ��ʵ��(�ȳ���Ա�Զ�)

	//��irp��ջ�ṹ�� ��ȡ�豸����� irp �ṹ
	PIO_STACK_LOCATION lpIoStack=IoGetCurrentIrpStackLocation(pIrp);

	//����ɹ���ȡ�� irp �ṹ
	if(lpIoStack)
	{
		//�жϿ�����
		//switch(lpIoStack->Parameters.DeviceIoControl.IoControlCode)
		//{
		//case IOCTL_DIRECT_IN://д�絽����
		//	break;
		//case IOCTL_DIRECT_OUT://�����������ݵ��û���
		//	break;
		//}

	}

	//����pIrp״̬ ���������޷�����������������
	pIrp->IoStatus.Status=STATUS_SUCCESS;//���سɹ�
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("ControlDispatchRoutine end\n"));
	return status;
}



#pragma PAGE_MEMORY
void IdtHookInt3()
{
	__asm
	{
		//ȥ��ҳ�汣��
		cli;
			mov eax,cr0;
			and eax,not 10000h; //and eax,0FFFEFFFFh
			mov cr0,eax;

			mov eax,int3_function_address;
			mov BYTE ptr [eax],0xE9;
			inc eax;
			lea ebx,MyInt3;
			sub ebx,int3_function_address;
			sub ebx,5;
			mov [eax],ebx;


		//�ָ�ҳ�汣��
		cli;
			mov eax,cr0;
			or eax,10000h; 
			mov cr0,eax ;
			sti;
	}
}
#pragma PAGE_MEMORY
void IdtUnhookInt3()
{
	__asm
	{
		//ȥ��ҳ�汣��
		cli;
			mov eax,cr0;
			and eax,not 10000h; //and eax,0FFFEFFFFh
			mov cr0,eax;

			mov eax,int3_function_address;	
			lea ebx,int3Hookcode;
			mov ecx,[ebx];
			mov [eax],ecx;
			add ebx,4;
			add eax,4;
			mov ecx,[ebx];
			mov [eax],ecx;
			add ebx,4;
			add eax,4;
			mov bl,[ebx];
			mov byte ptr [eax],bl;
			


		//�ָ�ҳ�汣��
		cli;
			mov eax,cr0;
			or eax,10000h; 
			mov cr0,eax ;
			sti;
	}
}

#pragma PAGE_MEMORY
__declspec(naked)
void MyInt3()
{	
	__asm
	{
		pushad;
	    pushfd;
	} 
	//����������Լ����������˴���
	//��ȡ����������	
	PsGetCurrentProcess();
	//ep+=0x16c;

	//KdPrint(("%d",ep));
	/*if (strcmp((PTSTR)((ULONG)EP+0x16c),"notepad.exe")==0)
	{
		KdPrint(("����"));	
		__asm iretd;
	}*/

	__asm 
	{
		popfd;
		popad;

		//ִ�б�hook��ԭָ��
		push 0;
		mov word ptr [esp+2],0;
		jmp int3_function_address_9;
	 }
}
#pragma PAGE_MEMORY
void __declspec(naked)  int3Hookcode()
{
	__asm
	{
		push 0;
		mov WORD ptr [esp+2], 0;
	}
}