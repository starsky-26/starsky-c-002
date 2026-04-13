#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>// qsort
#include <time.h>
//#include <windows.h>//（windows特有库）sleep函数等（含max、min）
//#include <conio.h>//（windows特有库）getch函数等
#include <errno.h>// 检查strtol转换溢出问题→"errno"

#define eps 1e-8
#define array_len(x) (sizeof(x)/sizeof(x[0]))//计算数组长度

/*
数据结构-线性表-单向链表 函数实现
基本信息类型：intlink
节点类型：node
若同时建立多个顺序表，需建立多个node变量类型 和 多个基本信息变量，并将所有函数的node分别换成对应词
（即：一种基本信息结构体、多种node、多套函数）
*/

static int link_error_output = 1;  // 默认打印错误
// 链表节点结构（除link部分外可自定义）
typedef struct Node {
	int num;
	struct Node *link;
} node;

// 基本信息结构体
typedef struct {
	node *head;// 链表首节点地址
	node *tail;// 链表尾节点地址
	int size;// 链表节点个数
} intlink, *intlink_p;

// 位置信息联合体
typedef union {
	node *pr;
	int n;
} intplace;

void link_set_error_output(int enable);
node* link_search(intlink *list, int x);
intlink* link_intlist(int n, int sign);
node* link_getplace(intlink *list, int t);
node* link_addnode_pr_1(intlink *list, node *t);
node* link_addnode_pr_2(intlink *list, node *t);
node* link_addnode_head(intlink *list);
node* link_addnode_tail(intlink *list);
node* link_addnode(intlink *list, int k);
int link_deletenode(intlink *list, int k);
void link_destroylist(intlink *list);
int link_scanf(intlink *list, int k);
void link_fillnode_0(node *q);
int link_fillnode(node *q);
int link_printlist(intlink *list);

int main() {

	return 0;
}

// 自定义函数

// 1.需要DIY：
// 查找数据（从head开始，查找第一个特定成员值为x的节点）（找到返回节点指针，找不到返回NULL）
node* link_search(intlink *list, int x) {
	if(list==NULL) {
		if (link_error_output) printf("Can't find the target linklist now.\n");
		return NULL;
	}
	if(list->size==0) {
		return NULL;
	}
	for(node *t=list->head; t!=NULL; t=t->link) {
		if(t->num==x) {// 根据所查找成员&节点结构DIY
			return t;
		}
	}
	return NULL;
}

// 向地址为q / 序号为n的节点写入0
void link_fillnode_0(node *q) {
	if(q==NULL) {
		if (link_error_output) printf("Can't find the target node.\n");
		return;
	}
	q->num=0;// 具体根据节点实际状态DIY
	return;
}

// 向地址为q的节点录入数据（返回本节点成功录入数据个数；读取到文件尾时返回-1）
int link_fillnode(node *q) {
	if(q==NULL) {
		if (link_error_output) printf("Can't find the target node.\n");
		return -1;
	}
	return scanf("%d", &q->num);// 具体根据节点实际状态DIY
}

// 打印全部链表数据（空链表等错误返回-1；其余情况返回1）
int link_printlist(intlink *list) {
	if(list==NULL) {
		if (link_error_output) printf("Can't find the target linklist now.\n");
		return -1;
	}
	node *t=list->head;
	int count=0;
	printf("Here are contents of the linklist now:\n");
	if(t==NULL) {
		printf("The linklist is empty.\n");
		return -1;
	}
	while(t!=NULL) {
		printf("%d:%d\n", ++count, t->num);// 具体根据节点实际状态填写
		t=t->link;
	}
	return 1;
}


// 2.链表构建函数：

