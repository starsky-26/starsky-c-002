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
数据结构-线性表-双向链表 函数实现
基本信息类型：intdlink
节点类型：node
若同时建立多个顺序表，需建立多个node变量类型 和 多个基本信息变量，并将所有函数的node分别换成对应词
（即：一种基本信息结构体、多种node、多套函数）
*/

static int dlink_error_output = 1;  // 默认打印错误
// 链表节点结构（除link部分外可自定义）
typedef struct Node {
	int num;
	struct Node *link1;
	struct Node *link2;
} node;

// 基本信息结构体
typedef struct {
	node *head;// 链表首节点地址
	node *tail;// 链表尾节点地址
	int size;// 链表节点个数
} intdlink, *intdlink_p;

// 位置信息联合体（sign=0取地址，sign=1取序号）
// 所有涉及intplace的函数，调用要写(intplace){.pr=(x)} or (intplace){.n=(x)}
typedef union {
	node *pr;
	int n;
} intplace;

int main() {

	return 0;
}

// 自定义函数

// 可视化检查当前位置信息是否合法（正常=1，非法=0）
// 特别注意：为降低复杂度，此处不检查地址是否一定在链表中！！！
// 返回1时确保：链表存在且非空，intplace在链表里（地址除外）
int dlink_checkplace(intdlink *list, intplace x, int sign) {
	if(sign!=0 && sign!=1) {
		if (dlink_error_output) printf("The sign to translate \"intplace\" should be 0 or 1.\n");
		return 0;
	}
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist now.\n");
		return 0;
	}
	if(list->size==0) {
		if (dlink_error_output) printf("The target dlinklist is empty.\n");
		return 0;
	}
	
	switch(sign) {
		case 0:{// 地址
			node *pr = x.pr;
			if(pr==NULL) {
				if (dlink_error_output) printf("Invalid position.(the position should not be NULL)\n");
				return 0;
			}
			return 1;
			break;
		}
		case 1:{// 序号
			int n = x.n;
			if(n<=0) {
				if (dlink_error_output) printf("Invalid position.(the node number should >= 1)\n");
				return 0;
			}
			if(n>list->size) {
				if (dlink_error_output) printf("Can't find the target node.(longer than the list's size)\n");
				return 0;
			}
			return 1;
			break;
		}
	}
	return 0;
}

// 创建链表
// 返回值：链表信息结构体指针（失败可视化错误处理并返回NULL）
intdlink* dlink_intlist(int n, int sign) {// sign=1录入数据，sign=0不录入数据
	intdlink *list;
	if(!(list=(intdlink*)malloc(sizeof(intdlink)))) {// 创建链表信息并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new dlinklist.\n");
		return NULL;
	}
	list->head=NULL;
	list->size=0;
	list->tail=NULL;

	node *q;

	if(n==0) {// 如果要创建空链表
		return list;
	}
	
	for(int i=1; i<=n; i++) {// 循环创建n个节点（并全部写入0）
		if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
			if (dlink_error_output) printf("There is not enough space for the new dlinklist.\n");
			dlink_destroylist(list);// 摧毁1~i-1节点
			return NULL;
		}
		switch(sign) {
			case 0:
				dlink_fillnode_0(q);
				break;
			case 1:
				if(dlink_fillnode(q)<=0) {// 数据不足or输入的数据类型错误
					if (dlink_error_output) printf("Failed to input data when creating the dlinklist.\n");
					dlink_destroylist(list);// 摧毁1~i-1节点
					free(q);
					return NULL;
				}
				break;
		}
		if(list->head==NULL) {// 首个节点
			list->head=q;
			q->link1=NULL;
		}
		else {
			q->link1 = list->tail;
			list->tail->link2 = q;// 设置第i-1节点的link2
		}
		list->tail = q;// tail移动到当前（第i）节点
		list->tail->link2=NULL;// 始终保持新结点link2为0
	}
	list->size = n;
	return list;
}

