/**
 * @file 校园导游系统
 * @author Po7mn1
 * @brief 
 * @version 0.1
 * @command gcc -g main.c -o main.exe  -fexec-charset=GBK
 * @date 2022-06-27（周一）
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXSIZE 100           // 最大定点数
#define FINITY 99999999       /*此处用5000代表无穷大*/
#define StackSize 10000000    // 设定栈空间
int Stack[StackSize];                                  // 路径栈，存储路径线路
int Top;                                               // 栈顶
int visited[StackSize];                                // 判断是否入栈
int count;                                             // 统计几条路
typedef int DataType;
typedef struct Node{
     int item;                // 序号
     char name[100];          // 地点名称
     char info[1000];         // 地点介绍
} Node;
typedef struct Graph{                   // 定义图结构体
     Node data[MAXSIZE];                // 地点信息
     DataType edges[MAXSIZE][MAXSIZE];  // 邻接矩阵
     DataType n;                        // 顶点数
     DataType e;                        // 边数
} Graph;
int djsave[MAXSIZE];
int count;
int shortlength[MAXSIZE];
int primqueue[MAXSIZE]={0};

void Menu();                                           // 主界面
void Create(Graph *map);                               // 读取文件并创建图
void Query(Graph *map);                                // 景点信息查询
void QueryNum(Graph *map, int num);                    // 按编号查询
void QueryName(Graph *map, char name[100]);            // 按名称查询
void Print(Graph *map);                                // 列出所有景点和道路信息
void Add(Graph *map);                                  // 添加边；新增景点信息
void Modify(Graph *map);                               // 修改景点和道路信息
void ModifyByNum(Graph *map, int num);                 // 输入景点编号修改景点信息
void ModifyByName(Graph *map, char name[100]);         // 输入景点编号修改景点信息
void ModifyRoad(Graph *map, int a, int b, int len);    // 修改路径信息，如原无路径则新增
void Delete(Graph *map);                               // 删除景点和道路信息
void DeleteByNum(Graph *map, int num);                 // 根据编号删除景点
void DeleteByName(Graph *map, char name[100]);         // 根据景点名删除景点
void DeleteByRoad(Graph *map, int a, int b);           // 删除道路信息
void Both(Graph *map);                                 // 任意两个景点的路径查询
void DFSSearch(Graph *map, int m, int a, int b);       // DFS任意两个景点的路径查询
int Dijkstra(Graph *map, int start);                   // 迪杰斯特拉
void Min(Graph *map);                                  // 两点最短路径


void DFSSearch( Graph *map, int m, int a, int b)
{
     // a 为起点
     // b 为重点
	int i, j, len;
	for (i = 0; i < map->n; i++)
	{
		if (map->edges[a][i] != FINITY && visited[i] == 0)// 起始结点到i存在路径且i未被访问
		{
			visited[i] = 1;                              // 如果被访问过了，该顶点设置为1
			if ( i == b )                                // 等于目标结点时输出
			{
				count++;
				printf("★第%d条路: ", count);
				for ( j = 0; j < m; j++ )
				{
					printf("%s->", map->data[Stack[j]].name );
                         if (j < j - 1)
						len += + map->edges[Stack[j]][Stack[j + 1]];      // 统计长度
				}
                    len += map->edges[Stack[j - 1]][b]; 
				printf("%s\n", map->data[b].name);
				printf("总长度是：[%dm]\n\n", len);
				visited[i] = 0;
			}else{                                       // 不等则存储结点
				Stack[m] = i;
				m++;
				DFSSearch(map, m, i, b);
				m--;
				visited[i] = 0;                         // 递归把这条路径上所有结点初始化为未访问
			}
		}
	}
}
void Both(Graph *map){        // 任意两个景点的路径查询
     int result; // eax
     int j = 0; // [esp+18h] [ebp-10h]
     int i = 0; // [esp+1Ch] [ebp-Ch]
     int a = 0; // [esp+20h] [ebp-8h]
     int b = 0; // [esp+24h] [ebp-4h]
     printf("请输入起始景点编号和终点景点编号: ");
     scanf("%d %d", &a, &b);
     // printf("%d %d", a, b);
     for (i = 0; map->n > i && map->data[i].item != a; ++i);
     for (j = 0; map->n > j && map->data[j].item != b; ++j);
     if (map->n != i && map->n != j )
     {
          if ( i == j )
               printf("起点和终点不能相同");
          else
               printf("从 [%s] 出发到 [%s] 的路径有以下几条：\n", map->data[i].name, map->data[j].name);
               DFSSearch(map, 1, a, b);
     }else{
          printf("输入的编号不存在\n");
     }
     system("pause");
     system("title 查找成功！");
     count = 1; // 趟数为0，否则会冲突
}

