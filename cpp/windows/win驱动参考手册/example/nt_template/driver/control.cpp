#include"all.h"
#include"control.h"


#pragma INIT_MEMORY
VOID InitDriver()
{
}

#pragma PAGE_MEMORY
VOID UnloadDriver()
{

}

#pragma PAGE_MEMORY
NTSTATUS ControlInstructions(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp,ULONG inSize,ULONG outSize)
{
	NTSTATUS status = STATUS_SUCCESS;

	//��ȡ�û��㴫��� ���뻺�����ĵ�ַ
	PVOID lp_input_buffer=pIrp->AssociatedIrp.SystemBuffer,
		lp_output_buffer=NULL;

	pIrp->IoStatus.Information=0;
	ULONG* size_returned=&pIrp->IoStatus.Information;
	
	if(lp_input_buffer && inSize>3)
	{
		//������������������
		if(pIrp->MdlAddress)
		{
			//������������� ��ӳ�䵽�ں��Թ� ����ʹ��
			lp_output_buffer=MmGetSystemAddressForMdlSafe(pIrp->MdlAddress,NormalPagePriority );
		}

		ULONG control=0;//��ȡ������
		
		__asm
		{
			push eax;
			push ebx;

			mov eax,lp_input_buffer;
			mov ebx,[eax];
			mov control,ebx;

			add eax,4;
			mov lp_input_buffer,eax;

			pop ebx;
			pop eax;
		}
		
		inSize-=4;
		switch(control)
		{	
		case 10000:
			Examplate_Control(lp_input_buffer,inSize,lp_output_buffer,outSize,size_returned);
			break;

		}
	}
	else
	{
		status=STATUS_BUFFER_ALL_ZEROS;
	}

	return status;
}

#pragma PAGE_MEMORY
NTSTATUS Examplate_Control(IN PVOID in_buffer,IN ULONG size_inbuffer,IN PVOID out_buffer,IN ULONG size_outbuffer,OUT ULONG* size_returned)
{
	NTSTATUS status = STATUS_SUCCESS;
	//����һЩ���� ����ж����������
	WCHAR buffer[MAX_BUFFER]={0};
	RtlCopyMemory(buffer,in_buffer,size_inbuffer);
	KdPrint(("size=%d inParam:%S \n",size_inbuffer,buffer));
	
	//�����Ҫ��������
	if(out_buffer)
	{
		UNICODE_STRING unicode_string;
		RtlInitUnicodeString(&unicode_string,L"д�����ݳɹ�");
		RtlCopyMemory(out_buffer,unicode_string.Buffer,unicode_string.Length);
		*size_returned=unicode_string.Length;

		KdPrint(("size=%d outParam:%wZ",*size_returned,&unicode_string));	
	}

	
	/*LinkList<ULONG> link;
	link.init();

	for(ULONG i=0;i<10;++i)
	{
		link.push_back(i);
	}

	LinkList<ULONG>::POS pos=link.begin();
	while(pos)
	{
		DbgPrint("%d\n",pos->data);
		link.next(pos);
	}*/

	return status;
}