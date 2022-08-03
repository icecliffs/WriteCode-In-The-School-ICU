/**
 * @file 项目三：家谱管理
 * @author Po7mn1
 * @brief 
 * @version 0.1
 * @command gcc -g main.c -o main.exe  -fexec-charset=GBK
 * @date 2022-06-25（周六）
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int MAX_ITEM  = 10;
int GD_ITEM = 0;
int GD_PEOPLE = 0;

typedef int ElemData;
typedef char CharData;

typedef struct Birth{              // 生日结构体
     ElemData year;                // 年
     ElemData month;               // 月
     ElemData day;                 // 日
} BirthData;

typedef struct {                   // 成员结构体
     ElemData item;                // 代目
     CharData name[50];            // 姓名
     CharData sex[2];              // 性别
     BirthData BirthtDay;          // 生日
     CharData address[1000];       // 家庭住址
     ElemData marry;               // 婚否
     ElemData dirorlive;           // 死了没
     BirthData DeadtDay;           // 什么时候死的
     CharData how[1000];           // 怎么死的
} Member;

typedef struct TNode{              // 树结构
     Member data;
     struct TNode *left, *right;
} TNode, *Tree;

typedef struct SeQueue{            // 队列定义，存放代数
     Tree Queue[1000];
     int front, rear;
} SeQueue, TQueue;

int Tflag = 0;                // 时间的全局flag
int AddFlag = 0;              // 添加成员的flag
int MoyFlag = 0;              // 修改成员的flag
int DelFlag = 0;              // 删除成员的flag
int FindFlag = 0;             // 查找成员的flag
int FindFindFlag = 0;         // 第x代的flag
int EasyFindFlag = 0;         // 简单查找flag
int DeadFlag = 0;

void MainPanel();                                           // 主面板
Tree ReadFile(FILE *fp);                                    // 读取文件，并生成树
time_t GetTimeNow(time_t *InputTime);                       // 获取现在的时间
void PreOrder(Tree T);                                      // 先序遍历，实则查看树结构内容，显示家谱
void ShowDetailed(Tree T);                                  // 查看详细，实则查看详细
int TotalNode(Tree T);                                      // 统计树节点，实则统计有几个人
int DateDiff(int y_s, int m_s, int d_s, int y_e, int m_e, int d_e);  // 计算日期间隔日
int GenerTotal(Tree T);                                     // 查看代数，遍历左子树
void BirthToday();                                          // 遍历今天谁生日
void TodayBirth();                                          // 看看今天谁生日
void Add(Tree T);                                           // 添加孩子面板
int AddNewChild(Tree T, char name[100]);                    // 添加孩子功能
void Modify(Tree T);                                        // 修改信息
void ModifyChild(Tree T, char name[100]);                   // 修改信息功能
void Delete(Tree T);                                        // 删除成员
void DeleteChild(Tree T, char name[100]);                   // 删除成员功能
void Find(Tree T);                                          // 查找成员
void FindChild(Tree T, char name[100]);                     // 查找成员功能
void ShowGener(Tree T);                                     // 显示第n代所有人的信息
void FindGener(Tree T, int num);                            // 显示第n代所有人的信息:遍历
void FindRelation(Tree T);                                  // 查看俩人的关系
int EasyFind(Tree T, char name[100]);                       // 简单查找，返回是否
int FindRelationGener(Tree T, char name[100], int *ctfflag);// 查看俩人的关系，遍历方式
void Dead(Tree T);                                          // 成员死亡形式
void ThreeChild(Tree T);                                    // 三胎政策
void HowDie(Tree T, char name[100]);                        // 处理死亡
Tree TempOrder(Tree T, char name[100]);                     // 临时遍历

Tree TempOrder(Tree T, char name[100]){
     Tree lr, rr;
     if(!T)
     {
          return NULL;
     }
     if(strcmp(T->data.name, name)==0){
          return T;
     }else{
          lr = TempOrder(T->left, name);
          rr = TempOrder(T->right, name);
          return lr?rr:(lr?rr:NULL);
     }
}

void ThreeChild(Tree T){
     int s;
     Tree Temp;
     char name[100];
     printf("请输入姓名：\n");
     scanf("%s", name);
     Temp = TempOrder(T, name);
     if(Temp->left!=NULL){
          s++;
          Temp = Temp->left;
     }
     while(Temp!=NULL){
          Temp = Temp->right;
          s++;
     }
     if(s>3){
          printf("该成员已满足三胎政策！\n");
     }else{
          printf("多生点...\n");
     }
}

void HowDie(Tree T, char name[100]){
     int command;
     if (T == NULL)
     {
          return 0;
     }else{
          if (strcmp(name, T->data.name) == 0)
          {
               DeadFlag = 1;
               printf("请选择 [%s] 的死亡方式: \n", T->data.name);
               printf("1.自然死亡\n");
               printf("2.溺死\n");
               printf("3.装死\n");
               printf("4.过劳死（996）\n");
               printf("5.烧死\n");
               printf("6.打死\n");
               printf("7.热死\n");
               printf("8.自定义死亡！\n");
               printf("0.返回上一级\n");
               printf("请输入选项(1-7):\n");
               scanf("%d", &command);
               if (command == 0)
               {
                    return;
               } else if(command == 1) {
                    strcpy(T->data.how, "自然死亡");
               } else if(command == 2) {
                    strcpy(T->data.how, "溺死");
               } else if(command == 3) {
                    strcpy(T->data.how, "装死");
               } else if(command == 4) {
                    strcpy(T->data.how, "过劳死（996）");
               } else if(command == 5) {
                    strcpy(T->data.how, "烧死");
               } else if(command == 6) {
                    strcpy(T->data.how, "打死");
               } else if(command == 7) {
                    strcpy(T->data.how, "热死");
               }else{
                    printf("错误！");
                    system("pause");
               }
          }
          HowDie(T->left, name);
          HowDie(T->right, name);
     }
}

int FindRelationGener(Tree T, char name[100], int *ctfflag){
     if (T == NULL)
     {
          return 0;
     }else{
          if (strcmp(name, T->data.name) == 0)
          {
               *ctfflag = T->data.item;
          }
          FindRelationGener(T->left, name, ctfflag);
          FindRelationGener(T->right, name, ctfflag);
     }
}

int EasyFind(Tree T, char name[100]){      // 查找成员功能
     if (T == NULL)
     {
          return 0;
     }else{
          if (strcmp(name, T->data.name) == 0)
          {
               EasyFindFlag = 1;
          }
          EasyFind(T->left, name);          // 递归查找
          EasyFind(T->right, name);
     }
     return EasyFindFlag;
}

void FindRelation(Tree T){
     int aitem, bitem;   // 几代几代
     char a[100],b[100];
     // FindRelationGener(T, a, );
     printf("请输入第一个人的姓名: ");
     scanf("%s", a);
     if (!EasyFind(T, a))
          printf("[!] %s 没在家谱里！\n", a);
     EasyFindFlag = 0;
     FindRelationGener(T, a, &aitem);
     printf("请输入第二个人的姓名: ");
     scanf("%s", b);
     if (!EasyFind(T, b))
          printf("[!] %s 没在家谱里！!\n", b);
     EasyFindFlag = 0;
     FindRelationGener(T, b, &bitem);
     if (bitem > aitem)
     {
          printf("[%s] 比 [%s] 大 [%d] 辈\n", a, b, bitem-aitem);
     }else if(bitem < aitem){
          printf("[%s] 比 [%s] 小 [%d] 辈\n", a, b, aitem-bitem);
     }else if(bitem == aitem){
          printf("[%s] 和 [%s] 同辈\n", a, b);
     }
     return;
}

void FindGener(Tree T, int num){             // 显示第n代所有人的信息:遍历
     if (T == NULL)
     {
          return 0;
     }else{
          if (T->data.item == num)
          {
               FindFindFlag = 1;
               ShowDetailed(T);
               if (T->left)             // 如果有孩子
               {
                    T = T->left;
                    ShowDetailed(T);    // 打印输出
                    while (T->right)    // 遍历右子树
                    {
                         T = T->right;
                         ShowDetailed(T);    // 打印输出
                    }
               }
          }
          FindGener(T->left, num);          // 递归查找
          FindGener(T->right, num);
     }
}

void ShowGener(Tree T){
     int num;
     system("title 查询几代人");
     printf("请输入需要查询第几代人: ");
     scanf("%d", &num);
     printf("查询的第 [%d] 代\n", num);
     FindFindFlag = 0;
     FindGener(T, num);
     if (FindFindFlag == 0)
     {
          printf("没有这一代！\n");
     }else{
          printf("查找成功！\n");
     }
     return;
}

void FindChild(Tree T, char name[100]){      // 查找成员功能
     if (T == NULL)
     {
          return 0;
     }else{
          // 此人为家谱第2代成员,其信息为:
          // B         出生于:1980-6 -12 厦门集美    已婚    男      健在
          // 他孩子的信息为:
          // E         出生于:2021-6 -12 厦门集美    已婚    男      健在
          // F         出生于:2021-6 -12 厦门集美    已婚    男      健在
          if (strcmp(name, T->data.name) == 0)
          {
               FindFlag = 1;
               printf("此人为家谱第 [%d] 代成员,其信息为: \n", T->data.item);
               ShowDetailed(T);
               printf("他孩子的信息为: \n");
               if (T->left)             // 如果有孩子
               {
                    T = T->left;
                    ShowDetailed(T);    // 打印输出
                    while (T->right)    // 遍历右子树
                    {
                         T = T->right;
                         ShowDetailed(T);    // 打印输出
                    }
               }
          }
          FindChild(T->left, name);          // 递归查找
          FindChild(T->right, name);
     }
}

void Find(Tree T){                           // 查找成员
     char name[100];
     system("title 查找成员");
     printf("请输入要查找的人的姓名: ");
     scanf("%s", name);
     FindFlag = 0;
     FindChild(T, name);
     if (FindFlag == 0)
     {
          printf("没有这个人！\n");
     }else{
          printf("查找成功！\n");
     }
     return;
}

void DeleteChild(Tree T, char name[100]){
     // char name[100];
     // printf("请输入要删除的人的姓名: ");
     // scanf("%s", name);
     // DelFlag = 0;
     // DeleteChild(T, name);
     // if (DelFlag == 0)
     // {
     //      printf("没有这个人！\n");
     // }else{
     //      printf("删除成功！\n");
     //      system("cls");
     // }
     return;
}

void Delete(Tree T){                         // 修改信息
     // char name[100];
     // printf("请输入要删除的人的姓名: ");
     // scanf("%s", name);
     // DelFlag = 0;
     // DeleteChild(T, name);
     // if (DelFlag == 0)
     // {
     //      printf("没有这个人！\n");
     // }else{
     //      printf("删除成功！\n");
     //      system("cls");
     // }
     return;
}

void ModifyChild(Tree T, char name[100]){  // 修改信息功能
     int command;
     if (T == NULL)
     {
          return 0;
     }else{
          if (strcmp(name, T->data.name) == 0)
          {
               MoyFlag = 1;
               // 请选择要修改A的哪项数据:
               // 1.修改姓名
               // 2.修改出生年月
               // 3.修改家庭住址
               // 4.修改婚姻状况
               // 5.修改在世情况
               // 6.修改性别
               // 7.返回上一级
               // 请输入选项(1-7):
               printf("请选择要修改 [%s] 的哪项数据: \n", T->data.name);
               printf("1.修改姓名\n");
               printf("2.修改出生年月\n");
               printf("3.修改家庭住址\n");
               printf("4.修改婚姻状况\n");
               printf("5.修改在世情况\n");
               printf("6.修改性别\n");
               printf("7.返回上一级\n");
               printf("请输入选项(1-7):\n");
               scanf("%d", &command);
               switch (command)
               {
                    case 1:
                         printf("请输入修改后的姓名: ");
                         scanf("%s", &T->data.name);
                         break;
                    case 2:
                         printf("请输入修改后的出生年月(格式形如: 2020-1-1): ");
                         scanf("%d-%d-%d", &T->data.BirthtDay.year, &T->data.BirthtDay.month, &T->data.BirthtDay.day);
                         break;
                    case 3:
                         printf("请输入修改后的住址: ");
                         scanf("%s", T->data.address);
                         break;
                    case 4:
                         printf("请输入修改后的婚姻状况0/1 (0表示未婚，1表示已婚): ");
                         scanf("%d", &T->data.marry);
                         break;
                    case 5:
                         printf("请输入修改后的在世情况0/1(0表示去世，1表示在世): ");
                         scanf("%d", &T->data.dirorlive);
                         if (T->data.dirorlive == 0)
                         {
                              printf("请输入过世时间(格式形如: 2020-1-1): ");
                              scanf("%d-%d-%d", &T->data.DeadtDay.year, &T->data.DeadtDay.month, &T->data.DeadtDay.day);
                         }
                         break;
                    case 6:
                         printf("请输入修改后的性别（1表示男，0表示女）: ");
                         scanf("%d", &T->data.sex);
                         break;
                    case 7:
                         return;
                    default:
                         printf("[!] 输入错误！\n");
                         return;
               }
          }
          ModifyChild(T->left, name);
          ModifyChild(T->right, name);
     }
}

void Modify(Tree T){                         // 修改信息
     char name[100];
     system("title 修改成员");
     printf("请输入要修改人的姓名: ");
     scanf("%s", name);
     MoyFlag = 0;
     ModifyChild(T, name);
     if (MoyFlag == 0)
     {
          printf("没有这个人！\n");
     }else{
          printf("修改成功！\n");
          system("cls");
     }
     return;
}

int AddNewChild(Tree T, char name[100]){
     int rank;      // 请输入添加孩子的排行（从1开始计数）
     int dying = 0;
     Tree P = (Tree *)malloc(sizeof(TNode));
     if (T == NULL)
     {
          return;
     }else{
          if (strcmp(name, T->data.name) == 0)         // 找到爹了
          {
               AddFlag = 1;        // 有找到
               printf("请输入添加孩子的排行（从1开始计数）: ");
               scanf("%d", &rank);
               if (T->left) // 如果父辈存在
               {
                    P->data.item = T->data.item + 1;
                    if (rank == 1) // 如果是第一代
                    {
                         P->right = T->left; // 则设置为父节点的左孩子
                         T->left = P;
                    }else{
                         rank--;
                         T = T->left;
                    }
                    rank--;    // 需取前一代的数
                    while (T->right && rank > 0)
                    {
                         T = T->right;
                         rank--;
                    }
                    P->right = T->right;
                    T->right = P;
               }else{    // 如果左孩子不存在先把右孩子给创建了
                    P->data.item = T->data.item + 1;
                    T->left = P;
               }
               printf("请输入添加孩子的姓名: ");
        	     scanf("%s",T->data.name);
        		printf("请输入添加孩子的出生年月(格式形如: 2020-01-01): ");
               scanf("%d-%d-%d", &T->data.BirthtDay.year, &T->data.BirthtDay.month, &T->data.BirthtDay.day);
             	printf("请输入添加孩子的家庭住址: ");
			scanf("%s", T->data.address);
			printf("请输入添加孩子的婚姻状况 0/1 (0表示未婚，1表示已婚): "); 
			scanf("%d", &T->data.marry);
               printf("请输入添加孩子的在世情况 0/1 (0表示去世，1表示在世): ");
               scanf("%d", &T->data.dirorlive);
               if (T->data.dirorlive == 0)
               {
			     printf("请输入添加孩子的去世时间(格式形如: 2010-1-1): ");
			     scanf("%d-%d-%d", &T->data.DeadtDay.year, &T->data.DeadtDay.month, &T->data.DeadtDay.day); 
               }else{
                    T->data.DeadtDay.year = 0;
                    T->data.DeadtDay.month = 0;
                    T->data.DeadtDay.day = 0;
               }
			printf("请输入添加孩子的性别 0/1 (0表示女，1表示男): "); 
			scanf("%d", &T->data.sex); 
		}
          AddNewChild(T->left, name);
          AddNewChild(T->right, name);
     }
}

void Add(Tree T){                                  // 添加孩子面板
     char name[100];
     system("title 添加孩子");
     printf("请输入要添加孩子成员的姓名: ");
     scanf("%s", name);
     AddFlag = 0;
     AddNewChild(T, name);
     if (AddFlag == 0)
     {
          printf("没有这个人！\n");
     }
     return;
}


void TodayBirth(Tree T){
     Tflag = 0;
     int *t, year, month, day;
     time_t ta;
     t = GetTimeNow(&ta);
     year = *t;
     month = t[1];
     day = t[2];
     printf("今天的日期是：[%d-%d-%d]\n", year, month, day);
     BirthToday(T);
     if (Tflag == 0)
     {
          printf("今天没人生日\n");
     }else{
          printf("上面的人过生日了...[o_o]\n");
     }
}

time_t GetTimeNow(time_t *InputTime){        // 获取现在的时间
     time_t t;
     int year; 
     int month;
     int day;
     t = time(0);
     year = localtime(&t)->tm_year + 1900;
     month = localtime(&t)->tm_mon + 1;
     day = localtime(&t)->tm_mday;
     *InputTime = year;
     InputTime[1] = month;
     InputTime[2] = day;
     InputTime[3] = day;
     return InputTime;
}

void BirthToday(Tree T){
     int *t, year, month, day;
     time_t ta;
     t = GetTimeNow(&ta);
     year = *t;
     month = t[1];
     day = t[2];
     int y,m,d;
     if (T == NULL)
     {
          return;
     }else{
          y = T->data.BirthtDay.year;
          m = T->data.BirthtDay.month;
          d = T->data.BirthtDay.day;
          if (y == year && m == month && d == day)
          {
               Tflag = 1;
               printf("%s\n", T->data.name);
          }
          BirthToday(T->left);
          BirthToday(T->right);
     }
}

int GenerTotal(Tree T){
     if (T == NULL)
     {
          return 0;
     }else if(T){
          return GenerTotal(T->left) + 1;    // 递归遍历左子树，firstChild
     }
}

int DateDiff(int y_s, int m_s, int d_s, int y_e, int m_e, int d_e){  // 计算日期间隔日
	int y2, m2, d2;
	int y1, m1, d1;
	m1 = (m_s + 9) % 12;
	y1 = y_s - m1/10;
	d1 = 365*y1 + y1/4 - y1/100 + y1/400 + (m1*306 + 5)/10 + (d_s - 1);
	m2 = (m_e + 9) % 12;
	y2 = y_e - m2/10;
	d2 = 365*y2 + y2/4 - y2/100 + y2/400 + (m2*306 + 5)/10 + (d_e - 1);
	return (d2 - d1);
}

int TotalNode(Tree T){                      // 统计树节点，实则统计有几个人
	if(T==NULL){
          return 0;
     }else{
          return TotalNode(T->left) + TotalNode(T->right) + 1;
     }
}

void ShowDetailed(Tree T){                   // 查看详细，实则查看详细
     int result;
     // 处理时间
     time_t t;
     t = time(0);
     int year; 
     int month;
     int day;
     year = localtime(&t)->tm_year + 1900;
     month = localtime(&t)->tm_mon + 1;
     day = localtime(&t)->tm_mday;
     // 处理时间结束
     printf("%-6s %d-%-2d-%-2d %-15s", T->data.name, T->data.BirthtDay.year, T->data.BirthtDay.month, T->data.BirthtDay.day, T->data.address);
     if (T->data.marry)
     {
          printf("\t已婚");
     }
     if (!T->data.marry)
     {
          printf("\t未婚");
     }
     if (T->data.sex)
     {
          printf("\t男");
     }
     if (!T->data.sex)
     {
          printf("\t女");
     }
     result = (int)T;         // 强转
     if (T->data.dirorlive)
     {
          result = printf("\t活着\n");
     }
     if (!T->data.dirorlive){
          result = printf("\t死了！[%d-%d-%d]死的，距今[%d]天    [%s]\n", T->data.DeadtDay.year,  T->data.DeadtDay.month,  T->data.DeadtDay.day, DateDiff(T->data.DeadtDay.year,T->data.DeadtDay.month,  T->data.DeadtDay.day, year, month, day), T->data.how);
     }
     return result;
}

void PreOrder(Tree T){         // 遍历，显示家谱
     int n = 0;
     if (T)
     {
          if (T->data.item == 1)
          {
               printf("第1代\n");
               ShowDetailed(T);
          } 
          if(T->data.item == 2){
               printf("第2代\n");
               ShowDetailed(T);
          }
          if(T->data.item == 3){
               printf("第3代\n");
               ShowDetailed(T);
          }
          PreOrder(T->right);
          PreOrder(T->left);
     }
     return;
}

void ShowAlgebra(Tree T){        // 查看几代
     signed int result;
     int i;
     for (i = 0;; ++i)
     {
          result = i;
          if (i > GD_ITEM)
          {
               break;
          }
          printf("第 %d 代", i);
     }
}

Tree ReadFile(FILE *fp){      // 读取文件，并生成树
     int flag;
     int i = 0;
     // fscanf(fb, "%d", &flag);
     Tree T;
     fscanf(fp, "%d", &flag);      // 读取辈分
     if (flag == 0)
     {
          T = NULL;      // 
     }else{
          T = (Tree *)malloc(sizeof(TNode));
          T->data.item = flag;     // 辈分
          if (flag > MAX_ITEM)
          {
               MAX_ITEM = 10;
          }
          fscanf(fp, "%s", T->data.name);          
          fscanf(fp, "%d", &T->data.sex);          
          fscanf(fp, "%d-%d-%d", &T->data.BirthtDay.year , &T->data.BirthtDay.month , &T->data.BirthtDay.day);          
          fscanf(fp, "%s", T->data.address);          
          fscanf(fp, "%d", &T->data.marry);          
          fscanf(fp, "%d", &T->data.dirorlive);          
          fscanf(fp, "%d-%d-%d",  &T->data.DeadtDay.year , &T->data.DeadtDay.month , &T->data.DeadtDay.day);
          T->left = ReadFile(fp);
          T->right = ReadFile(fp);
          return T;
     }
}

void MainPanel(){                  // 主面板
     system("cls");
     system("color 2f");
     system("title 欢迎进入家谱管理系统");
     printf("=======================================================================\n");
     printf("\n");
     printf("\n");
     printf("\t\t\t>欢迎进入家谱管理系统<\n");
     printf("            1. 遍历家谱                 2. 添加孩子\n");
     printf("            3. 修改成员                 4. 删除成员\n");
     printf("            5. 查找成员                 6. 显示N代所有人的信息\n");
     printf("            7. 看看今天谁生日           8. 确定俩人的关系\n");
     printf("            9. 成员死亡形式             10. 是否符合计划生育政策\n");
     printf("            11. 退出程序\n");
     printf("\n");
     printf("\n");
     printf("=======================================================================\n");
}
void Dead(Tree T){                                          // 成员死亡形式
     char name[100];
     system("title 修改成员");
     printf("请输入要修改人的姓名: ");
     scanf("%s", name);
     DeadFlag = 0;
     HowDie(T, name);
     if (DeadFlag == 0)
     {
          printf("没有这个人！\n");
     }else{
          printf("修改成功！\n");
          system("cls");
     }
     return;
}

int main(){
     int command;
     FILE *fp;
     if ((fp = fopen("genealogy.txt", "r+")) == NULL)
     {
          system("title Error!");
          printf("[!] 文件打开错误！请检查是否有权限打开或者文件被占用！\n");
     }
     Tree *HomeTree = NULL;
     fp = fopen("genealogy.txt", "r+");
     HomeTree = ReadFile(fp);
     while (1)
     {
          system("cls");
          MainPanel();
          printf("> ");
          scanf("%d", &command);
          switch (command)
          {
               case 1:                       // 遍历家谱
                    system("title 家谱里的人");
                    printf("[*] 共计[%d]代，有[%d]人\n", GenerTotal(HomeTree), TotalNode(HomeTree));
                    printf("%-6s %-10s %-21s %-6s %-8s %-40s %-5s\t\n", "姓名", "出生日", "家庭住址", "婚姻", "性别", "死了没？什么时候死的", "怎么死的");
                    // printf("%-10s%d-%-2d-%-2d %-10s %-3s %-3s %-3s %-10s\n", "姓名", "出生日", "家庭住址", "婚姻信息", "男的女的", "死了没", "什么时候死的");
                    PreOrder(HomeTree);
                    system("pause");
                    break;
               case 2:                       // 添加孩子
                    system("cls");
                    Add(HomeTree);           // 添加孩子函数
                    system("pause");
                    break;
               case 3:                       // 修改成员
                    system("cls");
                    Modify(HomeTree);
                    system("pause");
                    break;
               case 4:                       // 删除成员
                    system("cls");
                    Delete(HomeTree);        // 添加孩子函数
                    system("pause");
                    break;
               case 5:                       // 查找成员
                    system("cls");
                    Find(HomeTree);
                    system("pause");
                    break;
               case 6:                       // 显示N代所有人的信息
                    system("cls");
                    ShowGener(HomeTree);
                    system("pause");
                    break;
               case 7:                       // 看看今天谁生日
                    system("cls");
                    TodayBirth(HomeTree);
                    system("pause");
                    break;
               case 8:                       // 确定俩人的关系
                    system("cls");
                    FindRelation(HomeTree);
                    system("pause");
                    break;
               case 9:
                    Dead(HomeTree);
                    system("pause");           // 成员怎么死的
                    break;
               case 10:
                    ThreeChild(HomeTree);
                    system("pause");
                    break;
               case 11:                       // 退出程序
                    system("title Goodbye, And never comes back :D");
                    printf("Byebye :)\n");
                    return;
                    break;
               default:
                    system("cls");
                    system("title [!] Wrong Data! :D");
                    printf("[!] 您输入的信息有误！请重新输入！\n");
                    getch();
                    break;
          }
     }
}