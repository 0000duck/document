//control.h
//������չ ��ʵ�־��幦��

//��ʼ������ ����DriverEntry�б�����
#pragma INIT_MEMORY
VOID InitDriver();
//ж�غ��������ͷ��Զ�����Դ ��ж������UnloadDevice�е���
#pragma PAGE_MEMORY
VOID UnloadDriver();

//�û��㴫��Ŀ�����Ϣ
#pragma PAGE_MEMORY
NTSTATUS ControlInstructions(IN PDEVICE_OBJECT pDevObj,IN PIRP pIrp,ULONG inSize,ULONG outSize);

//���������Ϣʵ��
#pragma PAGE_MEMORY
NTSTATUS Examplate_Control(IN PVOID in_buffer,IN ULONG size_inbuffer,IN PVOID out_buffer,IN ULONG size_outbuffer,OUT ULONG* size_returned);