// InlineHook.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include<Windows.h>
#include<string>
#include<iostream>
//����hook���ֽ�
byte code[5]={0};
//hook����
bool inlineHook();
//�Զ��� hook ����
void __stdcall hookSleep(DWORD dwMilliseconds);
int _tmain(int argc, _TCHAR* argv[])
{
	inlineHook();
	Sleep(100);
	system("pause");
	return 0;
}
bool inlineHook()
{
	PVOID address=Sleep;
	
	MEMORY_BASIC_INFORMATION mbi_thunk;  
    //��ѯҳ��Ϣ��  
	VirtualQuery(address, &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION));
	
	//�Ķ�ҳ��������Ϊ��д��
	VirtualProtect(mbi_thunk.BaseAddress,mbi_thunk.RegionSize,PAGE_EXECUTE_READWRITE, &mbi_thunk.Protect);

	//����ԭ����
	memcpy(code,(PVOID)address,5);

	//hook
	__asm
	{
		mov eax,hookSleep;
		sub eax,Sleep;
		sub eax,5;

		mov ebx,Sleep
		mov byte ptr [ebx],0xE9;
		mov [ebx+1],eax;
	}
	VirtualProtect(mbi_thunk.BaseAddress,mbi_thunk.RegionSize, mbi_thunk.Protect, NULL);

	return true;
}

//_declspec(naked) //��������������
void __stdcall hookSleep(DWORD dwMilliseconds)
{
	std::cout<<"hook�ɹ� �������Ϊ"<<dwMilliseconds<<std::endl;
	if(dwMilliseconds<100)
	{
		std::cout<<"ִ��hook����"<<std::endl;
	}
	else
	{
		//��תִ��ԭ����
		__asm
		{
			//��תִ��ԭ����
			mov eax,Sleep;
			add eax,5;
			jmp eax;
		}
	}

}