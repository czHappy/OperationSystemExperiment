
#include<Windows.h>
#include<iostream>
using namespace std;

HANDLE hMapFile;//�ļ�ӳ���� ʹ��paging file
LPSTR mappingName = "mappingObject";//ӳ���������
LPSTR message;//������Ϣ


int main(){
	hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 256, mappingName);
	if (hMapFile == NULL) {
		printf("�����ļ�ӳ��ʧ�ܣ�");
		return -1;
	}
	//���ļ�ӳ����� ӳ�䵽���̵��ڴ���
	message = (LPSTR)MapViewOfFile(hMapFile,FILE_MAP_WRITE,0,0,256);
	if (message == NULL) {
		printf("���ļ�ӳ���ں˶���ӳ�䵽���ý��̵ĵ�ַ�ռ�ʧ�ܣ�");
		return -1;
	}
	while (true) {
		printf("������Ϣ��");
		printf("%s\n", message);
		getchar();
	
	}

	return 0;
} 
