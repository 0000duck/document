//driver.cpp

#include "Driver.h"
#include "control.h"


#pragma INIT_MEMORY
extern "C" NTSTATUS DriverEntry (IN PDRIVER_OBJECT pDriverObject,IN PUNICODE_STRING /*pRegistryPath*/	) 
{
	KdPrint(("DriverEntry begin\n"));
	NTSTATUS status;
	
	//ע���豸ж������
	pDriverObject->DriverUnload = UnloadDevice;

	////ע��irp����
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = CreateDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = CloseDispatchRoutine;
	//pDriverObject->MajorFunction[IRP_MJ_WRITE] = DispatchRoutine;
	//pDriverObject->MajorFunction[IRP_MJ_READ] = DispatchRoutine;
	
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ControlDispatchRoutine;
	//���������豸����
	status = CreateDevice(pDriverObject);
	
	//��ʼ���Զ������
	InitDriver();

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
	RtlInitUnicodeString(&devName,DRIVER_NAME);
	
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
	//�����ڴ� �����豸��
	USHORT length=wcslen(DRIVER_NAME)*sizeof(WCHAR);
	pDevExt->ustrDeviceName.Buffer=(PWCH)ExAllocatePool(PagedPool,length);
	pDevExt->ustrDeviceName.Length=length;
	pDevExt->ustrDeviceName.MaximumLength=length;
	RtlCopyUnicodeString(&pDevExt->ustrDeviceName,&devName);
	
	//����������
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName,DRIVER_LINK_NAME);
	
	//������������
	pDevExt->ustrSymLinkName = symLinkName;
	status = IoCreateSymbolicLink( &symLinkName,&devName );
	
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice( pDevObj );
		return status;
	}
	//�����ڴ� ���� ����������
	length=wcslen(DRIVER_LINK_NAME)*sizeof(WCHAR);
	pDevExt->ustrSymLinkName.Buffer=(PWCH)ExAllocatePool(PagedPool,length);
	pDevExt->ustrSymLinkName.Length=pDevExt->ustrSymLinkName.MaximumLength=length;
	RtlCopyUnicodeString(&pDevExt->ustrSymLinkName,&symLinkName);
	
	KdPrint(("CreateDevice sucess and end\n"));
	return STATUS_SUCCESS;
}



#pragma PAGE_MEMORY
VOID UnloadDevice (IN PDRIVER_OBJECT pDriverObject) 
{
	KdPrint(("UnloadDevice begin\n"));
	
	//��ȡ�豸���� ���ص��豸���Զ�������һ��������
	PDEVICE_OBJECT	pNextObj = pDriverObject->DeviceObject
		,pTmpObj;

	//�����豸����
	while (pNextObj) 
	{
		//��ȡ�豸��չ�ṹ
		PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)pNextObj->DeviceExtension;

		//�ͷű����豸�����ڴ�
		ExFreePool(pDevExt->ustrDeviceName.Buffer);
		
		//ɾ����������
		IoDeleteSymbolicLink(&pDevExt->ustrSymLinkName);
		
		//�ͷű�����������ڴ� 
		ExFreePool(pDevExt->ustrSymLinkName.Buffer);
		
		//��ȡ������һ�ڵ�
		pTmpObj= pNextObj->NextDevice;
		//ɾ���豸
		IoDeleteDevice(pNextObj);

		//ָ��������һ�ڵ�
		pNextObj = pTmpObj;
	}
	
	UnloadDriver();
	KdPrint(("UnloadDevice end\n"));
}




//#pragma PAGE_MEMORY
//NTSTATUS DispatchRoutine(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp) 
//{
//	KdPrint(("DispatchRoutine begin\n"));
//	
//	//...����Ҫʵ�ֵĹ���
//	//...���һЩ����
//	//...����ʡ��
//
//	NTSTATUS status = STATUS_SUCCESS;
//	//�������irp״̬ �Ƿ�ɹ���
//	pIrp->IoStatus.Status = status;
//	//����irpʵ�ʲ����ֽ���
//	pIrp->IoStatus.Information = 0;	// bytes xfered
//	//ָʾ��ɴ�irp
//	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
//	
//	KdPrint(("DispatchRoutine end\n"));
//	return status;
//}
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
		switch(lpIoStack->Parameters.DeviceIoControl.IoControlCode)
		{
		case IOCTL_DIRECT_IN://д�絽����
			status=WriteDirectIn(pIrp,lpIoStack->Parameters.DeviceIoControl.InputBufferLength,lpIoStack->Parameters.DeviceIoControl.OutputBufferLength);
			break;
		case IOCTL_DIRECT_OUT://�����������ݵ��û���
			status=ReadDirectOut(pIrp,lpIoStack->Parameters.DeviceIoControl.InputBufferLength,lpIoStack->Parameters.DeviceIoControl.OutputBufferLength);
			break;
		case IOCTL_DIRECT_CONTROL:
			status=ControlInstructions(pDevObj,pIrp,lpIoStack->Parameters.DeviceIoControl.InputBufferLength,lpIoStack->Parameters.DeviceIoControl.OutputBufferLength);
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

	//��ȡ�û��㴫��� ���뻺�����ĵ�ַ
	PVOID lp_input_buffer=pIrp->AssociatedIrp.SystemBuffer,
		lp_output_buffer=NULL;

	//������������������
	if(pIrp->MdlAddress)
	{
		//������������� ��ӳ�䵽�ں��Թ� ����ʹ��
		lp_output_buffer=MmGetSystemAddressForMdlSafe(pIrp->MdlAddress,NormalPagePriority );
	}

	KdPrint(("WriteDirectIn inSize=%d outSize=%d %S\n",inSize,outSize,(WCHAR*)lp_input_buffer));
	
	UNICODE_STRING unicode_string;
	RtlInitUnicodeString(&unicode_string,L"д�����ݳɹ�");
	if(lp_output_buffer)
	{
		RtlCopyMemory(lp_output_buffer,unicode_string.Buffer,unicode_string.Length);
	}
	
	pIrp->IoStatus.Information=unicode_string.Length;

	KdPrint(("WriteDirectIn end\n"));

	return status;
}


#pragma PAGE_MEMORY
NTSTATUS ReadDirectOut(IN PIRP pIrp,ULONG inSize,ULONG outSize)
{
	KdPrint(("ReadDirectOut begin\n"));
	NTSTATUS status = STATUS_SUCCESS;

	
	PVOID lp_output_buffer=NULL;

	//������������������
	if(pIrp->MdlAddress)
	{
		//������������� ��ӳ�䵽�ں��Թ� ����ʹ��
		lp_output_buffer=MmGetSystemAddressForMdlSafe(pIrp->MdlAddress,NormalPagePriority );

		KdPrint(("ReadDirectOut inSize=%d outSize=%d\n",inSize,outSize));
	
		UNICODE_STRING unicode_string;
		RtlInitUnicodeString(&unicode_string,L"��ȡ���ݳɹ�");
		RtlCopyMemory(lp_output_buffer,unicode_string.Buffer,unicode_string.Length);
		
		pIrp->IoStatus.Information=unicode_string.Length;

		KdPrint(("ReadDirectOut end\n"));

	}

	return status;
}