void DeleteByNum(Graph *map, int num){  // 输入景点编号修改景点信息
     int flag = FINITY;
     if (num != 0)
     {
          for (int i = 0; i < map->n; i++)   // 遍历顶点
          {
               if (map->data[i].item == num)
               {
                    flag = i;
               }
          }
     }
     if (flag == FINITY)
     {
          printf("对不起，无此编号！\n");
          system("title [!] Wrong Data! :D");
          system("pause");
          return;
     }
     for (int i = 0; i < map->n; i++)
     {
          map->data[i] = map->data[i + 1];
          map->n--;
     }
     system("cls");
     system("title 删除成功！");
     printf("删除成功！\n");
     system("pause");
     return;
}

void DeleteByName(Graph *map, char name[100]){
     int flag = FINITY;
     for (int i = 0; i < map->n; i++)   // 遍历顶点
     {
          if (strcmp(map->data[i].name, name) == 0)
          {
               flag = i;
          }
     }
     if (flag == FINITY)
     {
          printf("对不起，无此编号！\n");
          system("title [!] Wrong Data! :D");
          system("pause");
          return;
     }
     for (int i = flag; i < map->n; i++)
     {
          map->data[i] = map->data[i + 1];
          map->n--;
     }
}

void DeleteByRoad(Graph *map, int a, int b){
     int flag = FINITY;
     if (a != 0)
     {
          for (int i = 0; i < map->n; i++)   // 遍历顶点
          {
               if (map->data[i].item == a)
               {
                    flag = i;
               }
          }
     }
     if (flag == FINITY)
     {
          printf("对不起，无此编号！\n");
          system("title [!] Wrong Data! :D");
          system("pause");
          return;
     }
     if (b != 0)
     {
          for (int i = 0; i < map->n; i++)   // 遍历顶点
          {
               if (map->data[i].item == b)
               {
                    flag = i;
               }
          }
     }
     if (flag == FINITY)
     {
          printf("对不起，无此编号！\n");
          system("title [!] Wrong Data! :D");
          system("pause");
          return;
     }
     map->edges[a][b] = FINITY;
     map->edges[b][a] = FINITY;
     printf("道路删除完成！\n");
     return;
}

void Delete(Graph *map){
     int command, iNt;
     int a,b,len;
     char name[100];
     printf("\n\n1：输入景点编号删除景点信息\n");
     printf("2：输入景点名称删除景点信息\n");
     printf("3：删除道路信息\n");
     printf("4：返回上一级\n");
     printf(">> ");
     scanf("%d", &command);
     switch (command)
     {
          case 1:
               printf("请输入景点编号: ");
               scanf("%d", (int)&iNt);
               DeleteByNum(map, iNt);
               break;
          case 2:
               printf("请输入景点名称: ");
               scanf("%s", name);
               DeleteByName(map, name);
               break;
          case 3:
               printf("请输入待删除路径两顶点编号，空格隔开。\n");
               scanf("%d %d", &a, &b);
               DeleteByRoad(map, a, b);
               break;
          case 4:
               return;
          default:
               system("cls");
               system("title [!] Wrong Data! :D");
               printf("[!] 您输入的信息有误！请重新输入！\n");
               system("pause");
               break;
     }
}

