#pragma once
#include"all.h"

//ԭ����ָ������
extern "C"
typedef
NTSTATUS (__stdcall *OldNtQueryInformationProcess)(
  __in       HANDLE ProcessHandle,
  __in       PROCESSINFOCLASS ProcessInformationClass,
  __out      PVOID ProcessInformation,
  __in       ULONG ProcessInformationLength,
  __out_opt  PULONG ReturnLength
);


//�Զ���hook����

extern "C"
NTSTATUS __stdcall MyNtQueryInformationProcess(
  __in       HANDLE ProcessHandle,
  __in       PROCESSINFOCLASS ProcessInformationClass,
  __out      PVOID ProcessInformation,
  __in       ULONG ProcessInformationLength,
  __out_opt  PULONG ReturnLength
);

VOID Hook_NtQueryInformationProcess();
VOID Unhook_NtQueryInformationProcess();
