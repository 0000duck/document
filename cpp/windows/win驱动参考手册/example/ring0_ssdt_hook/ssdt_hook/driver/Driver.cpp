//driver.cpp

#include "Driver.h"

//����KeServiceDescriptorTable��
extern "C" ULONG KeServiceDescriptorTable;
//��¼ԭ������ַ
HookNtOpenProcess realNtOpenProcess;
//��¼���������� pid
HANDLE protected_pid;
//��¼�Ƿ񱻱���
bool isHook=false;

#pragma INIT_MEMORY
extern "C" NTSTATUS DriverEntry (IN PDRIVER_OBJECT pDriverObject,IN PUNICODE_STRING pRegistryPath	) 
{
	KdPrint(("DriverEntry begin\n"));
	//__asm int 3;
	
	NTSTATUS status;

	//ע���豸ж������
	pDriverObject->DriverUnload = UnloadDevice;

	////ע��irp����
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseDispatchRoutine;

	
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ControlDispatchRoutine;
	//���������豸����
	status = CreateDevice(pDriverObject);

	KdPrint(("DriverEntry end\n"));
	return status;
}


#pragma INIT_MEMORY
ULONG GetCurFunctionAddr(ULONG index)
{
	ULONG function_addr;
	__asm
	{
		push eax;
		push ebx;
			mov eax,KeServiceDescriptorTable;
			mov eax,[eax];
			mov ebx,index;	//��������
			shl ebx,2;		//���㺯��ƫ�� ������λ �൱��*4 �� imul ebx,ebx,4 
			add eax,ebx;	
			mov eax,[eax];
			mov function_addr,eax;
		pop ebx;
		pop eax;
	}
	return function_addr;
}

#pragma INIT_MEMORY
ULONG SetCurFunctionAddr(ULONG index,ULONG addr)
{
	ULONG function_addr;
	__asm
	{
		push eax;
		push ebx;

		//ȥ��ҳ�汣��
		cli;
			mov eax,cr0;
			and eax,not 10000h; //and eax,0FFFEFFFFh
			mov cr0,eax;

			mov eax,KeServiceDescriptorTable;
			mov eax,[eax];
			mov ebx,index;	//��������
			shl ebx,2;		//���㺯��ƫ�� ������λ �൱��*4 �� imul ebx,ebx,4 
			add eax,ebx;
			mov ebx,[eax];
			mov function_addr,ebx;

			mov ebx,addr;
			mov [eax],ebx;
		
		//�ָ�ҳ�汣��
		cli;
			mov eax,cr0;
			or eax,10000h; 
			mov cr0,eax ;
			sti;

		pop ebx;
		pop eax;
	}
	return function_addr;
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
	RtlInitUnicodeString(&devName,L"\\Device\\SSDT_HOOK");
	
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
	RtlInitUnicodeString(&symLinkName,L"\\??\\SSDT_HOOK");
	
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
	UnHook();
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
		switch(lpIoStack->Parameters.DeviceIoControl.IoControlCode)
		{
		case IOCTL_DIRECT_IN://д�絽����
			status=WriteDirectIn(pIrp,lpIoStack->Parameters.DeviceIoControl.InputBufferLength,lpIoStack->Parameters.DeviceIoControl.OutputBufferLength);
			break;
		case IOCTL_DIRECT_HOOK://�����������ݵ��û���
			status=STATUS_SUCCESS;
			Hook();
			break;
		case IOCTL_DIRECT_UNHOOK:
			status=STATUS_SUCCESS;
			UnHook();
			break;
		}

	}

	//����pIrp״̬ ���������޷�����������������
	pIrp->IoStatus.Status=STATUS_SUCCESS;//���سɹ�
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("ControlDispatchRoutine end\n"));
	return status;
}


#pragma PAGE_MEMORY
NTSTATUS WriteDirectIn(IN PIRP pIrp,ULONG inSize,ULONG outSize)
{
	KdPrint(("WriteDirectIn begin\n"));
	NTSTATUS status = STATUS_SUCCESS;

	//��ȡ�û��㴫��� ���뻺�������ݿ��� �ĵ�ַ
	PVOID lp_input_buffer=pIrp->AssociatedIrp.SystemBuffer;

	__asm
	{
		push eax;
		mov eax,lp_input_buffer;
		mov eax,[eax];
		mov protected_pid,eax;
		pop eax;
	}
	KdPrint(("WriteDirectIn pid=%d\n",protected_pid));
	
	
	pIrp->IoStatus.Information=4;

	KdPrint(("WriteDirectIn end%d\n"));

	return status;
}


#pragma PAGE_MEMORY
void Hook()
{
	KdPrint(("HOOK begin\n"));
	
	if(!isHook)
	{
		ULONG index=0xbe;
		realNtOpenProcess=(HookNtOpenProcess)SetCurFunctionAddr(index,(ULONG)MyNtOpenProcess);
		isHook=true;
	}
	KdPrint(("HOOK end%d\n"));
	
}

#pragma PAGE_MEMORY
void UnHook()
{
	KdPrint(("UNHOOK begin\n"));
	
	ULONG index=0xbe;
	if(isHook)
	{
		SetCurFunctionAddr(index,(ULONG)realNtOpenProcess);
		isHook=false;
	}
	KdPrint(("UNHOOK end%d\n"));
	
}

#pragma PAGE_MEMORY
extern "C" NTSTATUS __stdcall MyNtOpenProcess( 
	OUT	PHANDLE ProcessHandle, 
	IN	ACCESS_MASK DesiredAccess, 
	IN	POBJECT_ATTRIBUTES ObjectAttributes, 
	IN	PCLIENT_ID ClientId ) 
{ 
	NTSTATUS status=STATUS_SUCCESS; 
	
	//����ԭ NtOpenProcess
	status=(NTSTATUS)realNtOpenProcess( ProcessHandle, DesiredAccess, ObjectAttributes, ClientId ); 	

	//��� NtOpenProcess ���óɹ�
	if(ClientId) 
	{ 
		//��ȡ���򿪽��� pid
		HANDLE pid = ClientId->UniqueProcess; 	 

		// ����Ǳ�������PID����ܾ����ʣ������������Ϊ�� 
		if(pid == protected_pid) 
		{ 
			ProcessHandle=NULL;
			status=STATUS_ACCESS_DENIED;
			//PsLookupProcessByProcessId((ULONG)PID,&EP);
			/*��ȡ���ô˺����� δ�����ṹ �Ի���������
			EP=PsGetCurrentProcess();			 
			KdPrint((" ACESS Process Name  --:%s--   \n",(PTSTR)((ULONG)EP+0x174)));
			*/
		} 
	} 

	return status; 
} 