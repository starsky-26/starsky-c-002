#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>// qsort
#include <time.h>
#include <windows.h>//（windows特有库）sleep函数等（含max、min）
#include <conio.h>//（windows特有库）getch函数等
#include <errno.h>// 检查strtol转换溢出问题→"errno"

#define eps 1e-8
#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))// 计算数组长度
#define BASEPATH "D:\\~~~~~wsy笔记本文件夹\\1 王少阳大学学习资料【待反向覆盖】\\大学时期图文作品\\4编程（C、python、java）\\wsyC语言程序\\基础工具\\实用小工具\\Schedule\\数据库"
#define FILES_NUMBER 13// 0~13，共14个

// 程序基本信息备注
// 时间计算时将time返回值同步到以 2025.1.1 0点 为初始（1735660800）,计算仅限于2026年起

// 自定义函数声明
int day_to_week(int, int, int);
void count_time(long long, int*);
void write_1(int f0, int x);
void write_time(int f0, int x);

// 变量数组
int t[10];// 依次为：年、月、日、上午(0)/中午(1)/下午(2)/晚上(3)、时、分、秒、星期、当天当前秒数（0~86399）
	// 上午：5~11 中午：11~2 下午：2~7  晚上：7~5
char name[1005][30];// 姓名
int date[1005][3];// 年 月 日
FILE *fp_r[20];// 文件只读指针集锦
FILE *fp_w[20];// 文件只写指针集锦

// 基本量数组
int month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};// 月份日数清单
const char *weekday_cn[] = {"", "一", "二", "三", "四", "五", "六", "日"};// 星期名表
const char *period_cn[] = {"上午", "中午", "下午", "晚上"};// 时段名表
const int hour_to_period[24] = {
    3, 3, 3, 3, 3,        // 0-4: 晚上
    0, 0, 0, 0, 0, 0,     // 5-10: 上午
    1, 1, 1,              // 11-13: 中午
    2, 2, 2, 2, 2,        // 14-18: 下午
    3, 3, 3, 3, 3         // 19-23: 晚上
};
const char *filenames[] = {"birthday list.txt", "1zfb.txt", "2ty.txt", "3wz.txt", "4hp.txt", "5genshin.txt",
    "6running.txt", "7piano.txt", "8fruit.txt", "9milk.txt",
    "10arm.txt", "11waterplus.txt", "12wash.txt", "13soybean_milk.txt"};

