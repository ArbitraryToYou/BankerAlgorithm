#include <iostream>
#include <vector>
#include <Windows.h>
typedef int MyType;
using std::cout;
using std::cin;
using std::endl;
using std::vector;

int nProc = 0;
int nRes = 0;

int nFlags = 0;
bool isEnd = false;
vector<MyType> Work;
vector<bool> Finish;
vector<int> SecureNum;	//��ȷ������

void IsSecure(const vector<vector<MyType> >& need, const vector<vector<MyType> >& allocation,
	const vector<MyType>& available);
void ReWork(const vector<MyType>& oldwork, int index);
void ShowWork(const vector<MyType>& tmp);
void Show(const vector<vector<MyType> >& tmp, int procNum);

int main()
{
	cout << "��������";
	cin >> nProc;
	cout << "��Դ���ࣺ";
	cin >> nRes;

	for (int i = 0; i < nProc; i++)
		Finish.push_back(false);
	for (int j = 0; j < nRes; j++)
		Work.push_back(0);

	vector<MyType> Available(nRes);	//�ɷ������Դ��
	vector<vector<MyType> > Allocation(nProc, vector<MyType>(nRes));	//�ѷ������Դ��
	vector<vector<MyType> > Max(nProc, vector<MyType>(nRes));			//���������Դ��
	vector<vector<MyType> > Need(nProc, vector<MyType>(nRes));			//����Ҫ��Դ��
	vector<MyType> OldAva(nRes, 0);	//������̽ʧ�ܻ�ԭ
	vector<MyType> OldAll(nRes, 0);

	//���ݳ�ʼ��
	for (int i = 0; i < nProc; i++)
	{
		cout << "����P" << i << ":\n";
		for (int j = 0; j < nRes; j++)
		{
			cout << "\t��Դ" << char(65 + j) << ":\n";
			cout << "\t���������: ";
			cin >> Max[i][j];
			cout << "\t�ѷ�����Դ��: ";
			cin >> Allocation[i][j];
			Need[i][j] = Max[i][j] - Allocation[i][j];	//��Ҫ����Դ��
		}
	}

	for (int j = 0; j < nRes; j++)
	{
		cout << "��Դ" << char(65 + j) << "ӵ����: ";
		cin >> Available[j];
		for (int i = 0; i < nProc; i++)
			Available[j] -= Allocation[i][j];
		if (Available[j] < 0)
		{
			cout << "ģ��ʧ��!�����ԡ�\n";
			system("pause");
			return 0;
		}
	}

	system("cls");
	//���м��㷨
	for (int i = 0; i < nProc; i++)
	{
		for (int j = 0; j < nRes; j++)
		{
			if (Need[i][j] <= Available[j])
			{
				//��̽�Է���
				OldAva[j] = Available[j];
				OldAll[j] = Allocation[i][j];

				Available[j] -= Need[i][j];
				Allocation[i][j] += Need[i][j];

				Work[j] = OldAva[j];//
				nFlags++;
			}
			else
				break;	//��һ����һ������
		}
		//��ԭ
		for (int j = 0; j < nFlags; j++)
		{
			Available[j] = OldAva[j];
			Allocation[i][j] = OldAll[j];
		}
		if (nFlags == nRes)
		{//��ȫ�Լ��
			SecureNum.push_back(i);
			Finish[i] = true;

			cout << "P" << i << ": \n";
			cout << "Work: \n";
			ShowWork(Work);
			//���ո���Դ
			for (int l = 0; l < nRes; l++)
				Work[l] += Allocation[i][l];
			//��ʾ
			cout << "Need: \n";
			Show(Need, i);
			cout << "Allocation: \n";
			Show(Allocation, i);
			cout << "Work+Allocation: \n";
			ShowWork(Work);

			IsSecure(Need, Allocation, Available);
			//���ȫ��true���ǰ�ȫ״̬
			for (auto x = Finish.begin(); x != Finish.end(); x++)
				if (*x == false)
				{
					isEnd = false;
					break;
				}

			if (isEnd)
				break;
		}
		nFlags = 0;
	}
	if (SecureNum.size() == nProc)
	{
		cout << "\n\n����<";
		for (int i = 0; i < nProc; i++)
			cout << "P" << SecureNum[i] << "  ";
		cout << ">�ǰ�ȫ״̬�����ᵼ�½�������!\n";
	}
	else
		cout << "ϵͳ���벻��ȫ״̬���ʲ�������Դ��\n";

	return 0;
}

void IsSecure(const vector<vector<MyType> >& need, const vector<vector<MyType> >& allocation,
	const vector<MyType>& available)
{
	vector<MyType> OldWork(nRes, 0);
	for (int j = 0; j < nRes; j++)
		OldWork[j] = Work[j];

	for (int i = 0; i < nProc; i++)
	{
		if (Finish[i])
			continue;
		cout << "P" << i << ": \n";
		cout << "Work: \n";
		ShowWork(Work);
		cout << "Need: \n";
		Show(need, i);
		cout << "Allocation: \n";
		Show(allocation, i);
		for (int j = 0; j < nRes; j++)
		{
			if (need[i][j] <= Work[j])
				Work[j] += allocation[i][j];
			else
			{
				//��ԭ
				cout << "��Դ����ʧ��!\n";
				ReWork(OldWork, j);
				break;
			}
			if (j == nRes - 1)//���Է���
			{
				Finish[i] = true;
				cout << "Work+Allocation: \n";
				ShowWork(Work);

				SecureNum.push_back(i);
				//�ݹ�
				IsSecure(need, allocation, available);
				return;
			}
		}
		if (isEnd)
			break;
	}
	isEnd = true;
}

void ReWork(const vector<MyType>& oldwork, int index)
{
	for (int j = 0; j <= index; j++)
		Work[j] = oldwork[j];
}

void ShowWork(const vector<MyType>& tmp)
{
	for (int j = 0; j < nRes; j++)
		cout << char(65 + j) << "=" << tmp[j] << "   ";
	cout << endl;
}
void Show(const vector<vector<MyType> >& tmp, int procNum)
{
	for (int j = 0; j < nRes; j++)
		cout << char(65 + j) << "=" << tmp[procNum][j] << "   ";
	cout << endl;
}