// 获取位置：
// 获取某已知地址节点的序号
// 返回值：节点序号（失败返回0）
int dlink_getplace_pr(intdlink *list, node *p) {
	if(!dlink_checkplace(list, (intplace){.pr=p}, 0)) {
		return 0;
	}
	int count=1;
	for(node *t=list->head; t!=NULL; t=t->link2) {
		if(t==p) {
			return count;
		}
		count++;
	}
	if (dlink_error_output) printf("Invalid position.(the position isn't in the list)\n");
	return 0;
}

// 获取某已知序号节点的地址
// 返回值：节点地址（失败返回NULL）
node* dlink_getplace_n(intdlink *list, int n) {
	if(!dlink_checkplace(list, (intplace){.n=n}, 1)) {
		return NULL;
	}
	
	node *t=list->head;
	n--;
	while(n--) {
		t = t->link2;
	}
	return t;
}

// 向链表头前添加1个空节点
node* dlink_addnode_head(intdlink *list) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist.\n");
		return NULL;
	}
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	dlink_fillnode_0(q);
	q->link1 = NULL;
	list->size++;
	
	if(list->tail==NULL) {//如果是空链表
		list->head = q;
		list->tail = q;
		q->link2 = NULL;
	} else {
		q->link2 = list->head;
		list->head->link1 = q;
		list->head = q;
	}
	return q;
}

// 向链表尾后添加1个空节点
node* dlink_addnode_tail(intdlink *list) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist.\n");
		return NULL;
	}
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	dlink_fillnode_0(q);
	q->link2 = NULL;
	list->size++;
	
	if(list->tail==NULL) {//如果是空链表
		list->head = q;
		list->tail = q;
		q->link1 = NULL;
	} else {
		q->link1 = list->tail;
		list->tail->link2 = q;
		list->tail = q;
	}
	return q;
}

// 向某节点的前or后添加1个空节点
node* dlink_addnode(intdlink *list, int target, intplace x, int sign) {// target：-1=前，1=后
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist.\n");
		return NULL;
	}
	if(target!=-1 && target!=1) {
		if (dlink_error_output) printf("The \"target\" used to determine direction should be -1 or 1.\n");
		return NULL;
	}
	if(!dlink_checkplace(list, x, sign)) {
		return NULL;
	}
	if(sign) {// 统一翻译成x.pr
		x.pr = dlink_getplace_n(list, x.n);
	}
	switch(target) {// 针对头尾提前单独处理
		case -1:
			if(x.pr==list->head) return dlink_addnode_head(list);	
			break;
		case 1:
			if(x.pr==list->tail) return dlink_addnode_tail(list);	
			break;
	}
	
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	dlink_fillnode_0(q);
	list->size++;
	switch(target) {
		case -1:// 节点前增加
			q->link2 = x.pr;
			q->link1 = x.pr->link1;
			x.pr->link1->link2 = q;
			x.pr->link1 = q;						
			break;
		case 1:// 节点后增加
			q->link1 = x.pr;
			q->link2 = x.pr->link2;
			x.pr->link2->link1 = q;
			x.pr->link2 = q;						
			break;
	}
	return q;
}

