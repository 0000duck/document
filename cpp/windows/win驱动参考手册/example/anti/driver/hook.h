#pragma once
#include"all.h"

/************************************************************SSDT ���*********************************************************/

//��ȡ ssdt������ǰ��ַ
ULONG GetCurSsdtFunctionAddr(ULONG index);
//��ȡ ssdt������ǰ��ַ
ULONG SetCurSsdtFunctionAddr(ULONG index,ULONG addr);


/******************************************************���� EPROCESS�ṹ ���**************************************************/

//���� ��ȡEPROCESS ȫ��
VOID GetProcessFullName(__out WCHAR* name_buffer,__in const ULONG size_buffer,__in const PEPROCESS pEprocess);
//ԭ��ͬ�� ֻ������ ������·�� ֻ������ ������
VOID __stdcall GetProcessName(__out WCHAR* name_buffer,__in const ULONG size_buffer,__in const PEPROCESS pEprocess);

//ͬ PsGetCurrentProcess ֻ�ǿ��ܻ�hook PsGetCurrentProcess �Ӷ�ʹPsGetCurrentProcess������Ч
extern "C" PEPROCESS __stdcall MyPsGetCurrentProcessEx(VOID);