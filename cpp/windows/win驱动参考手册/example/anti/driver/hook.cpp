#include"hook.h"

//����KeServiceDescriptorTable��
extern "C" ULONG KeServiceDescriptorTable;

/************************************************************SSDT ���*********************************************************/
#pragma PAGE_MEMORY
ULONG GetCurSsdtFunctionAddr(ULONG index)
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

#pragma PAGE_MEMORY
ULONG SetCurSsdtFunctionAddr(ULONG index,ULONG addr)
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



/******************************************************���� EPROCESS�ṹ ���**************************************************/
#pragma PAGE_MEMORY
VOID GetProcessFullName(__out WCHAR* name_buffer,__in const ULONG size_buffer,__in const PEPROCESS pEprocess)
{
	RtlZeroMemory(name_buffer,size_buffer);

	ULONG tmp_buffer=(ULONG)pEprocess;
	USHORT tmp_lenght;
	
	__asm
	{
		pushad;

		mov eax,pEprocess;
		add eax,0x1ec;
		mov eax,[eax];			//UNICODE_STRING
		xor ebx,ebx;
		mov bx,word ptr [eax];	//�ַ�����
		mov tmp_lenght,bx;
		add eax,0x8;			//�ַ���������
		mov tmp_buffer,eax;		

		popad;
	}
	
	RtlCopyMemory(name_buffer,(PVOID)tmp_buffer,tmp_lenght);

}

#pragma PAGE_MEMORY
VOID __stdcall GetProcessName(__out WCHAR* name_buffer,__in const ULONG size_buffer,__in const PEPROCESS pEprocess)
{
	WCHAR* buffer;

	RtlZeroMemory(name_buffer,size_buffer);

	USHORT tmp_lenght;
	
	__asm
	{
		pushad;

		mov eax,pEprocess;
		add eax,0x1ec;
		mov eax,[eax];			//UNICODE_STRING
		xor ebx,ebx;
		mov bx,word ptr [eax];	//�ַ�����
		mov tmp_lenght,bx;
		add eax,0x8;			//�ַ���������
		mov buffer,eax;

		popad;
	}
	
	RtlCopyMemory(name_buffer,buffer,tmp_lenght);

	USHORT index=tmp_lenght;
	for(;index>0;--index)
	{
		if(L'\\'==buffer[index])
		{
			break;
		}
	}

	if(index)
	{
		++index;
	}
	RtlCopyMemory(name_buffer,&buffer[index],tmp_lenght-index);
	
}
#pragma PAGE_MEMORY
extern "C"
__declspec(naked)
 PEPROCESS __stdcall MyPsGetCurrentProcessEx(VOID)
{
	__asm
	{
		mov     eax,dword ptr fs:[0x124];
		mov     eax,dword ptr [eax+0x50]
		retn;
	}
}