int main() {
	
	// 文件只读指针统一设定
	for(int i=0; i<=FILES_NUMBER; i++) {
		char fullpath[512];// 用于拼接生成总路径
		sprintf(fullpath, "%s\\%s", BASEPATH, filenames[i]);
		fp_r[i] = fopen(fullpath, "r");
		if (fp_r[i] == NULL) {
    		printf("ERROR：数据库文件 %d 发生缺失", i);
    		exit(1);
		}
	}
	// 读取birthday list文件数据
	int i0=0;
	while(fscanf(fp_r[0], "%29s%d%d%d", name[i0], &date[i0][0], &date[i0][1], &date[i0][2]) == 4) {
		i0++;
		if(i0>=1005) {
			printf("ERROR：birthday list文件数据过多，请检查数据库状态\n");
			exit(1);
		}
	}
	
	while(1) {// 程序总循环
	
	// 重置所有只读指针
	for(int i=0; i<=FILES_NUMBER; i++)
		rewind(fp_r[i]);
	
	Sleep(1000);
	printf("\n你好！~\n\n");
	Sleep(1000);
	
	count_time((long long)time(0), t);
	printf("现在是 %d 年 %d 月 %d 日 星期%s %s %02d:%02d:%02d", t[0], t[1], t[2], weekday_cn[t[7]], period_cn[t[3]], t[4], t[5], t[6]);
	printf("\n\n");
	Sleep(1000);
	
	printf("今日特殊事项：\n");
	int i1=0;
	for(int i=0; i<i0; i++) {
		if(date[i][1]==t[1] && date[i][2]==t[2]) {
			if(date[i][0]!=0) {
				printf("	今天是 %s 的 %d 岁生日！！！\n", name[i], t[0]-date[i][0]);
				i1++;
			}
			else {
				printf("	今天是 %s 生日！！！\n", name[i]);
				i1++;
			}
		}
	}
	if(i1 == 0)
		printf("	无\n");
	printf("\n");
	Sleep(1000);
	
	printf("请按照提示录入当前状态信息：（yes=1，no=0）\n\n");
	Sleep(1000);
	printf("人员状况？（father、mother）（sleeping算0）\n");
	int bb, mm;
	while (scanf("%d%d", &bb, &mm) != 2) {
    	while (getchar() != '\n'); // 清空输入缓冲区
   		printf("输入无效，请重新输入两个数字（例如：1 0）");
	}
	
	printf("正在读取数据，请稍后");
	for(int i=1; i<=6; i++) {
		Sleep(200);
		printf(" .");
	}
	printf("\n");
	Sleep(1000);
	
	// 事件状态数据读取
	int wz=0, hp=0, genshin=0, running=0, piano=0, fruit=0, milk=0, arm=0, waterplus=0, wash=0;
	long long zfb_t=0, ty_t=0, soybean_milk_t=0;
	{
	if(fscanf(fp_r[1], "%lld", &zfb_t) +
	fscanf(fp_r[2], "%lld", &ty_t) +
	fscanf(fp_r[3], "%d", &wz) +
	fscanf(fp_r[4], "%d", &hp) +
	fscanf(fp_r[5], "%d", &genshin) +
	fscanf(fp_r[6], "%d", &running) +
	fscanf(fp_r[7], "%d", &piano) +
	fscanf(fp_r[8], "%d", &fruit) +
	fscanf(fp_r[9], "%d", &milk) +
	fscanf(fp_r[10], "%d", &arm) +
	fscanf(fp_r[11], "%d", &waterplus) +
	fscanf(fp_r[12], "%d", &wash) +
	fscanf(fp_r[13], "%lld", &soybean_milk_t) !=13) {
		printf("ERROR：事件状态数据缺失，请检查数据库状态");
		exit(1);
	}
	}
	
	printf("=========================================================分隔线=========================================================\n");
	printf("以下为当前行动指南：（完成每项后点击enter）\n\n");
	Sleep(1000);
	int count = 1;
	
	// 通用事项：
	printf("%d.请检查设定30min循环计时器\n\n", count++);
	while(1) {
		int temp1 = getch();
		if(temp1 == 13) {
			break;
		}
	}
	
	printf("%d.看一眼电脑桌面的备忘纸条儿→\n\n", count++);
	while(1) {
		int temp1 = getch();
		if(temp1 == 13) {
			break;
		}
	}
	
	printf("%d.请快速查看微信、pyq消息\n\n", count++);
	while(1) {
		int temp1 = getch();
		if(temp1 == 13) {
			break;
		}
	}
	
	// 支付宝、桃源：
	if(((long long)time(0)-zfb_t >= 14400 || zfb_t==0) && ((long long)time(0)-ty_t >= 14400 || ty_t==0)) {
		printf("%d.请查看支付宝、桃源（暂时跳过本项请点击 T ，仅完成支付宝请点击 1 ，仅完成桃源请点击 2 ）\n\n", count++);
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				write_time(1, 1);
				write_time(2, 1);
				break;
			}
			else if(temp1 == '1') {
				write_time(1, 1);
				break;
			}
			else if(temp1 == '2') {
				write_time(2, 1);
				break;
			}
			else if(temp1 == 't' || temp1 == 'T') {
				break;
			}
		}
	}
	else if((long long)time(0)-zfb_t >= 14400 || zfb_t==0) {
		printf("%d.请查看支付宝（暂时跳过本项请点击 T ）\n\n", count++);
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				write_time(1, 1);
				break;
			}
			else if(temp1 == 't' || temp1 == 'T') {
				break;
			}
		}
	}
	else if((long long)time(0)-ty_t >= 14400 || ty_t==0) {
		printf("%d.请查看桃园（暂时跳过本项请点击 T ）\n\n", count++);
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				write_time(2, 1);
				break;
			}
			else if(temp1 == 't' || temp1 == 'T') {
				break;
			}
		}
	}
	
	// 跑步：
	count_time((long long)time(0), t);
	if(t[8]/3600 >=16 && (double)t[8]/3600.0 <= 19.5 && running == 0) {
		printf("%d.现在！立刻！马上！去跑步！！！（暂时跳过本项请点击 T ）\n\n", count++);//下午专用
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				write_1(6, 1);
				break;
			}
			else if(temp1 == 't' || temp1 == 'T') {
				break;
			}
		}
	}
	
	// 豆浆机：
	if((long long)time(0)-soybean_milk_t >= 259200) {
		printf("%d.给豆浆机清灰please~（暂时跳过本项请点击 T ）\n\n", count++);
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				write_time(13, 1);
				break;
			}
			else if(temp1 == 't' || temp1 == 'T') {
				break;
			}
		}
	}
	
	// 王者、和平：
	count_time((long long)time(0), t);
	if(t[8]/3600 >= 20 && wz==0 && hp==0) {
		printf("%d.请登录一下王者、和平，领奖（暂时跳过本项请点击 T ，仅完成王者请点击 1 ，仅完成和平请点击 2 ）\n\n", count++);//晚上专用
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				write_1(3, 1);
				write_1(4, 1);
				break;
			}
			else if(temp1 == '1') {
				write_1(3, 1);
				break;
			}
			else if(temp1 == '2') {
				write_1(4, 1);
				break;
			}
			else if(temp1 == 't' || temp1 == 'T') {
				break;
			}
		}
	}
	else if(t[8]/3600 >= 20 && wz==0) {
		printf("%d.请登录一下王者，领奖（暂时跳过本项请点击 T ）\n\n", count++);
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				write_1(3, 1);
				break;
			}
			else if(temp1 == 't' || temp1 == 'T') {
				break;
			}
		}
	}
	else if(t[8]/3600 >= 20 && hp==0) {
		printf("%d.请登录一下和平，领奖（暂时跳过本项请点击 T ）\n\n", count++);
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				write_1(4, 1);
				break;
			}
			else if(temp1 == 't' || temp1 == 'T') {
				break;
			}
		}
	}
	
	// 如果是晚上（此处为晚9点~第二天凌晨1点前）：水果、奶、arm、加水、洗衣服
	count_time((long long)time(0), t);
	if(t[8]/3600 >= 21 || t[8]/3600 <= 0) {
		if(fruit == 0) {
			printf("%d.记得吃水果（暂时跳过本项请点击 T ）\n\n", count++);
			while(1) {
				int temp1 = getch();
				if(temp1 == 13) {
					write_1(8, 1);
					break;
				}
				else if(temp1 == 't' || temp1 == 'T') {
					break;
				}
			}
		}
		if(milk == 0) {
			printf("%d.记得喝奶（暂时跳过本项请点击 T ）\n\n", count++);
			while(1) {
				int temp1 = getch();
				if(temp1 == 13) {
					write_1(9, 1);
					break;
				}
				else if(temp1 == 't' || temp1 == 'T') {
					break;
				}
			}
		}
		if(arm == 0) {
			printf("%d.哑铃要坚持每天练（暂时跳过本项请点击 T ）\n\n", count++);
			while(1) {
				int temp1 = getch();
				if(temp1 == 13) {
					write_1(10, 1);
					break;
				}
				else if(temp1 == 't' || temp1 == 'T') {
					break;
				}
			}
		}
		if(wash == 0) {
			printf("%d.衣服还没洗呢（暂时跳过本项请点击 T ）\n\n", count++);
			while(1) {
				int temp1 = getch();
				if(temp1 == 13) {
					write_1(12, 1);
					break;
				}
				else if(temp1 == 't' || temp1 == 'T') {
					break;
				}
			}
		}
		if(waterplus == 0) {
			printf("%d.睡前得给两个加湿器加水（暂时跳过本项请点击 T ）\n\n", count++);
			while(1) {
				int temp1 = getch();
				if(temp1 == 13) {
					write_1(11, 1);
					break;
				}
				else if(temp1 == 't' || temp1 == 'T') {
					break;
				}
			}
		}
	}
	
	// 其他：不在
	if(bb == 0) {
		count_time((long long)time(0), t);
		if(piano == 0 && t[8]/3600 <= 22 && t[8]/3600 >= 8) {
			printf("%d.弹弹琴放松一下（当然如果有人睡觉的话就只好t掉啦）（暂时跳过本项请点击 T ）\n\n", count++);
			while(1) {
				int temp1 = getch();
				if(temp1 == 13) {
					write_1(7, 1);
					break;
				}
				else if(temp1 == 't' || temp1 == 'T') {
					break;
				}
			}
		}
//		if(genshin == 0) // 暂时取消对该条已完成的判断
		{
			printf("%d.少爷，该启动原神了~~~（玩玩别的也行）（暂时跳过本项请点击 T ）\n\n", count++);
			while(1) {
				int temp1 = getch();
				if(temp1 == 13) {
					write_1(5, 1);
					break;
				}
				else if(temp1 == 't' || temp1 == 'T') {
					break;
				}
			}
		}
	}
	
	// 其他：在
	if(bb == 1) {
		printf("%d.请按照如下清单顺序展开严肃学习：\n\n", count++);
		Sleep(1000);
		int count1=1;
		printf("	%d.清空待复习单词\n\n", count1++);
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				break;
			}
		}
		printf("	%d.复习一遍数分错题本 & 易错雷区（见手机备忘录）\n\n", count1++);
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				break;
			}
		}
		printf("	%d.学习编程\n\n", count1++);
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				break;
			}
		}
		printf("	%d.读读课外书\n\n", count1++);
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				break;
			}
		}
		printf("	%d.学习大一下数分\n\n", count1++);
		while(1) {
			int temp1 = getch();
			if(temp1 == 13) {
				break;
			}
		}
	}
	
	printf("tips:当前事项完成/中断后，请点击 K 再次运行哦！！！\n\n");
	printf("     依次输入 E S C 可彻底结束程序（并初始化数据，so没到睡觉别瞎点）\n\n");
	
	int out1 = 0;// 程序总循环or退出
	while(1) {
		int temp3 = getch();
		if(temp3 == 'k' || temp3 == 'K') {
			break;
		}
		
		if(temp3 == 'e' || temp3 == 'E') {
			temp3 = getch();
			if(temp3 == 's' || temp3 == 'S') {
				temp3 = getch();
				if(temp3 == 'c' || temp3 == 'C') {
					printf("晚安，记得早点睡觉啦，Good night~（程序系统将于5秒后自动关闭）\n");
					Sleep(5000);
					out1++;
					break;
				}
				else if(temp3 == 'k' || temp3 == 'K') {
					break;	
				}
			}
			else if(temp3 == 'k' || temp3 == 'K') {
				break;	
			}
		}
	}
	if(out1 == 1) {// 初始化数据并退出程序总循环
		for(int i=3; i<=12; i++)
			write_1(i, 0);
		break;
	}
	printf("The new round:\n");
	}// 程序总循环结尾
	
	// 关闭所有只读文件指针
	for(int i=0; i<=FILES_NUMBER; i++) {
		fclose(fp_r[i]);
	}
	
	return 0;
}


