#include <stdio.h>

//查询某字符的ASCII编码值、多组输入样例
//输入的数据为x
//手动输入时：以Ctrl+Z结尾

int main()
{
	char x;
	while(scanf(" %c", &x) != EOF)//！！！“回车”也算字符！！！
	{
		printf("%d\n", x);
	}
	
	return 0;
}
