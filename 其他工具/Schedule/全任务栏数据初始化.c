#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>//qsort
#include <time.h>
#include <windows.h>//sleep函数等（含max、min）
#include <errno.h>//检查strtol转换溢出问题→"errno"

#define eps 1e-8
#define array_len(x) (sizeof(x)/sizeof(x[0]))//计算数组长度
#define basepath "D:\\~~~~~wsy笔记本文件夹\\1 王少阳大学学习资料【待反向覆盖】\\大学时期图文作品\\4编程（C、python、java）\\wsyC语言程序\\基础工具\\实用小工具\\Schedule\\数据库"

// 程序基本信息备注
// 运行本程序将向除birthday list外所有数据库文件写入“0”

// 自定义函数声明
void write_1(int f0, int x);
void write_time(int f0, int x);

// 变量数组
FILE *fp_r[20];// 文件只读指针集锦
FILE *fp_w[20];// 文件只写指针集锦

// 基本量数组
const char *filenames[] = {"birthday list", "1zfb.txt", "2ty.txt", "3wz.txt", "4hp.txt", "5genshin.txt",
    "6running.txt", "7piano.txt", "8fruit.txt", "9milk.txt",
    "10arm.txt", "11waterplus.txt", "12wash.txt", "13soybean_milk.txt"};
int main()
{
	for(int i=1; i<=13; i++)
			write_1(i, 0);
return 0;
}

// 向文件计入状态“0or1”
void write_1(int f0, int x) {// 文件序号，0or1
	char fullpath[256];
	sprintf(fullpath, "%s\\%s", basepath, filenames[f0]);
	fp_w[f0] = fopen(fullpath, "w");
	fprintf(fp_w[f0], "%d", x);
	fclose(fp_w[f0]);
	return;
}

// 向文件计入时间戳（long long）or0
void write_time(int f0, int x) {// 文件序号
	char fullpath[256];
	sprintf(fullpath, "%s\\%s", basepath, filenames[f0]);
	fp_w[f0] = fopen(fullpath, "w");
	if(x==0)
		fprintf(fp_w[f0], "%d", 0);
	else
		fprintf(fp_w[f0], "%lld", (long long)time(0));
	fclose(fp_w[f0]);
	return;
}