// 自定义函数
// 由年月日推出星期几
int day_to_week(int y, int m, int d) {
	int w, c, t=-1;
	if(y<=1582 && m<=10 && d<=4)
		t=2;
	if(m<3)
	{
		y--;
		m+=12;
	}
	c=y/100;
	y%=100;
	w=(c/4-2*c+y+y/4+26*(m+1)/10+d+t)%7;
	if(w<=0) w+=7;
	return w;
}

// 接受当前时间戳，填写t数组内容
void count_time(long long t0, int* t) {
	t0 -= 1735660800;// 与2025年1月1日0点的相对秒数差（t0）
	int day0 = (int)(t0/86400);// 与2025年的相对天数差（取整）（day0）
	int t1 = t0 % 86400;// 当天当前的秒数（t1）
	t[8] = t1;
	int year = 2025;
	
	int temp1 = day0, temp2;
	
	// 计算当前年份，同时temp2为该年剩余日数
	while(1) {
		temp2 = temp1;
		temp1 -= 365+((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));//以当前天数（temp1）减去当前年份（year）的天数
		if(temp1 < 0) break;
		year++;
	}
	t[0] = year;
	
	// 月份基本量修正
	month[2] = 28 + ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
	
	//月、日计算
	for(int i=1; i<=12; i++)
	{
		temp2 -= month[i];
		if(temp2 < 0) {
			t[1] = i;
			temp2 += month[i];
			t[2] = temp2;
			break;
		}
	}
	t[2]++;
	// 时、分、秒计算
	int temp3 = t1/3600;// 当天已过小时数
	
	t[3] = hour_to_period[temp3];
	
	if(temp3 > 12) temp3 -= 12;// 当天已过小时数 转为 12小时制
	t[4] = temp3;
	t[5] = (t1%3600)/60;
	t[6] = t1%60;
	t[7] = day_to_week(t[0], t[1], t[2]);
	return;
}

// 向文件计入状态“0or1”
void write_1(int f0, int x) {// 文件序号，0or1
	char fullpath[512];
	sprintf(fullpath, "%s\\%s", BASEPATH, filenames[f0]);
	fp_w[f0] = fopen(fullpath, "w");
	fprintf(fp_w[f0], "%d", x);
	fclose(fp_w[f0]);
	return;
}

// 向文件计入时间戳（long long）or0
void write_time(int f0, int x) {// 文件序号
	char fullpath[512];
	sprintf(fullpath, "%s\\%s", BASEPATH, filenames[f0]);
	fp_w[f0] = fopen(fullpath, "w");
	if(x==0)
		fprintf(fp_w[f0], "%d", 0);
	else
		fprintf(fp_w[f0], "%lld", (long long)time(0));
	fclose(fp_w[f0]);
	return;
}
