// IATHook.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<Windows.h>
#include<iostream>
//hook ����
void iatHook();
//�滻���Զ��庯��
VOID WINAPI hookSleep(__in DWORD dwMilliseconds);
int _tmain(int argc, _TCHAR* argv[])
{
	iatHook();
	Sleep(100);
	system("pause");
	return 0;
}

void iatHook()
{
	//ȡ��DOSͷ �Ȼ�ַ 
	DWORD image_base=(DWORD)GetModuleHandle(NULL);
	PIMAGE_DOS_HEADER dos_header=(PIMAGE_DOS_HEADER)image_base;

	//ȡ��PEͷ
	PIMAGE_NT_HEADERS pe_header=(PIMAGE_NT_HEADERS)(image_base+dos_header->e_lfanew);

	//ȡ�õ������Ϣ IMAGE_DIRECTORY_ENTRY_IMPORTֵΪ1 ��ʾimport tabale
	PIMAGE_IMPORT_DESCRIPTOR import_descriptor=(PIMAGE_IMPORT_DESCRIPTOR)(image_base+pe_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	
	//�������ڱ��溯����Ϣ�ľֲ�����
	PIMAGE_THUNK_DATA thunk_name,thunk_addr;   
	//�������� ����� 
	for (PIMAGE_IMPORT_DESCRIPTOR iterator=import_descriptor; iterator->FirstThunk; iterator++)   
	{      
		std::cout<<"����ģ��:"<<(char*)(image_base+iterator->Name)<<std::endl;
		   
		//����IAT��Ϣ  PIMAGE_THUNK_DATA��ַ	
		for (thunk_name = (PIMAGE_THUNK_DATA) (image_base + iterator->OriginalFirstThunk),thunk_addr=(PIMAGE_THUNK_DATA) (image_base + iterator->FirstThunk); 
			thunk_addr->u1.Function;++thunk_name,
			++thunk_addr)
		
		{  
			//ȡ�ú������ơ�hint/name��ǰ�����ֽ��Ǻ�������ţ���4���ֽ��Ǻ��������ַ����ĵ�ַ
			DWORD address_data=image_base+thunk_name->u1.AddressOfData;
            char* function_name = (char*)(address_data+2);

			//ȡ�ô�ź�����ַ��ָ��
			DWORD function_addr = thunk_addr->u1.Function;
			
			std::cout<<std::hex<<function_addr<<"   "<<function_name<<std::endl;
			
			if((DWORD)Sleep==function_addr)
			{   
				DWORD pSleep=(DWORD)&thunk_addr->u1.Function; 
				
				MEMORY_BASIC_INFORMATION mbi_thunk;  
				 //��ѯҳ��Ϣ��  
				VirtualQuery((PVOID)pSleep, &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION));
				//�Ķ�ҳ��������Ϊ��дִ��
				VirtualProtect(mbi_thunk.BaseAddress,mbi_thunk.RegionSize,PAGE_EXECUTE_READWRITE, &mbi_thunk.Protect);
				__asm
				{
						mov ebx,pSleep 
						lea eax,hookSleep;
						mov [ebx],eax
				}
				//�ָ�ҳ�汣��
				VirtualProtect(mbi_thunk.BaseAddress,mbi_thunk.RegionSize, mbi_thunk.Protect, NULL);

			}
			
		}   
	}   
}

VOID WINAPI hookSleep(__in DWORD dwMilliseconds)
{
	MessageBox(NULL,L"hook �ɹ�",L"test",MB_OK);
}