// 创建链表（成功链表信息结构体指针，不成功可视化错误处理并返回NULL）
intlink* link_intlist(int n, int sign) {// 节点总数n，是否录入数据（录入=1，不录入=0）（待录入数据数应=n）
	intlink *list;
	if(!(list=(intlink*)malloc(sizeof(intlink)))) {// 创建链表信息并判断是否成功
		if (link_error_output) printf("There is not enough space for the new linklist.\n");
		return NULL;
	}
	list->head=NULL;
	list->size=0;
	list->tail=NULL;

	node *q;

	if(n==0) {// 如果要创建空链表
		return list;
	}

	if(sign==0) {
		for(int i=1; i<=n; i++) {// 循环创建n个节点（并全部写入0）
			if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
				if (link_error_output) printf("There is not enough space for the new linklist.\n");
				link_destroylist(list);// 摧毁1~i-1节点
				return NULL;
			}
			link_fillnode_0(q);
			if(list->head==NULL) list->head=q;
			else list->tail->link = q;// 设置第i-1节点的link
			list->tail = q;// tail移动到当前（第i）节点
			list->tail->link=NULL;// 始终保持尾结点link为0
		}
	} else {
		for(int i=1; i<=n; i++) {// 循环创建n个节点（并读取数据）
			if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
				if (link_error_output) printf("There is not enough space for the new linklist.\n");
				link_destroylist(list);// 摧毁1~i-1节点
				return NULL;
			}
			if(link_fillnode(q)<=0) {// 数据不足or输入的数据类型错误
				if (link_error_output) printf("Failed to input data when creating the linklist.\n");
				link_destroylist(list);// 摧毁1~i-1节点
				free(q);
				return NULL;
			}
			if(list->head==NULL) list->head=q;
			else list->tail->link = q;// 设置第i-1节点的link
			list->tail = q;// tail移动到当前（第i）节点
			list->tail->link=NULL;// 始终保持尾结点link为0
		}
	}
	list->size = n;
	return list;
}