// 删除intplace=x的节点 or 其前/后节点
// 返回值：返回1（失败返回0）
int dlink_deletenode(intdlink *list, int target, intplace x, int sign) {// target：-1=前，0=x，1=后
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist.\n");
		return 0;
	}
	if(target!=-1 && target!=0 && target!=1) {
		if (dlink_error_output) printf("The \"target\" used to determine direction should be -1 , 0 or 1.\n");
		return 0;
	}
	if(!dlink_checkplace(list, x, sign)) {// 判断位置是否合法
		return 0;
	}
	if(sign) {// 统一翻译成x.pr
		x.pr = dlink_getplace_n(list, x.n);
	}
	switch(target) {
		case 0:// 删除当前节点
			if(x.pr==list->head) {
				node *t = list->head;
				list->head = list->head->link2;
				list->head->link1 = NULL;
				free(t);
			} else if(x.pr==list->tail) {
				node *t = list->tail;
				list->tail = list->tail->link1;
				list->tail->link2 = NULL;
				free(t);
			}
			else {
				x.pr->link1->link2 = x.pr->link2;
				x.pr->link2->link1 = x.pr->link1;
				free(x.pr);
			}
			list->size--;
			break;
		case -1:// 删除前一节点
			if(x.pr==list->head) {
				if (dlink_error_output) printf("Invalid position.(the position isn't in the list)\n");
				return 0;
			}
			if(x.pr->link1==list->head) {// 删除头结点
				list->head = x.pr;
				free(x.pr->link1);
				x.pr->link1 = NULL;
			} else {
				node *t = x.pr->link1;
				x.pr->link1 = t->link1;
				t->link1->link2 = x.pr;
				free(t);
			}
			list->size--;
			break;
		case 1:// 删除后一节点
			if(x.pr==list->tail) {
				if (dlink_error_output) printf("Invalid position.(the position isn't in the list)\n");
				return 0;
			}
			if(x.pr->link2==list->tail) {// 删除尾结点
				list->tail = x.pr;
				free(x.pr->link2);
				x.pr->link2 = NULL;
			} else {
				node *t = x.pr->link2;
				x.pr->link2 = t->link2;
				t->link2->link1 = x.pr;
				free(t);
			}
			list->size--;
			break;
	}
	return 1;
}

// 删除全部节点，删除链表信息
int dlink_deletelist(intdlink *list) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target dlinklist.\n");
		return 0;
	}
	for(node *t=list->head, *p; t!=NULL;) {
		p = t->link2;
		free(t);
		t = p;
	}
	free(list);
	return 1;
}








// 1.需要DIY：
// 查找数据（从head开始，查找第一个特定成员值为x的节点）（找到返回节点指针，找不到返回NULL）
node* dlink_search(intlink *list, int x) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target linklist now.\n");
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
void dlink_fillnode_0(node *q) {
	if(q==NULL) {
		if (dlink_error_output) printf("Can't find the target node.\n");
		return;
	}
	q->num=0;// 具体根据节点实际状态DIY
	return;
}

// 向地址为q的节点录入数据（返回本节点成功录入数据个数；读取到文件尾时返回-1）
int dlink_fillnode(node *q) {
	if(q==NULL) {
		if (dlink_error_output) printf("Can't find the target node.\n");
		return -1;
	}
	return scanf("%d", &q->num);// 具体根据节点实际状态DIY
}

// 打印全部链表数据（空链表等错误返回-1；其余情况返回1）
int dlink_printlist(intlink *list) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target linklist now.\n");
		return -1;
	}
	node *t=list->head;
	int count=0;
	if(t==NULL) return -1;
	printf("Here are contents of the linklist now:\n");
	while(t!=NULL) {
		printf("%d:%d\n", ++count, t->num);// 具体根据节点实际状态填写
		t=t->link;
	}
	return 1;
}


// 2.链表构建函数：

// 创建链表（成功链表信息结构体指针，不成功可视化错误处理并返回NULL）
intlink* dlink_intlist(int n, int sign) {// 节点总数n，是否录入数据（录入=1，不录入=0）（待录入数据数应=n）
	intlink *list;
	if(!(list=(intlink*)malloc(sizeof(intlink)))) {// 创建链表信息并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new linklist.\n");
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
				if (dlink_error_output) printf("There is not enough space for the new linklist.\n");
				link_destroylist(list);// 摧毁1~i-1节点
				return NULL;
			}
			dlink_fillnode_0(q);
			if(list->head==NULL) list->head=q;
			else list->tail->link = q;// 设置第i-1节点的link
			list->tail = q;// tail移动到当前（第i）节点
			list->tail->link=NULL;// 始终保持尾结点link为0
		}
	} else {
		for(int i=1; i<=n; i++) {// 循环创建n个节点（并读取数据）
			if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
				if (dlink_error_output) printf("There is not enough space for the new linklist.\n");
				link_destroylist(list);// 摧毁1~i-1节点
				return NULL;
			}
			if(dlink_fillnode(q)<=0) {// 数据不足or输入的数据类型错误
				if (dlink_error_output) printf("Failed to input data when creating the linklist.\n");
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
node* dlink_addnode_pr_1(intlink *list, node *t) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target linklist.\n");
		return NULL;
	}
	if(t==NULL) {
		if (dlink_error_output) printf("Can't find the target node.\n");
		return NULL;
	}
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	dlink_fillnode_0(q);
	list->size++;
	
	if(t==list->head) {// 在头结点前插入
		q->link = list->head;
		list->head=q;
	} else {
		q->link = t;
		for(node *i=list->head; i!=NULL; i=i->link) {
			if(i->link==t) {// i为t节点前一节点地址
				break;
			}
		}
		i->link=q;
	}
	return q;
}

