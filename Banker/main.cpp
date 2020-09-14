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
vector<int> SecureNum;	//正确的序列

void IsSecure(const vector<vector<MyType> >& need, const vector<vector<MyType> >& allocation,
	const vector<MyType>& available);
void ReWork(const vector<MyType>& oldwork, int index);
void ShowWork(const vector<MyType>& tmp);
void Show(const vector<vector<MyType> >& tmp, int procNum);

int main()
{
	cout << "进程数：";
	cin >> nProc;
	cout << "资源种类：";
	cin >> nRes;

	for (int i = 0; i < nProc; i++)
		Finish.push_back(false);
	for (int j = 0; j < nRes; j++)
		Work.push_back(0);

	vector<MyType> Available(nRes);	//可分配的资源数
	vector<vector<MyType> > Allocation(nProc, vector<MyType>(nRes));	//已分配的资源数
	vector<vector<MyType> > Max(nProc, vector<MyType>(nRes));			//最大需求资源数
	vector<vector<MyType> > Need(nProc, vector<MyType>(nRes));			//还需要资源数
	vector<MyType> OldAva(nRes, 0);	//用于试探失败还原
	vector<MyType> OldAll(nRes, 0);

	//数据初始化
	for (int i = 0; i < nProc; i++)
	{
		cout << "进程P" << i << ":\n";
		for (int j = 0; j < nRes; j++)
		{
			cout << "\t资源" << char(65 + j) << ":\n";
			cout << "\t最大需求量: ";
			cin >> Max[i][j];
			cout << "\t已分配资源数: ";
			cin >> Allocation[i][j];
			Need[i][j] = Max[i][j] - Allocation[i][j];	//需要的资源数
		}
	}

	for (int j = 0; j < nRes; j++)
	{
		cout << "资源" << char(65 + j) << "拥有量: ";
		cin >> Available[j];
		for (int i = 0; i < nProc; i++)
			Available[j] -= Allocation[i][j];
		if (Available[j] < 0)
		{
			cout << "模拟失败!请重试。\n";
			system("pause");
			return 0;
		}
	}

	system("cls");
	//银行家算法
	for (int i = 0; i < nProc; i++)
	{
		for (int j = 0; j < nRes; j++)
		{
			if (Need[i][j] <= Available[j])
			{
				//试探性分配
				OldAva[j] = Available[j];
				OldAll[j] = Allocation[i][j];

				Available[j] -= Need[i][j];
				Allocation[i][j] += Need[i][j];

				Work[j] = OldAva[j];//
				nFlags++;
			}
			else
				break;	//试一试下一个进程
		}
		//还原
		for (int j = 0; j < nFlags; j++)
		{
			Available[j] = OldAva[j];
			Allocation[i][j] = OldAll[j];
		}
		if (nFlags == nRes)
		{//安全性检查
			SecureNum.push_back(i);
			Finish[i] = true;

			cout << "P" << i << ": \n";
			cout << "Work: \n";
			ShowWork(Work);
			//回收该资源
			for (int l = 0; l < nRes; l++)
				Work[l] += Allocation[i][l];
			//显示
			cout << "Need: \n";
			Show(Need, i);
			cout << "Allocation: \n";
			Show(Allocation, i);
			cout << "Work+Allocation: \n";
			ShowWork(Work);

			IsSecure(Need, Allocation, Available);
			//如果全部true就是安全状态
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
		cout << "\n\n序列<";
		for (int i = 0; i < nProc; i++)
			cout << "P" << SecureNum[i] << "  ";
		cout << ">是安全状态，不会导致进程死锁!\n";
	}
	else
		cout << "系统进入不安全状态，故不分配资源。\n";

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
				//还原
				cout << "资源分配失败!\n";
				ReWork(OldWork, j);
				break;
			}
			if (j == nRes - 1)//可以分配
			{
				Finish[i] = true;
				cout << "Work+Allocation: \n";
				ShowWork(Work);

				SecureNum.push_back(i);
				//递归
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