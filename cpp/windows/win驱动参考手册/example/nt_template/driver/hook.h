//hook.h
//��װ��hook��ص�һЩ��������
#pragma once
#include"all.h"
#define HOOKAPI __stdcall

/************************************************************SSDT ���*********************************************************/

//��ȡ ssdt������ǰ��ַ
#pragma PAGE_MEMORY
ULONG GetCurSsdtFunctionAddr(ULONG index);
//��ȡ ssdt������ǰ��ַ
#pragma PAGE_MEMORY
ULONG SetCurSsdtFunctionAddr(ULONG index,ULONG addr);


/******************************************************���� EPROCESS�ṹ ���**************************************************/

//���� ��ȡEPROCESS ȫ��
#pragma PAGE_MEMORY
VOID HOOKAPI GetProcessFullName(__out WCHAR* name_buffer,__in const ULONG size_buffer,__in const PEPROCESS pEprocess);
//ԭ��ͬ�� ֻ������ ������·�� ֻ������ ������
#pragma PAGE_MEMORY
VOID HOOKAPI GetProcessName(__out WCHAR* name_buffer,__in const ULONG size_buffer,__in const PEPROCESS pEprocess);

//ͬ PsGetCurrentProcess ֻ�ǿ��ܻ�hook PsGetCurrentProcess �Ӷ�ʹPsGetCurrentProcess������Ч
#pragma PAGE_MEMORY
extern "C" PEPROCESS HOOKAPI MyPsGetCurrentProcessEx(VOID);