#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>// qsort
#include <time.h>
#include <windows.h>//（windows特有库）sleep函数等（含max、min）
#include <conio.h>//（windows特有库）getch函数等
#include <errno.h>// 检查strtol转换溢出问题→"errno"

#define eps 1e-8
#define array_len(x) (sizeof(x)/sizeof(x[0]))//计算数组长度
#define basepath "D:\\~~~~~wsy笔记本文件夹\\1 王少阳大学学习资料【待反向覆盖】\\大学时期图文作品\\4编程（C、python、java）\\wsyC语言程序\\基础工具\\实用小工具\\Schedule\\数据库"

// 程序基本信息备注
// 本程序用于对Schedule的生日数据库进行数据输入
// 数据格式：姓名 年 月 日
// 数据处理上限：1000条
// 输入方式：按数据格式输入，一行一个

// 自定义函数声明
int cmp(const void*, const void*);

// 变量数组
char name[1005][30];// 姓名
int date[1005][3];// 年 月 日
int a[1005];// 序号数组

// 基本量数组

int main()
{
	// 初始设定序号数组
	for(int i=0; i<=1004; i++) {
		a[i] = i;
	}
	
	// 读取birthday list中已有数据
	char fullpath[512];// 用于拼接生成总路径
	sprintf(fullpath, "%s\\%s", basepath, "birthday list.txt");
	FILE* fp = fopen(fullpath, "r");
	if (fp == NULL) {
    		printf("ERROR：找不到birthday list文件");
    		exit(1);
		}
	int i=0;
	while(fscanf(fp, "%29s%d%d%d", name[i], &date[i][0], &date[i][1], &date[i][2]) == 4) {
		i++;
	}
	
	// 录入新数据
	while(fscanf(stdin, "%29s%d%d%d", name[i], &date[i][0], &date[i][1], &date[i][2]) != EOF) {
		i++;
	}
	
	// 数据排序
	qsort(a, i, sizeof(int), cmp);
	
	// 新数据存储
	fp = fopen(fullpath, "w");
	int j=0;
	while(1) {
		fprintf(fp, "%s %d %d %d", name[a[j]], date[a[j]][0], date[a[j]][1], date[a[j]][2]);
		j++;
		if(j==i) break;
		fprintf(fp, "\n");
	}
	
return 0;
}

int cmp(const void* x, const void* y) {// 参考顺序：年 月 日 字典序
	int a=*(int*)x, b=*(int*)y;
	for(int i=0; i<=2; i++)
		if (date[a][i] != date[b][i]) return date[a][i] - date[b][i];// 升序
    return strcmp(name[a], name[b]);// 按姓名字典序升序
}