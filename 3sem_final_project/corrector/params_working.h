#ifndef _PARAMS_WORKING_
#define _PARAMS_WORKING_

typedef void* Pointer;


// ���������� ������������ ������ ��������� �� �������� ���������� ��� �������
// ������������ ������������ ������ ���������� �� ������������
Pointer* set_cfg(int argc, char** argv);

// ������ ������ �������, �.�. ������ ��� ���� ���������� �����������
void delete_cfg(int mode, Pointer* cfg);

#endif