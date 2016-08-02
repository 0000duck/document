//driver.cpp

#include "Driver.h"
#include"anti.h"
#include"hook.h"
#include"myhook.h"

//��¼����������
WCHAR protected_name[MAX_PATH];


#pragma INIT_MEMORY
extern "C" NTSTATUS DriverEntry (IN PDRIVER_OBJECT pDriverObject,IN PUNICODE_STRING pRegistryPath	) 
{
	KdPrint(("DriverEntry begin\n"));
	initData();
	NTSTATUS status;
	
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
VOID initData()
{
	NTSTATUS status;

	OBJECT_ATTRIBUTES object_attributes;
	IO_STATUS_BLOCK io_status;

	HANDLE handle_file;
	UNICODE_STRING file_symbol;
	//��ʼ���ļ������� �������豸�� \device\harddiskvolume1\test.txt
	RtlInitUnicodeString(&file_symbol,L"\\??\\c:\\anti.txt");

	//��ʼ���豸����
	InitializeObjectAttributes(&object_attributes,&file_symbol,OBJ_CASE_INSENSITIVE,NULL,NULL);

	//�����ļ�
	status=ZwCreateFile(&handle_file,
		GENERIC_READ,//д
		&object_attributes,
		&io_status,
		NULL,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,
		FILE_OPEN,	//�����ļ� FILE_OPEN(���ļ�)
		FILE_SYNCHRONOUS_IO_NONALERT,NULL,0);

	if(STATUS_SUCCESS==status)
	{
		KdPrint(("���ļ��ɹ�\n"));
	}
	else
	{
		KdPrint(("���ļ�ʧ��\n"));
	}

	if(STATUS_SUCCESS==status)
	{
		//�ļ�����
		
		FILE_STANDARD_INFORMATION file_standard_information;
		
		if(STATUS_SUCCESS==ZwQueryInformationFile(handle_file,&io_status,&file_standard_information,sizeof(FILE_STANDARD_INFORMATION),FileStandardInformation))
		{
			LONGLONG file_long=file_standard_information.EndOfFile.QuadPart;
			
			if(file_long>MAX_PATH)
			{
				file_long=MAX_PATH;

			}
			KdPrint(("�ļ�����Ϊ%d�ֽ�\n",file_long));

			//���ö���λ��
			LARGE_INTEGER byte_offset;
			byte_offset.QuadPart=2;

			//���ļ�
			RtlZeroMemory(protected_name,sizeof(protected_name));
			if(STATUS_SUCCESS==ZwReadFile(handle_file,NULL,NULL,NULL,&io_status,protected_name,file_long,&byte_offset,0))
			{
				KdPrint(("������:%S\n",protected_name));
				//RtlInitUnicodeString(&protected_name,file_buffer);
				//KdPrint(("������:%wZ\n",&protected_name));
			}

		}

		//�ر��ļ�
		if(STATUS_SUCCESS==ZwClose(handle_file))
		{
			KdPrint(("�ر��ļ��ɹ�\n"));
		}
	}

	//Hook_NtQueryInformationProcess();

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
	pDevExt->ustrDeviceName = devName;
	
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

	//Unhook_NtQueryInformationProcess();
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
		switch(lpIoStack->Parameters.DeviceIoControl.IoControlCode)
		{
		case IOCTL_DIRECT_IN://д�絽����

			KdPrint(("case IOCTL_DIRECT_IN\n"));
			status=WriteDirectIn(pIrp,lpIoStack->Parameters.DeviceIoControl.InputBufferLength,lpIoStack->Parameters.DeviceIoControl.OutputBufferLength);
			break;
		case IOCTL_DIRECT_OUT://�����������ݵ��û���
			status=ReadDirectOut(pIrp,lpIoStack->Parameters.DeviceIoControl.InputBufferLength,lpIoStack->Parameters.DeviceIoControl.OutputBufferLength);
			break;
	
		case IOCTL_DIRECT_GET_DEBUG_PORT:
			status=ReadDebugPort(pIrp,lpIoStack->Parameters.DeviceIoControl.InputBufferLength,lpIoStack->Parameters.DeviceIoControl.OutputBufferLength);
			break;
		case IOCTL_DIRECT_GET_DEBUG_PORT_SET:
			status=ReadDebugPortSet(pIrp,lpIoStack->Parameters.DeviceIoControl.InputBufferLength,lpIoStack->Parameters.DeviceIoControl.OutputBufferLength);
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

	KdPrint(("WriteDirectIn inSize=%d outSize=%d %S\n",inSize,outSize,lp_input_buffer));
	
	UNICODE_STRING unicode_string;
	RtlInitUnicodeString(&unicode_string,L"д�����ݳɹ�");
	if(lp_output_buffer)
	{
		RtlCopyMemory(lp_output_buffer,unicode_string.Buffer,unicode_string.Length);
	}
	
	pIrp->IoStatus.Information=unicode_string.Length;

	KdPrint(("WriteDirectIn end%d\n"));

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

		KdPrint(("ReadDirectOut end%d\n"));

	}

	return status;
}