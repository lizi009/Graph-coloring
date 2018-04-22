#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<ctime>
#include<fstream>
#include<string>
using namespace std;


int const m =500;//结点数
int const n=49;//颜色数
int node = 0, color = 0;
int connect[m][m];//连接表
int insol[m];//初始解
int taboo[m][n]; //禁忌表
int tabumove[10000][2];//记录表
int untabumove[10000][2];
int conflict[m][n];//冲突数
int MaxIter =10000000;//迭代上限
int f, best_f;
int iter = 0;

//初始化
void Initialization()
{
	for(int i=0;i<m;i++)//连接表的初始化
		for(int j=0;j<m;j++)
			connect[i][j]=0;
	int x, y;
	ifstream fd;//导入数据生成连接表
	fd.open("f:\\DSJC500.5.col", ios::in);
	if (!fd.is_open())
		cout << "open file failure" << endl;
	while (!fd.eof())
	{
		fd >> x >> y;
		connect[x - 1][y - 1] = 1;
		connect[y - 1][x - 1] = 1;
	}
	fd.close();



    for (int i = 0; i <10000; i++)//记录表的初始化
    {
	    for (int j = 0; j < 2; j++)
	    {
		    tabumove[i][j] = 0;
		    untabumove[i][j] = 0;
	    }
    }
	
	for (int i = 0; i < m; i++)//生成初始解
    {
	     insol[i] = rand() % n;
    }


    for (int i = 0; i < m; i++)//邻接颜色表的初始化
    {
	     for (int j = 0; j < n; j++)
	     {
		      conflict[i][j] = 0;
	     }
    }

for (int i = 0; i < m; i++)
{
	for (int j = 0; j < m; j++)
	{
		if (connect[i][j] == 1)
		{
			int j1 = insol[j];
			conflict[i][j1]++;
		}
	}
}


for (int i = 0; i < m; i++)//禁忌表的初始化
{
	for (int j = 0; j < n; j++)
	{
		taboo[i][j] = 0;
	}
}

for (int i = 0; i < m; i++)//f初始化
{
	for (int j = 0; j < m; j++)
	{
		if (connect[i][j] == 1 && (insol[i] == insol[j]))
			f++;
	}
}
f = f / 2;
best_f = f;

}

//找最佳禁忌或者非禁忌移动，并进行更新
void FindMove()
{
	int delt = 10000;//初始为最大整
	int tmp;//临时变量
	int tabu_delt = 10000;
	int count = 0, tabu_count = 0;
	int A = best_f - f;
	int c_color;//当前结点颜色
	int *h_color;//邻接颜色表行首指针
	int *h_tabu;//禁忌表行首指针
	int c_color_table;//当前结点颜色表的值
	for (int i = 0; i < m; i++)
	{
		c_color = insol[i];
		h_color = conflict[i];
		c_color_table = h_color[c_color];
		if (h_color[c_color] > 0) {
			h_tabu = taboo[i];
			for (int j = 0; j < n; j++) {
				if (c_color != j) {
					tmp = h_color[j] - c_color_table;
					if (h_tabu[j] <= iter) {
						if (tmp <= delt) {
							if (tmp < delt) {
								count = 0;
								delt = tmp;
							}
							count++;
							untabumove[count - 1][0] = i;
							untabumove[count - 1][1] = j;
						}
					}
					else {//禁忌移动
						if (tmp <= tabu_delt) {//6.0
							if (tmp < tabu_delt) {
								tabu_delt = tmp;
								tabu_count = 0;
							}
							tabu_count++;
							tabumove[tabu_count - 1][0] = i;
							tabumove[tabu_count - 1][1] = j;

						}
					}
				}
			}
		}
	}
	tmp = 0;
	if (tabu_delt < A && tabu_delt < delt)
	{
		delt = tabu_delt;
		tmp = rand() % tabu_count;//相等delt随机选择   
		node = tabumove[tmp][0];
		color = tabumove[tmp][1];
	}
	else
	{
		tmp = rand() % count;//相等delt随机选择
		node = untabumove[tmp][0];
		color = untabumove[tmp][1];
	}
	//cout << delt << " ";
	f = delt + f;//更新冲突值
	if (f< best_f)
	{
		best_f = f;
	}
	//cout << best_f << " ";
	int old_color = insol[node];
	insol[node] = color;//更新解
	taboo[node][old_color] = iter + f + rand() % 10 + 1;//更新禁忌表
	for (int i = 0; i < m; i++)
	{
		if (connect[node][i] == 1)
		{
			conflict[i][old_color]--;
			conflict[i][color]++;
		}
	}
}


int main()
{
	srand((unsigned)time(0));//记录时间
	clock_t start, finish;
	start = clock();
	Initialization();//初始化
	while ((iter < MaxIter) && (best_f>0))
	{
		FindMove();
		iter++;
	}
	cout << endl;
	finish = clock();
	cout << time(0) <<"   ";
	cout << finish - start << "(ms)" <<"   ";
	cout << iter <<"   ";
	cout << n<<"   ";
	cout << best_f <<"   ";
	for (int i = 0; i < m; i++)
		cout << insol[i] << " ";
	cout <<endl;
	system("pause");
}