void ModifyByName(Graph *map, char name[100]){
     int flag = FINITY;
     for (int i = 0; i < map->n; i++)   // 遍历顶点
     {
          if (strcmp(map->data[i].name, name) == 0)
          {
               flag = i;
          }
     }
     if (flag == FINITY)
     {
          printf("对不起，无此编号！\n");
          system("title [!] Wrong Data! :D");
          system("pause");
          return;
     }
     system("title 修改节点");
     printf("请输入该景点修改后的名称和介绍：\n");
     printf("名称：");
     scanf("%s", map->data[flag].name);
     printf("介绍：");
     scanf("%s", map->data[flag].info);
     printf("[*] 修改成功！\n");
     system("pause");
     return;
}

void ModifyRoad(Graph *map, int a, int b, int len){
     for (int i = 0; i < map->n; i++)
     {
          for (int j = 0; j < map->e; j++)
          {
               if (map->edges[i][j] != 0)
               {
                    if (map->data[i - 1].item == a && map->data[j - 1].item == b)
                    {
                         map->edges[i][j] = len;
                    }
               }
          }
     }
     system("title [*] 修改成功");
     printf("修改完成！\n");
     system("pause");
}

void ModifyByNum(Graph *map, int num){  // 输入景点编号修改景点信息
     int flag = FINITY;
     if (num != 0)
     {
          for (int i = 0; i < map->n; i++)   // 遍历顶点
          {
               if (map->data[i].item == num)
               {
                    flag = i;
               }
          }
     }
     if (flag == FINITY)
     {
          printf("对不起，无此编号！\n");
          system("title [!] Wrong Data! :D");
          system("pause");
          return;
     }
     system("title 修改节点");
     printf("请输入该景点修改后的名称和介绍：\n");
     printf("名称：");
     scanf("%s", map->data[flag].name);
     printf("介绍：");
     scanf("%s", map->data[flag].info);
     printf("[*] 修改成功！\n");
     system("pause");
     return;
}

void Modify(Graph *map){           // 修改景点和道路信息
     int command, iNt;
     int a,b,len;
     char name[100];
     printf("\n\n1：输入景点编号修改景点信息\n");
     printf("2：输入景点名称修改景点信息\n");
     printf("3：修改路径信息，如原无路径则新增\n");
     printf("4：返回上一级\n");
     printf(">> ");
     scanf("%d", &command);
     switch (command)
     {
          case 1:
               printf("请输入景点编号: ");
               scanf("%d", (int)&iNt);
               ModifyByNum(map, iNt);
               break;
          case 2:
               printf("请输入景点名称: ");
               scanf("%s", name);
               ModifyByName(map, name);
               break;
          case 3:
               printf("请输入待修改路径信息,如原无路径则新增。\n");
               printf("格式形如(1-2,60),表示编号 1 2，路径长度60：\n");
               scanf("%d-%d,%d", &a, &b, &len);
               ModifyRoad(map, a, b, len);
               break;
          case 4:
               return;
          default:
               system("cls");
               system("title [!] Wrong Data! :D");
               printf("[!] 您输入的信息有误！请重新输入！\n");
               system("pause");
               break;
     }
}

void Add(Graph *map){              // 添加边；新增景点信息
     int result, location;
     if (map->n == MAXSIZE)
     {
          printf("[!] 已经达到最大景点数\n");
     }else{
          printf("请输入待新增景点信息\n");
          printf("景点编号: ");
          scanf("%d", &location);
          for (int i = 0; i < map->n; i++)
          {
               if (map->data[i].item == location)
               {
                    printf("[!] 该编号已经存在\n");
                    system("pause");
                    return;
               }
          }
          map->data[map->n].item = location; // 将景点编号赋值给新顶点
          printf("景点名称: ");
          scanf("%s", map->data[map->n].name);
          printf("景点介绍: ");
          scanf("%s", map->data[map->n].info);
          map->n++;                // 顶点+1
          printf("[*] 新增景点信息完成！\n");
          system("pause");
          return;
     }
}

