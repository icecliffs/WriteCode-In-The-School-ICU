/**
 * @file 项目一：学生信息管理
 * @author Po7mn1
 * @brief 
 *   项目简介：
     学生信息管理是学校教务部门日常工作的重要组成部分。本项目是对学生信息管理的简单模拟，包含两个模块：基本信息查询和成绩管理。
     数据要求：
     每个学生的信息包括：学号、姓名、性别、出生年月、电话、住址。成绩设置为五门课程成绩，后续可扩展。数据可直接输入输出，也可以采用文件存储。
     功能要求：
     至少包含下列基本功能，可增加功能计入加分项，缺少功能减分
     1：学生基本信息的增加。
     2：通过输入学号或者姓名实现基本信息的查询、修改、删除。
     3：通过输入学号或者姓名实现成绩的输入、查询、修改。
     4：对成绩进行降序排列，要求可以按照总分或者某XXXX一门指定课程排序。
     界面要求：
     根据系统功能要求设计。应有合理的提示，每个功能可以设立菜单，根据提示，可以完成相关的功能要求。
 * @version 0.1
 * @command gcc -g main.c -o main.exe  -fexec-charset=GBK
 * @date 2022-06-23（周四）
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<windows.h>
#include<time.h>

#define lessonNum 5           // 课程数量
#define ListIncrement 15      // 重新分配增量
#define ListInitLength 1000     // 表的初始化长度
#define _CRT_SECURE_NO_WARNINGS // 别tm报错，烦死了
 
typedef long LElemType;       // 长整
typedef char DataType;        // 字符
typedef int ElemType;         // 整型

typedef struct Birday{        // 出生年月定义
     int year;                // 年
     int month;               // 月
     int day;                 // 日
} DataBirth;

typedef struct Stu{                          // 学生结构体定义
     DataType StuID      [15];               // 学号
     DataType StuName    [10];               // 姓名
     DataType StuSex     [2];                // 性别
     DataBirth StuBorn;                      // 出生年月
     DataType StuPhone   [12];               // 电话
     DataType StuAddress [100];              // 住址
     ElemType StuScore   [lessonNum];        // 成绩
} Student;

typedef struct StuTable{                     // 学生表表
     Student *List; // 首地址
     ElemType length;    // 长度
     ElemType ListSize;  // 分配的长度
} StudentList;

typedef struct{                      // 成绩结构体
     int StuID;                              // 学生序号
     int StuScore;                           // 学生成绩
} _score;

void MainPanel();                                      // 主页面版
void ScorePanel();                                     // 成绩面板
void StuPanel();                                       // 学生基本信息面板
void initStuData();                                    // 初始化学生数据库
void initStuScoreData();                               // 初始化学生成绩数据库
void StuDataInput(StudentList *DATA);                  // 学生数据导入
void StuDataOutput(StudentList *DATA);                 // 学生数据导出
void StuScoreInput(StudentList *DATA);                 // 学生成绩导入
void StuScoreOutput(StudentList *DATA);                // 学生成绩导出
void StuInput(StudentList *LIST);                      // 输入学生数据
void AllowMalloc(StudentList *LIST);                   // 重新分配学生数据
void InsertStu(StudentList *LIST, Student elem);       // 增加学生（单个）
void InitStulist(StudentList *LIST);                   // 初始化学生线性表
void PrintStulist(StudentList *LIST);                  // 输出学生信息
void SelectStuData(StudentList *LIST);                 // 查询学生信息面板
void QueryStuData(StudentList *LIST, int flag);        // 查找学生信息
void ChangeStuData(StudentList *LIST, int flag);       // 修改学生信息
void UpdateChangeStuData(StudentList *LIST, int flag); // 修改学生信息
void ChangeStuDataPanel(StudentList *LIST);            // 修改学生信息面板
void DeleteStuData(StudentList *LIST);                 // 删除学生信息面板
int IsEmpty(StudentList *LIST);                        // 判断是否为空
void DeleteAndStuData(StudentList *LIST);              // 删除学生信息
void DeleteElem(StudentList *LIST, int pos);           // 删除学生条目
void InsertScoreData(StudentList *LIST);               // 添加成绩信息
void DisplayScore(StudentList *LIST);                  // 打印成绩信息
void ChangeStuScore(StudentList *LIST);                // 修改成绩信息
void SortPanel(StudentList *LIST);                     // 排序面板
void SortTotal(StudentList *LIST);                     // 按照总分进行排序
void SortLesson(StudentList *LIST);                    // 按照课程进行排序
void BubbleSort(_score *a,int n);                      // 冒泡排序
time_t GetTimeNow(time_t *InputTime);                  // 获取现在的时间

void StuDataInput(StudentList *DATA){        // 学生数据导入
	FILE *fp;
     int i = 0;
     fp = fopen("./students.txt", "r");
     if (fp == NULL)
          return 0;
     fscanf(fp, "%d", &DATA->length);
     for (int i = 0; i < DATA->length; i++)
     {
          fscanf(fp,"%s %s %s %d-%d-%d %s %s\n", &DATA->List[i].StuID, &DATA->List[i].StuName, &DATA->List[i].StuSex, &DATA->List[i].StuBorn.year, &DATA->List[i].StuBorn.month, &DATA->List[i].StuBorn.day, &DATA->List[i].StuPhone, &DATA->List[i].StuAddress);
     }
	fclose(fp);
}

void StuDataOutput(StudentList *DATA){                        // 学生数据导出       
	FILE *stuData;
	if((stuData = fopen("./students.txt","wb"))==NULL){
		printf("[!] 不能保存文件");	
	}
     fprintf(stuData, "%d\n", DATA->length); // 输出长度
     for (int i = 0; i < DATA->length; i++)
     {
          fprintf(stuData,"%s %s %s %d-%d-%d %s %s\n", DATA->List[i].StuID, DATA->List[i].StuName, DATA->List[i].StuSex, DATA->List[i].StuBorn.year, DATA->List[i].StuBorn.month, DATA->List[i].StuBorn.day, DATA->List[i].StuPhone, DATA->List[i].StuAddress);
     }
	// if(fwrite(&DATA,sizeof(StudentList), 1, stuData)!=1){
	// 	printf("[!] 数据写入失败");
	// }
	fclose(stuData);
}

void StuScoreInput(StudentList *DATA){             // 学生成绩导出
	FILE *fp;
     int i = 0;
     fp = fopen("./score.txt", "r");
     if (fp == NULL)
          return 0;
     fscanf(fp, "%d", &DATA->length);
     for (int i = 0; i < DATA->length; i++)
     {
          fscanf(fp,"%s %s %d %d %d %d %d\n", &DATA->List[i].StuName, &DATA->List[i].StuID, &DATA->List[i].StuScore[0], &DATA->List[i].StuScore[1], &DATA->List[i].StuScore[2], &DATA->List[i].StuScore[3], &DATA->List[i].StuScore[4]);
     }
	fclose(fp);
}

void StuScoreOutput(StudentList *DATA){                // 学生成绩导出
	FILE *stuData;
	if((stuData = fopen("./score.txt","wb"))==NULL){
		printf("[!] 不能保存文件");	
	}
     fprintf(stuData, "%d\n", DATA->length); // 输出长度
     for (int i = 0; i < DATA->length; i++)
     {
          fprintf(stuData,"%s %s ", DATA->List[i].StuName, DATA->List[i].StuID);
          for (int j = 0; j < lessonNum; j++)
          {
               fprintf(stuData, "%d ", DATA->List[i].StuScore[j]);
          }
          fprintf(stuData, "\n");
     }
     fclose(stuData);
}

void SortLesson(StudentList *LIST){
	int i,num,j;
	int len=LIST->length;
	_score *a=malloc(sizeof(_score)*len);
	printf("[*] 请输入排序科目序号");
	printf("[*] 1->2->3->4->5");
	printf(">>>> ");
     scanf("%d",&num);
	for(i=0;i<len;i++)
	{
		a[i].StuID=i;
          a[i].StuScore=LIST->List[i].StuScore[num-1];
	}
	BubbleSort(a,len);
	printf("%-15s%-10s%5s%d\n","学号","姓名","成绩",num);
	for(i=0;i<len;i++)
	{
		printf("%-15s%-10s%d\n",LIST->List[a[i].StuID].StuID,LIST->List[a[i].StuID].StuName,a[i].StuScore);
	}
	printf("\n");
	return;
}

void BubbleSort(_score *a,int n)
{
	_score tmp;
	int i,j; 
	for (i=0; i<n; i++)            //i表示比较趟数，0到n-2有效(总共n-1趟)
     {
          for (j=0; j<n-1-i; j++) //j表示每趟比较次数
          {    
     
               if (a[j].StuScore < a[j+1].StuScore)    //交换相邻两个元素的值
               {
                    tmp    = a[j];
                    a[j]   = a[j+1];
                    a[j+1] = tmp;
               }            
          }
     }
}

void EnCrypto(DataType crypto[1000]){
     int i;
     char salt[100] = "!@#^&*()WERTYUIOP741852963/*-+";
     int templ = strlen(crypto);
     char rainbal[1000] = {'\0'};
     for (i = 0; i < templ; i++)
     {
          rainbal[i] = crypto[i] + i + 4;
     }
     rainbal[i] = '\0';
     printf("%s\n", rainbal);
}

void DeCrypto(DataType crypto[1000]){
     int i;
     char salt[100] = "!@#^&*()WERTYUIOP741852963/*-+";
     int templ = strlen(crypto);
     char rainbal[1000] = {'\0'};
     for (i = 0; i < templ; i++)
     {
          rainbal[i] = crypto[i] - i - 4;
     }
     rainbal[i] = '\0';
     printf("%s\n", rainbal);
}

void SortTotal(StudentList *LIST){                // 按照总分进行排序
	int i,sum,j;
	int len=LIST->length;
	_score *a=malloc(sizeof(_score)*len);
	for(i=0;i<len;i++)
	{
		sum=0;
		a[i].StuID=i;
		for(j=0;j<lessonNum;j++)
		    sum+=LIST->List[i].StuScore[j];
          a[i].StuScore=sum;
	}
	BubbleSort(a,len);
	printf("%-15s%-10s%-5s\n","学号","姓名","总分");
	for(i=0;i<len;i++)
	{
		printf("%-15s%-10s%d\n",LIST->List[a[i].StuID].StuID,LIST->List[a[i].StuID].StuName,a[i].StuScore);
	}
	printf("\n");
	return;
}

void SortPanel(StudentList *LIST){
     int command;             // 控制台指令
     system("cls");
     system("title 排序成绩");
     while (1)
     {
          printf("[*] 1.按照总分进行排序\n");
          printf("[*] 2.按照某门课程进行排序\n");
          printf("[*] 0.返回上一级\n");
          printf(">>> ");
          scanf("%d", &command);
          switch (command)
          {
               case 0:             // 返回上一级
                    break;
               case 1:             // 总分排序
                    SortTotal(LIST);
                    break;
               case 2:             // 单科排序
                    SortLesson(LIST);
                    break;
               default:
                    system("cls");
                    printf("[!] 您输入的信息有误！请重新输入！\n");
                    break;
          }
          if (command == 0)
          {
               break;
          }
          scanf("%d", &command);
     }
     return;
}

void ChangeStuScore(StudentList *LIST){        // 修改成绩信息
     char name[20];
     int tempj, j, newj;
     system("title 请输入学号");
     printf("[*] 请输入学号\n");
     printf(">>>");
     scanf("%s", name);
     for (int i = 0; i < LIST->length; i++)
     {
          if (strcmp(name, LIST->List[i].StuID) == 0)
          {

               printf("[*] 请输入要修改的科目\n");
               for (int i = 0; i < lessonNum; i++)
               {
                    printf("%d.成绩[%d] - ", i+1, i+1);
               }
               printf("\n");
               printf(">>>> ");
               scanf("%d", &j);
               tempj = LIST->List[i].StuScore[j - 1];
               printf("[*] 请输入新的分数！\n");
               printf(">>>>> ");
               scanf("%d", &LIST->List[i].StuScore[j - 1]);
               printf("[!] 原来的分数: %d\n", tempj);
               printf("[!] 修改后的分数: %d\n", LIST->List[i].StuScore[j - 1]);
               system("pause");
               return;
          }
     }
     printf("[!] 此学号不存在！\n");
     getch();
     return;
}

void DisplayScore(StudentList *LIST){
     int i;
     int aaa=0;
     int bbbb=0;
     int ccccc=0;
     int dddddd=0;
     int j;
     system("title 显示成绩");
     system("cls");
     printf("%-15s%-10s","学号","姓名");
     for(i=0;i<lessonNum;i++){
          printf("\t成绩[%d]",i+1);
     }
     printf("\t等级分布");
     printf("\n");
     for(i=0; i<LIST->length; i++)  
     {  
          aaa=0;
          bbbb=0;
          ccccc=0;
          dddddd=0;
          printf("%-15s%-10s",LIST->List[i].StuID,LIST->List[i].StuName);  
          for(j=0;j<lessonNum;j++){
               if (LIST->List[i].StuScore[j] >= 0 && LIST->List[i].StuScore[j] < 60)
               {
                    dddddd++;
                    printf("\t%-3d[D]",LIST->List[i].StuScore[j]);
               }else if(LIST->List[i].StuScore[j] >= 60 && LIST->List[i].StuScore[j] < 90){
                    ccccc++;
                    printf("\t%-3d[C]",LIST->List[i].StuScore[j]);
               }else if(LIST->List[i].StuScore[j] >= 90 && LIST->List[i].StuScore[j] < 120){
                    bbbb++;
                    printf("\t%-3d[B]",LIST->List[i].StuScore[j]);
               }else if(LIST->List[i].StuScore[j] >= 120 && LIST->List[i].StuScore[j] <= 150){
                    aaa++;
                    printf("\t%-3d[A]",LIST->List[i].StuScore[j]);
               }
          }
          printf("\t[A]:%d [B]:%d [C]:%d [D]:%d", aaa,bbbb,ccccc,dddddd);
          printf("\n");
     }  
     printf("\n"); 
     system("pause");
}
void InsertScoreData(StudentList *LIST){
     char name[20];
     system("title 请输入学号");
     printf("[*] 请输入学号\n");
     printf(">>>");
     scanf("%s", name);
     for (int i = 0; i < LIST->length; i++)
     {
          if (strcmp(name, LIST->List[i].StuID) == 0)
          {
               printf("请输入 [%d] 门成绩：\n",lessonNum); 
               for(int j=0;j<lessonNum;j++){
                    int tempScore = 0;
                    // char *str = "";
                    // sprintf(str, "title 第 [%d] 门成绩", j);
                    // system(str);
                    scanf("%d",&tempScore);
                    if (tempScore >= 0  && tempScore <= 150)
                    {
                         LIST->List[i].StuScore[j] = tempScore;
                         continue;
                    }else{
                         LIST->List[i].StuScore[j] = 0;
                         printf("[!] 成绩输入错误！ 150>=x<=0\n");
                    }
               }
               printf("输入成功！\n");
               getch(); 
               return;
          }
     }
     printf("[!] 此学号不存在！\n");
     getch();
     return;
}

int IsEmpty(StudentList *LIST){
     if (LIST->length == 0)
     {
          return 1;
     }else{
          return 0;
     }
}

void DeleteElem(StudentList *LIST, int pos){
     int i;   
     if(pos<0||pos>LIST->length)  
     {  
          printf("[!] 越界\n");  
          exit(1);  
     }
     for(i=pos; i<LIST->length - 1; i++)  
          LIST->List[i]=LIST->List[i + 1];  
     LIST->length--;  
     return ;  
}

void DeleteAndStuData(StudentList *LIST){              // 删除学生信息
     char name[20];
     int i, cm;
     system("title 删除学生信息");
     if (IsEmpty(LIST) == 1)
     {
          printf("[!] 学生表未创建！");
          return;
     }
     printf(">>>> ");
     scanf("%s", name);
     for (i = 0; i < LIST->length; i++)
     {
          if (strcmp(name, LIST->List[i].StuID) == 0)
          {
               printf("学号：%s\n", LIST->List[i].StuID);
               printf("姓名： %s\n", LIST->List[i].StuName);
               printf("[*] 是否删除？ 删除 [1] 取消[0]: ");
               scanf("%d", &cm);
               switch (cm)
               {
                    case 1:
                         DeleteElem(LIST, i);
                         printf("已删除\n");
                         break;
                    case 0:
                         break;
                    default:
                         printf("输入错误！");
                         system("pause");
               }               
               return;
          }
     }
     printf("[!] 未找到！\n");
     return;
}

void DeleteStuData(StudentList *LIST){                 // 删除学生信息面板
     int command;             // 控制台指令
     system("cls");
     system("title 删除学生信息");
     while (1)
     {
          printf("[*] 1.输入学号进行删除\n");
          printf("[*] 0.返回上一级\n");
          printf(">>> ");
          scanf("%d", &command);
          switch (command)
          {
               case 0:             // 返回上一级
                    break;
               case 1:             // 输入学号进行修改
                    DeleteAndStuData(LIST);
                    break;
          }
          if (command == 0)
          {
               break;
          }
          scanf("%d", &command);
     }
     return;
}

void ChangeStuDataPanel(StudentList *LIST){                                     // 修改学生数据面板
     int command;             // 控制台指令
     system("cls");
     system("title 修改学生信息");
     while (1)
     {
          printf("[*] 1.输入学号进行修改\n");
          printf("[*] 2.输入姓名进行修改\n");
          printf("[*] 0.返回上一级\n");
          printf(">>> ");
          scanf("%d", &command);
          switch (command)
          {
               case 0:             // 返回上一级
                    break;
               case 1:             // 输入学号进行修改
                    ChangeStuData(LIST, command);
                    break;
               case 2:             // 输入姓名进行修改
                    ChangeStuData(LIST, command);
                    break;
               default:
                    system("cls");
                    printf("[!] 您输入的信息有误！请重新输入！\n");
                    break;
          }
          if (command == 0)
          {
               break;
          }
          scanf("%d", &command);
     }
     return;
}

void ChangeStuData(StudentList *LIST, int flag){     // 修改学生信息面板
     char name[20];
     printf(">>>> ");
     scanf("%s", name);
     system("title 修改学生信息");
     for (int i = 0; i < LIST->length; i++)
     {
          if (flag == 1 && strcmp(name, LIST->List[i].StuID) == 0)
          {
               UpdateChangeStuData(LIST, i);
               return;
          }
          if (flag == 2 && strcmp(name, LIST->List[i].StuName) == 0)
          {
               UpdateChangeStuData(LIST, i);
               return;
          }
     }
     return;
}

void UpdateChangeStuData(StudentList *LIST, int i){          // 修改学生信息
     int command;
     // char *TempStr;
     // strcpy(TempStr, LIST->List[i].StuID);
     // strcat(TempStr, LIST->List[i].StuName);
     // strcat(TempStr, LIST->List[i].StuSex);
     // strcat(TempStr, LIST->List[i].StuBorn.year);
     // strcat(TempStr, "-");
     // strcat(TempStr, LIST->List[i].StuBorn.month);
     // strcat(TempStr, "-");
     // strcat(TempStr, LIST->List[i].StuBorn.day);
     // strcat(TempStr, LIST->List[i].StuPhone);
     // strcat(TempStr, LIST->List[i].StuAddress);
     system("title 修改学生信息");
     printf("格式：\n(学号1-> 姓名->2 性别->3 生日->4 电话->5 地址->6)\n");
     printf(">>>>> ");
     scanf("%d", &command);
     switch (command)
     {
          case 1:
               printf(">>>>>> ");
               scanf("%s", LIST->List[i].StuID);
               break;
          case 2:
               printf(">>>>>> ");
               scanf("%s", LIST->List[i].StuName);
               break;
          case 3:
               printf(">>>>>> ");
               scanf("%s", LIST->List[i].StuSex);
               break;
          case 4:
               printf(">>>>>> ");
               printf("新格式：(####-##-##)\n");
               scanf("%d-%d-%d", &LIST->List[i].StuBorn.year,  &LIST->List[i].StuBorn.month,  &LIST->List[i].StuBorn.day);
               break;
          case 5:
               printf(">>>>>> ");
               scanf("%s", LIST->List[i].StuPhone);
               break;
          case 6:
               printf(">>>>>> ");
               scanf("%s", LIST->List[i].StuAddress);
               break;
          default:
               system("cls");
               printf("[!] 您输入的信息有误！请重新输入！\n");
               break;
     }
     printf("[*] 修改成功！\n[*] 数据更新后 %-15s%-10s%-10s%-4d-%-2d-%-2d %-12s %s", LIST->List[i].StuID, LIST->List[i].StuName, LIST->List[i].StuSex, LIST->List[i].StuBorn.year, LIST->List[i].StuBorn.month, LIST->List[i].StuBorn.day, LIST->List[i].StuPhone, LIST->List[i].StuAddress);
     return;
}

void SelectStuData(StudentList *LIST){                  // 查询学生信息面板
     int command;             // 控制台指令
     system("cls");
     system("title 查询学生信息");

     while (1)
     {
          printf("[*] 1.学号查询\n");
          printf("[*] 2.姓名查询\n");
          printf("[*] 5.按性别查询\n");
          printf("[*] 0.返回上一级\n");
          printf(">>> ");
          scanf("%d", &command);
          switch (command)
          {
               case 0:             // 返回上一级
                    break;
               case 1:             // 学号查询
                    QueryStuData(LIST, command);
                    break;
               case 2:             // 姓名查询
                    QueryStuData(LIST, command);
                    break;
               case 5:             // 查询所有N性
                    QueryStuData(LIST, command);
                    break;
               default:
                    system("cls");
                    printf("[!] 您输入的信息有误！请重新输入！\n");
                    break;
          }
          if (command == 0)
          {
               break;
          }
          scanf("%d", &command);
     }
     return;
}

void InitStulist(StudentList *LIST){              // 初始化学生线性表
     LIST->List = (Student *)malloc(ListInitLength * sizeof(Student));
     if (!LIST->List)
     {
          exit("Error!\n");
     }else{
          LIST->length = 0;                  // 初始化为空表
          LIST->ListSize = ListInitLength;   // 初始化表容量
     }
     return;
}

void QueryStuData(StudentList *LIST, int flag){
     // 1.学号查询
     // 2.姓名查询
     // 3.按学号查询成绩
     int i;
     if (flag == 5)
     {
          printf("%-15s%-10s%-10s%-11s%-12s %s\n", "学号", "姓名", "性别", "生日", "电话", "地址");
          printf("男性有：\n");
          for (i = 0; i < LIST->length; i++)
          {
               if (strcmp("男", LIST->List[i].StuSex) == 0)
               {
                    printf("%-15s%-10s%-10s%-4d-%-2d-%-2d %-12s %s\n", LIST->List[i].StuID, LIST->List[i].StuName, LIST->List[i].StuSex, LIST->List[i].StuBorn.year, LIST->List[i].StuBorn.month, LIST->List[i].StuBorn.day, LIST->List[i].StuPhone, LIST->List[i].StuAddress);
               }
          }
          printf("女性有：\n");
          for (i = 0; i < LIST->length; i++)
          {
               if (strcmp("女", LIST->List[i].StuSex) == 0)
               {
                    printf("%-15s%-10s%-10s%-4d-%-2d-%-2d %-12s %s\n", LIST->List[i].StuID, LIST->List[i].StuName, LIST->List[i].StuSex, LIST->List[i].StuBorn.year, LIST->List[i].StuBorn.month, LIST->List[i].StuBorn.day, LIST->List[i].StuPhone, LIST->List[i].StuAddress);
               }
          }
     }else{
          char name[20];
          printf(">>>> ");
          scanf("%s", name);
          for (i = 0; i < LIST->length; i++)
          {
               if (flag == 1 && strcmp(name, LIST->List[i].StuID) == 0)    // 
               {
                    system("title 按学号查询");
                    printf("%-15s%-10s%-10s%-11s%-12s %s\n", "学号", "姓名", "性别", "生日", "电话", "地址");
                    printf("%-15s%-10s%-10s%-4d-%-2d-%-2d %-12s %s\n", LIST->List[i].StuID, LIST->List[i].StuName, LIST->List[i].StuSex, LIST->List[i].StuBorn.year, LIST->List[i].StuBorn.month, LIST->List[i].StuBorn.day, LIST->List[i].StuPhone, LIST->List[i].StuAddress);
                    return;
               }
          }
          for (i = 0; i < LIST->length; i++)
          {
               if (flag == 2 && strcmp(name, LIST->List[i].StuName) == 0)    // 
               {
                    system("title 按姓名查询");
                    printf("%-15s%-10s%-10s%-11s%-12s %s\n", "学号", "姓名", "性别", "生日", "电话", "地址");
                    printf("%-15s%-10s%-10s%-4d-%-2d-%-2d %-12s %s\n", LIST->List[i].StuID, LIST->List[i].StuName, LIST->List[i].StuSex, LIST->List[i].StuBorn.year, LIST->List[i].StuBorn.month, LIST->List[i].StuBorn.day, LIST->List[i].StuPhone, LIST->List[i].StuAddress);
                    return;
               }
          }
          for (i = 0; i < LIST->length; i++)
          {
               if (flag == 3 && strcmp(name, LIST->List[i].StuID) == 0)    // 成绩查询
               {
                    system("cls");
                    printf("姓名：%s\n", LIST->List[i].StuName);
                    printf("学号：%s\n", LIST->List[i].StuID);
                    printf("-----------------------------------------------------------------------\n");
                    for (int j = 0; j < lessonNum; j++)
                    {
                         printf("成绩 [%d]\t", j+1);
                    }
                    printf("\n");
                    for (int j = 0; j < lessonNum; j++)
                    {
                         printf("%d                ", LIST->List[i].StuScore[j]);
                    }
                    printf("\n");
                    system("pause");
                    return;
               }
          }
          return;
     }
}

void AgainMalloc(StudentList *LIST){                   // 重新分配学生数据
     Student *NewMemory;           // Temp
     NewMemory = (Student *)realloc(LIST->List,(LIST->ListSize+ListIncrement)*sizeof(Student));        // 重新分配
     if (!NewMemory)
     {
          exit("Error!\n");
     }else{
          LIST->List = NewMemory;
          LIST->ListSize += ListIncrement;
          return;
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

void PrintStulist(StudentList *LIST){               // 输出学生信息
     int *t, year, month, day;
     time_t ta;
     t = GetTimeNow(&ta);
     year = *t;
     month = t[1];
     day = t[2];
     system("title 学生列表");
     printf("%-15s%-10s%-10s%-11s%-12s %s\n", "学号", "姓名", "性别", "生日", "电话", "地址");
     for (int i = 0; i < LIST->length; i++)
     {
          if (year == LIST->List[i].StuBorn.year && month == LIST->List[i].StuBorn.month && day == LIST->List[i].StuBorn.day)
          {
               printf("%-15s%-10s%-10s【今天过生日！】 %-12s %s\n", LIST->List[i].StuID, LIST->List[i].StuName, LIST->List[i].StuSex, LIST->List[i].StuPhone, LIST->List[i].StuAddress);
          }else{
               printf("%-15s%-10s%-10s%-4d-%-2d-%-2d %-12s %s\n", LIST->List[i].StuID, LIST->List[i].StuName, LIST->List[i].StuSex, LIST->List[i].StuBorn.year, LIST->List[i].StuBorn.month, LIST->List[i].StuBorn.day, LIST->List[i].StuPhone, LIST->List[i].StuAddress);
          }
          
     }
     printf("[*] 打印大成功！共计 [%d] 条学生数据！\n", LIST->length);
     system("pause");
     return;
}

void InsertStu(StudentList *LIST, Student elem){       // 增加学生（单个）
     if (LIST->length >= LIST->ListSize)               // 列表空间不足
     {
          AgainMalloc(LIST);
     }else{
          LIST->List[LIST->length] = elem;             // 学生插入学生表
          LIST->length++;
          return;
     }
}

void StuInput(StudentList *LIST){
     Student elem;
     int i;
     system("title 插入学生");
     printf("学号>>>");                 scanf("%s", elem.StuID);
     printf("姓名>>>");                 scanf("%s", elem.StuName);
     printf("性别>>>");                 scanf("%s", elem.StuSex);
     printf("出生日期(####-##-##)>>>"); scanf("%d-%d-%d", elem.StuBorn.year, elem.StuBorn.month, elem.StuBorn.day);
     printf("电话>>> ");                scanf("%s", elem.StuPhone);
     printf("地址>>> ");                scanf("%s", elem.StuAddress);
     for (i = 0; i < lessonNum; i++)
     {
          elem.StuScore[i];
     }
     InsertStu(LIST, elem);
     system("title 添加成功！");
     printf("[*] 添加成功！\n");
     system("pause");
     system("cls");
     return;
}

void initStuData(){      // 初始化学生数据库
     FILE *stuData = NULL;
     stuData = fopen("students.txt", "w+");
     fclose(stuData);
}

void initStuScoreData(){ // 初始化学生成绩数据库
     FILE *stuScore = NULL;
     stuScore = fopen("score.txt", "w+");
     fclose(stuScore);
}

void MainPanel(){   // 主页面版
     system("color 1f");
     int *t, year, month, day;
     time_t ta;
     t = GetTimeNow(&ta);
     year = *t;
     month = t[1];
     day = t[2];
     system("title 欢迎来到学生信息管理系统！");
     printf("=======================================\n");
     printf("                                       \n");
     printf("             >学生管理系统<            \n");
     printf("             1.基本信息管理            \n");
     printf("             2.基本成绩管理            \n");
     printf("             3.导入学生数据            \n");
     printf("             4.导出学生数据            \n");
     printf("             5.导入学生成绩            \n");
     printf("             6.导出学生成绩            \n");
     printf("             0.退出本程序              \n");
     printf("                                       \n");
     printf("        今天是 [%d-%d-%d]              \n", year, month, day);
     printf("                                       \n");
     printf("=======================================\n");

}

void ScorePanel(){  // 成绩面板
     system("color 1f");
     system("title 学生管理系统>学生成绩管理");
     printf("=======================================\n");
     printf("                                       \n");
     printf("       >学生管理系统->学生成绩管理<      \n");
     printf("             1.添加成绩信息            \n");
     printf("             2.查询成绩信息            \n");
     printf("             3.浏览成绩信息            \n");
     printf("             4.修改成绩信息            \n");
     printf("             5.排序成绩信息            \n");
     printf("             0.返回上一级              \n");
     printf("                                       \n");
     printf("=======================================\n");
}

void StuPanel(){    // 学生基本信息面板
     system("color 1f");
     system("title 学生管理系统>学生基本信息管理");
     printf("=======================================\n");
     printf("                                       \n");
     printf("      >学生管理系统->学生基本信息管理<      \n");
     printf("             1.添加学生信息            \n");
     printf("             2.浏览学生信息            \n");
     printf("             3.查询学生信息            \n");
     printf("             4.修改学生信息            \n");
     printf("             5.删除学生信息            \n");
     printf("             0.返回上一级              \n");
     printf("                                       \n");
     printf("=======================================\n");
}

void ScorePanelControl(StudentList *LIST){  // 成绩面板管理
     // 1.添加成绩信息
     // 2.查询成绩信息
     // 3.浏览成绩信息
     // 4.修改成绩信息
     // 5.排序成绩信息
     system("color 1f");
     int flags = 1;
     int command;   // 控制台指令
     while (flags)
     {
          system("cls");
          ScorePanel();
          printf(">> ");
          scanf("%d", &command);
          switch (command)
          {
               case 1:                  // 添加成绩信息
                    InsertScoreData(LIST);
                    break;
               case 2:                  // 查询成绩信息
                    system("title 请输入学号进行成绩查询");
                    QueryStuData(LIST, 3);
                    break;
               case 3:                  // 浏览成绩信息
                    DisplayScore(LIST);
                    break;
               case 4:                  // 修改成绩信息
                    ChangeStuScore(LIST);
                    break;
               case 5:                  // 排序成绩信息
                    SortPanel(LIST);
                    break;
               case 0:                  // 返回上一级
                    return;
               default:
                    system("cls");
                    printf("[!] 您输入的信息有误！请重新输入！\n");
                    scanf("%d", &command);
                    break;
          }
     }
}

void StuPanelControl(StudentList *LIST){  // 学生基本信息管理
     system("color 1f");
     int flags = 1;
     int command;   // 控制台指令
     while (flags)
     {
          system("cls");
          StuPanel();
          printf(">> ");
          scanf("%d", &command);
          switch (command)
          {
               case 1:                  // 添加学生信息
                    printf("依次输入：学号->姓名->性别->生日->电话->地址\n");
                    StuInput(LIST);
                    break;
               case 2:                  // 浏览学生信息
                    PrintStulist(LIST);
                    break;
               case 3:                  // 查询学生信息
                    SelectStuData(LIST);
                    break;
               case 4:                  // 修改学生信息
                    ChangeStuDataPanel(LIST);
                    break;
               case 5:                  // 删除学生信息
                    DeleteStuData(LIST);
                    break;
               case 0:                  // 返回上一级  
                    return;
               default:
                    system("cls");
                    printf("[!] 您输入的信息有误！请重新输入！\n");
                    scanf("%d", &command);
                    break;
          }
     }
}

int main(){
     int flags = 1;
     int command;             // 控制台指令
     // StudentList *LIST;
     StudentList LIST;
     InitStulist(&LIST);      // 初始化学生表
     while (flags)
     {
          system("cls");
          MainPanel();
          printf("> ");
          scanf("%d", &command);
          switch (command)
          {
               case 1:                       // 基本信息管理
                    StuPanelControl(&LIST);
                    break;
               case 2:                       // 基本成绩管理
                    ScorePanelControl(&LIST);
                    break;
               case 3:
                    StuDataInput(&LIST);     // 导入学生数据
                    break;
               case 4:                       // 导出学生数据
                    StuDataOutput(&LIST);
                    break;
               case 5:
                    StuScoreInput(&LIST);   // 导入学生成绩
                    break;
               case 6:
                    StuScoreOutput(&LIST);   // 导出学生成绩
               case 0:
                    printf("Byebye :)\n");
                    return;
                    break;
               default:
                    system("cls");
                    printf("[!] 您输入的信息有误！请重新输入！\n");
                    getch();
                    break;
          }
     }
     return 0;
}