//all.h
//һЩȫ������ ��Ҫ��һЩȫ�ֺ�Ķ���
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

/**************************************************����������Ϣ����*************************************************************************/
//������
#define DRIVER_NAME L"\\Device\\HelloDDK"
//����������
#define DRIVER_LINK_NAME L"\\??\\HelloDDK"


/*************************************************���Ժ��û���ͨ��*************************************************************************/
//ֱ�Ӷ�д
#define IOCTL_DIRECT_IN  CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_IN_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)
#define IOCTL_DIRECT_OUT CTL_CODE(FILE_DEVICE_UNKNOWN,0x801,METHOD_OUT_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)

//����ָ�� ��һ��ULONG Ϊ������ �������buffer
#define IOCTL_DIRECT_CONTROL  CTL_CODE(FILE_DEVICE_UNKNOWN,0x810,METHOD_IN_DIRECT,FILE_WRITE_DATA | FILE_READ_DATA)

/**************************************************ȫ�����ú�******************************************************************************/
#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#ifndef MAX_BUFFER
#define MAX_BUFFER 256
#endif

/*************************************************�Զ����һЩ�㷨 ���ݽṹ*****************************************************************/
#include"utility.hpp"