void Print(Graph *map){
     printf("\n%-20s%-20s%-20s\n\n", "景点编号", "景点名称", "景点介绍");
     for (int i = 0; i < map->n; i++)
     {
          printf("%-20d%-20s%-20s\n", map->data[i].item, map->data[i].name, map->data[i].info);
     }
     printf("道路信息\n");
     for (int i = 0; i < map->n; i++)
     {
          for (int j = 0; j < map->n; j++)
          {
               if (map->edges[i][j] != FINITY)
               {
                    printf("景点 [%d--%s] 和景点 [%d--%s] 存在道路，长度：[%dm]\n", map->data[i - 1].item, map->data[i - 1].name, map->data[j - 1].item, map->data[j - 1].name, map->edges[i][j]);
               }
          }
     }
     system("title 遍历成功");
     system("pause");
}

void Create(Graph *map)
{
     FILE *fp = NULL;
     fp = fopen("./map.txt", "r"); //相对路径下读取文件
     if (fp == NULL)
          return 0;

     // map->data = (Node *)malloc(100 * sizeof(Node));
     fscanf(fp, "%d %d", &map->n, &map->e); //读取顶点数&边数
     for (int i = 0; i < map->n; i++)            //读取地点信息
          fscanf(fp, "%d %s %s", &map->data[i].item , &map->data[i].name, &map->data[i].info);

     for (int i = 0; i < MAXSIZE; i++) //初始化邻接矩阵
          for (int j = 0; j < MAXSIZE; j++)
               map->edges[i][j] = FINITY;
     int i, j, k;
     while (fscanf(fp, "%d%d%d", &i, &j, &k) != EOF) //读取矩阵信息
          map->edges[i][j] = k;

     return 1;
}


void QueryNum(Graph *map, int num){     // 按编号查询
     int flag = FINITY;
     if (num != 0)
     {
          for (int i = 0; i < map->n; i++)   // 遍历顶点
          {
               if (map->data[i].item == num)
               {
                    flag = i;
                    printf("%d\n", map->data[i].item);
               }
          }
     }

     if (flag == FINITY)
     {
          printf("对不起，无此编号！\n");
          system("title [!] Wrong Data! :D");
          system("pause");
          return;
     }
     // 景点编号：1
     // 景点名称：一期食堂
     // 景点介绍：这是学生公寓一期最近的学生食堂

     // 该景点路径信息：
     // 到景点2--学生公寓一期的距离为80
     // 到景点3--明理园的距离为100
     system("title 查询成功");
     printf("景点编号：%d\n", map->data[flag].item);
     printf("景点编号：%s\n", map->data[flag].name);
     printf("景点编号：%s\n\n", map->data[flag].info);
     printf("该景点路径信息：\n");
     // printf("flag: %d\n", flag);
     for (int j = 0; j < map->e; j++)   // 寻找相邻的节点，e边
     {
          if (map->edges[flag + 1][j] != 0)
          {
               // printf("到景点2--学生公寓一期的距离为80\n", map->data[j - 1].item, map->data[j - 1].name, map->edges[flag + 1][j]);
               printf("到景点 [%d] --> [%s] 的距离为 [%d]\n", map->data[j - 1].item, map->data[j - 1].name, map->edges[flag + 1][j]);
          }
     }
     system("pause");
     return;
}