// 向地址为t的节点后添加1个空节点
node* dlink_addnode_pr_2(intlink *list, node *t) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target linklist.\n");
		return NULL;
	}
	if(t==NULL) {
		if (dlink_error_output) printf("Can't find the target node.\n");
		return NULL;
	}
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	dlink_fillnode_0(q);
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
node* dlink_addnode_head(intlink *list) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target linklist.\n");
		return NULL;
	}
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	dlink_fillnode_0(q);
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
node* dlink_addnode_tail(intlink *list) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target linklist.\n");
		return NULL;
	}
	node *q;
	if(!(q = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功
		if (dlink_error_output) printf("There is not enough space for the new node.\n");
		return NULL;
	}
	q->link=NULL;
	dlink_fillnode_0(q);
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
node* dlink_addnode(intlink *list, int k) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target linklist.\n");
		return NULL;
	}
	if(k==0) {
		return dlink_addnode_head(list);
	}

	node *p=link_getplace(list, k);// 确认第k节点存在（地址为p）
	if(p==NULL) return NULL;
	// 此时确定：链表非空 且 k在链表中

	if(k==list->size) {// 如果k是最后一个节点
		return dlink_addnode_tail(list);
	} else { // 正常创建节点
		node *t;
		if(!(t = (node*)malloc(sizeof(node)))) {// 创建节点并判断是否成功（地址为t）
			if (dlink_error_output) printf("There is not enough space for the new node.\n");
			return NULL;
		}
		dlink_fillnode_0(t);
		list->size++;
		t->link = p->link;
		p->link = t;
		return t;
	}
}

// 删除链表的第k个节点（如果k超限，可视化错误处理并返回-1；其余情况返回1）
int dlink_deletenode(intlink *list, int k) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target linklist.\n");
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
void dlink_destroylist(intlink *list) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target linklist.\n");
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
node* dlink_getplace(intlink *list, int k) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target linklist.\n");
		return NULL;
	}
	node* place=list->head;
	if(k>list->size) {
		if (dlink_error_output) printf("Can't find the target node.(longer than the list's size)\n");
		return NULL;// t超出链表节点总数
	} else if(k<=0) {
		if (dlink_error_output) printf("Invalid position.(the position should >= 1)\n");
		return NULL;
	}
	for(int i=1; i<k; i++) {
		place = place->link;
	}
	return place;
}


// 4.数据读写函数：
// 为链表前k个节点录入数据（k=-1代表能录多少录多少）（返回成功录入的节点个数；不成功可视化错误处理并返回0）
int dlink_scanf(intlink *list, int k) {
	if(list==NULL) {
		if (dlink_error_output) printf("Can't find the target linklist.\n");
		return 0;
	}
	if(k>list->size || list->size==0 || k==0) {
		if (dlink_error_output) printf("There are not enough nodes for scanf.\n");
		return 0;
	}
	if(k<=-2) {
		if (dlink_error_output) printf("Invalid position.(please use -1 or positive place.)\n");
		return 0;
	}
	// 此时：k不超范围 且 list不为空表
	if(k==-1) {
		int count=0;
		node *t=list->head;// t初始化为头节点
		while(1) {
			if(dlink_fillnode(t)<=0) {// 数据量少
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
			if(dlink_fillnode(t)<=0) {// 数据量少
				if (dlink_error_output) printf("There are not enough data for scanf.\n");
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
void dlink_set_error_output(int enable) {// 1→打印；0→不打印
	link_error_output = enable ? 1 : 0;
	return;
}
