#pragma once

#ifdef __cplusplus	//����c++���� ��c���ŷ�ʽ����
extern "C"
{
#endif

#include <NTDDK.h>

#ifdef __cplusplus
}
#endif 

#define PAGE_MEMORY code_seg("PAGE")		//��ҳ�ڴ�
#define LOCKED_MEMORY code_seg()			//�Ƿ�ҳ
#define INIT_MEMORY code_seg("INIT")		//INIT�ڴ� ���ڴ�ָ��ֻ�ڼ�������ʱ���� �������سɹ����Զ�ж��

#define PAGE_DATA_MEMORY data_seg("PAGE")		
#define DATA_LOCKED_MEMORY data_seg()			
#define DATA_INIT_MEMORY data_seg("INIT")		

#define DRIVER_NAME L"\\Device\\Anti"
#define DRIVER_LINK_NAME L"\\??\\Anti"

#ifndef MAX_PATH
#define MAX_PATH 256
#endif