void QueryName(Graph *map, char name[100]){     // 按名称查询
     int flag = FINITY;
     for (int i = 0; i < map->n; i++)   // 遍历顶点
     {
          if (strcmp(map->data[i].name, name) == 0)
          {
               flag = i;
          }
     }
     if (flag == FINITY)
     {
          printf("对不起，无此地点！\n");
          system("title [!] Wrong Data! :D");
          system("pause");
          return;
     }
     // 景点编号：1
     // 景点名称：一期食堂
     // 景点介绍：这是学生公寓一期最近的学生食堂

     // 该景点路径信息：
     // 到景点2--学生公寓一期的距离为80
     // 到景点3--明理园的距离为100
     system("title 查询成功");
     printf("景点编号：%d\n", map->data[flag].item);
     printf("景点编号：%s\n", map->data[flag].name);
     printf("景点编号：%s\n\n", map->data[flag].info);
     printf("该景点路径信息：\n");
     // printf("flag: %d\n", flag);
     for (int j = 0; j < map->e; j++)   // 寻找相邻的节点，e边
     {
          if (map->edges[flag + 1][j] != 0)
          {
               // printf("到景点2--学生公寓一期的距离为80\n", map->data[j - 1].item, map->data[j - 1].name, map->edges[flag + 1][j]);
               printf("到景点%d--%s的距离为%d\n", map->data[j - 1].item, map->data[j - 1].name, map->edges[flag + 1][j]);
          }
     }
     system("pause");
     return;
}

void Query(Graph *map){
     int command, iNt;
     char name[100];
     printf("\n\n1：按照景点编号查询\n");
     printf("2：按照景点名称查询\n");
     printf("3：列出所有景点和道路信息\n");
     printf("4：返回上一级\n");
     printf(">> ");
     scanf("%d", &command);
     switch (command)
     {
          case 1:
               printf("请输入景点编号: ");
               scanf("%d", (int)&iNt);
               QueryNum(map, iNt);
               break;
          case 2:
               printf("请输入景点名称: ");
               scanf("%s", name);
               QueryName(map, name);
               break;
          case 3:
               Print(map);
               break;
          case 4:
               return;
          default:
               system("cls");
               system("title [!] Wrong Data! :D");
               printf("[!] 您输入的信息有误！请重新输入！\n");
               system("pause");
               break;
     }
}

int Dijkstra(Graph *map, int start)
{
	int visited[MAXSIZE]={0};
	int min,i,j,k;
	int s=0;
	for(i=0;i<map->n;i++)//记录起点所有邻接结点权值 
	{
		shortlength[i]=map->edges[start][i];
		visited[i]=0;//初始化为未访问 
		djsave[i]=-1;
	}
	shortlength[start]=0;
	visited[start]=1;//开始结点标记已访问 
	for(i=0;i<map->n;i++)
	{
		k=-1;
		min=FINITY;
		for(j=0;j<map->n;j++)//找出最短路径 
		{
			if(visited[j]==0 && shortlength[j]<min)//j结点未被访问，起点到j结点的权值小于最小值 
			{
				min=shortlength[j];
				k=j;	
			}
		}
		visited[k]=1;
		
		for(j=0;j<map->n;j++)
		{
			if(visited[j]==0 && map->edges[k][j]+shortlength[k]<shortlength[j])//修正最短路径 
			{
				shortlength[j]=map->edges[k][j]+shortlength[k];
				djsave[j]=k;
			}
		} 
	}
}