// 向地址为t的节点前添加1个空节点
node* link_addnode_pr_1(intlink *list, node *t) {
	if(list==NULL) {
		if (link_error_output) printf("Can't find the target linklist.\n");
		return NULL;
	}
	if(t==NULL) {
		if (link_error_output) printf("Can't find the target node.\n");
		return NULL;
	}
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (link_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	link_fillnode_0(q);
	list->size++;
	
	if(t==list->head) {// 在头结点前插入
		q->link = list->head;
		list->head=q;
	} else {
		q->link = t;
		for(node *i=list->head; i!=NULL; i=i->link) {
			if(i->link==t) {// i为t节点前一节点地址
				i->link=q;
				break;
			}
		}
	}
	return q;
}

// 向地址为t的节点后添加1个空节点
node* link_addnode_pr_2(intlink *list, node *t) {
	if(list==NULL) {
		if (link_error_output) printf("Can't find the target linklist.\n");
		return NULL;
	}
	if(t==NULL) {
		if (link_error_output) printf("Can't find the target node.\n");
		return NULL;
	}
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (link_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	link_fillnode_0(q);
	list->size++;
	if(t==list->tail) {// 在尾结点后插入
		q->link = NULL;
		list->tail->link=q;
		list->tail = q;
	} else {
		q->link = t->link;
		t->link = q;
	}
	return q;
}

// 向链表头前添加1个空节点
node* link_addnode_head(intlink *list) {
	if(list==NULL) {
		if (link_error_output) printf("Can't find the target linklist.\n");
		return NULL;
	}
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (link_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	link_fillnode_0(q);
	list->size++;

	if(list->tail==NULL) {// 如果是空链表
		list->head=q;
		list->tail=q;
		q->link=NULL;
	} else {
		q->link=list->head;
		list->head=q;
	}
	return q;
}

// 向链表尾后添加1个空节点
node* link_addnode_tail(intlink *list) {
	if(list==NULL) {
		if (link_error_output) printf("Can't find the target linklist.\n");
		return NULL;
	}
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (link_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	q->link=NULL;
	link_fillnode_0(q);
	list->size++;

	if(list->tail==NULL) {// 如果是空链表
		list->head=q;
		list->tail=q;
	} else {
		list->tail->link=q;
		list->tail=q;
	}
	return q;
}

// 向链表第k个节点后添加1个空节点（若k=0，视为link_addnode_head）（不成功可视化错误处理并返回NULL）
node* link_addnode(intlink *list, int k) {
	if(list==NULL) {
		if (link_error_output) printf("Can't find the target linklist.\n");
		return NULL;
	}
	if(k==0) {
		return link_addnode_head(list);
	}

	node *p=link_getplace(list, k);// 确认第k节点存在（地址为p）
	if(p==NULL) return NULL;
	// 此时确定：链表非空 且 k在链表中

	if(k==list->size) {// 如果k是最后一个节点
		return link_addnode_tail(list);
	} else { // 正常创建节点
		node *t;
		if(!(t = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功（地址为t）
			if (link_error_output) printf("There is not enough space for the new node.\n");
			return NULL;
		}
		link_fillnode_0(t);
		list->size++;
		t->link = p->link;
		p->link = t;
		return t;
	}
}

// 删除链表的第k个节点（如果k超限，可视化错误处理并返回-1；其余情况返回1）
int link_deletenode(intlink *list, int k) {
	if(list==NULL) {
		if (link_error_output) printf("Can't find the target linklist.\n");
		return -1;
	}
	node *t=link_getplace(list, k);// 确认第k节点存在（地址为t）
	if(t==NULL) {// getplace出错
		return -1;
	}
	// 此时确定：k在链表中

	if(list->size==1) {// 链表只有一个节点
		list->size=0;
		free(list->head);
		list->head=NULL;
		list->tail=NULL;
		return 1;
	} else if(k==1) { // 节点不止1个，但要删头节点
		list->head=list->head->link;
		list->size--;
		free(t);
		return 1;
	} else if(k==list->size) { // 节点不止1个，但要删尾节点
		list->tail=link_getplace(list, k-1);
		list->size--;
		list->tail->link=NULL;
		free(t);
		return 1;
	} else { // 要删除中间节点
		node *t1=link_getplace(list, k-1);// 待删除节点的上一节点地址
		list->size--;
		t1->link=t->link;
		free(t);
		return 1;
	}
}

// 删除链表所有节点（包括链表信息）
void link_destroylist(intlink *list) {
	if(list==NULL) {
		if (link_error_output) printf("Can't find the target linklist.\n");
		return;
	} else if(list->head==NULL) { // 如果是空链表（此处不用list->size意在适配intlist中删除局部节点
		free(list);
		return;
	}
	node *t=list->head, *next;
	while(t!=NULL) {
		next=t->link;
		free(t);
		t=next;
	}
	free(list);
	return;
}


// 3.位置获取函数：

// 获取链表list中第k个节点的地址（不成功可视化错误处理并返回NULL）
// 通过link_getplace()>0 可确定：链表非空 且 k在链表中
node* link_getplace(intlink *list, int k) {
	if(list==NULL) {
		if (link_error_output) printf("Can't find the target linklist.\n");
		return NULL;
	}
	node* place=list->head;
	if(k>list->size) {
		if (link_error_output) printf("Can't find the target node.(longer than the list's size)\n");
		return NULL;// t超出链表节点总数
	} else if(k<=0) {
		if (link_error_output) printf("Invalid position.(the position should >= 1)\n");
		return NULL;
	}
	for(int i=1; i<k; i++) {
		place = place->link;
	}
	return place;
}


// 4.数据读写函数：
// 为链表前k个节点录入数据（k=-1代表能录多少录多少）（返回成功录入的节点个数；不成功可视化错误处理并返回0）
int link_scanf(intlink *list, int k) {
	if(list==NULL) {
		if (link_error_output) printf("Can't find the target linklist.\n");
		return 0;
	}
	if(k>list->size || list->size==0 || k==0) {
		if (link_error_output) printf("There are not enough nodes for scanf.\n");
		return 0;
	}
	if(k<=-2) {
		if (link_error_output) printf("Invalid position.(please use -1 or positive place.)\n");
		return 0;
	}
	// 此时：k不超范围 且 list不为空表
	if(k==-1) {
		int count=0;
		node *t=list->head;// t初始化为头节点
		while(1) {
			if(link_fillnode(t)<=0) {// 数据量少
				return count;
			} else {
				count++;
				if(count==list->size) {// 节点数少
					return list->size;
				}
			}
			t=t->link;
		}
	} else {
		node *t=list->head;// t初始化为头节点
		int count=0;
		while(k--) {
			if(link_fillnode(t)<=0) {// 数据量少
				if (link_error_output) printf("There are not enough data for scanf.\n");
				return count;
			} else {
				count++;
			}
			t=t->link;
		}
		return count;
	}
}


// 5.其他函数：

// 控制是否可视化打印错误信息
void link_set_error_output(int enable) {// 1→打印；0→不打印
	link_error_output = enable ? 1 : 0;
	return;
}