void Short(Graph *map)//求一个到所有的最短路径
{
	int start;
	int i,j;
	int flag=0;
	int x=0;
	int stack[MAXSIZE]={-1};
	int s=0;
	printf("请输入起始景点编号：");
	scanf("%d",&start);
	for(i=0;i<map->n;i++)
	if(map->data[i].item==start)
	{
		flag=1;
		x=i;
	}
	if(flag==0)
	{
		printf("输入的编号不存在！"); 
		getchar();
		getchar();
		system("cls");
	}
	Dijkstra(map, x);
	printf("%s到其他各景点的最短距离如下：\n",map->data[x].name);
	for(i=0;i<map->n;i++)
	{
		if(i!=x)
		{
			s=0;
			stack[0]=djsave[i]; //终点的前一个结点下标值放入数组 
			printf("%s->",map->data[x].name);
			for(j=1;j<map->n;j++)
			{
			stack[j]=djsave[stack[j-1]];//每次存储上一个结点的前一个结点下标 
			}
			for(j=0;j<map->n;j++)
			{
			if(stack[j]==-1) 
			break;
			s++;//计算共有多少结点（不包括开始和结束）
			}
			for(j=s-1;j>=0;j--)
			printf("%s->",map->data[stack[j]].name);
			printf("%s\n",map->data[i].name);
			printf("距离为：%dm\n",shortlength[i]);
		}		
	}
     return;
}


void Min(Graph *map) //两点最短路径
{
     int a,b,x,y;
     x = y = FINITY;
     printf("请输入起始景点编号和终点景点编号，空格隔开：\n");
     scanf("%d %d",&a,&b);
     // 判断两个点存不存在
     for (int i = 0; i < map->n; i++){
          if(map->data[i].item == a)
               x = i;
          if(map->data[i].item == b)
               y = i;
    }
     if(x==FINITY || y==FINITY){
          printf("输入的景点编号不存在");
          getchar();
          getchar();
          return;
     }
     Dijkstra(map, x);
     int stack[MAXSIZE]={-1};
	printf("从%s到%s的最短路径是：\n",map->data[x].name,map->data[y].name);
	stack[0]=djsave[y]; //终点的前一个结点下标值放入数组 
	printf("%s->",map->data[x].name);
     int i, s;
	for(i=1;i<map->n;i++)
	{
		stack[i]=djsave[stack[i-1]];//每次存储上一个结点的前一个结点下标 
	}
	for(i=0;i<map->n;i++)
	{
		if(stack[i]==-1) 
		break;
		s++;//计算共有多少结点（不包括开始和结束） 
	}
	for(i=s-2;i>=0;i--)
	printf("%s->",map->data[stack[i]].name);
	printf("%s\n",map->data[y].name);
	printf("距离为：%dm",shortlength[y]);
}



void Menu(){
     system("title XXX大学导航系统");
     printf("\n\n");
     printf("               欢迎来到XXX大学导航系统\n\n");
     printf("            1.景点信息查询\n");
     printf("            2.新增景点信息\n");
     printf("            3.修改景点和道路信息\n");
     printf("            4.删除景点和道路信息\n");
     printf("            5.任意两个景点的路径查询\n");
     printf("            6.求一个景点到所有景点的最短路径\n");
     printf("            7.求任意的两个景点之间的最短路径\n");
     printf("            8.退出系统\n\n");
}

int main(){
     int command, route = 0;
     Graph map;
     int flag = FINITY;
     Create(&map);

     while (1)
     {
          system("cls");
          Menu();
          printf("> ");
          scanf("%d", &command);
          switch (command)
          {
               case 1:        // 景点信息查询
                    Query(&map);
                    break;
               case 2:        // 新增景点信息
                    Add(&map);
                    break;
               case 3:        // 修改景点和道路信息
                    Modify(&map);
                    break;
               case 4:        // 删除景点和道路信息
                    Delete(&map);
                    break;
               case 5:        // 任意两个景点的路径查询
                    Both(&map);
                    break;
               case 6:        // 求一个景点到所有景点的最短路径
                    Short(&map);
                    system("pause");
                    break;
               case 7:        // 求任意的两个景点之间的最短路径
                    Min(&map);
                    system("pause");
                    break;
               case 8:        // 退出系统
                    system("title Goodbye, And never comes back :D");
                    printf("Byebye :)\n");
                    system("pause");
                    return;
                    break;
               default:
                    system("cls");
                    system("title [!] Wrong Data! :D");
                    printf("[!] 您输入的信息有误！请重新输入！\n");
                    system("pause");
                    break;
          }
     }
}