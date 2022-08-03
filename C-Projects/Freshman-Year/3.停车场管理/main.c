#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include<stdlib.h>
#define MAX_CARCOUNT 2
#define MONEY 2

void mainMenu(); //主菜单
void in_Park();   //进场车 
void out_Park();   //出场车 
void display_Park();   //显示停车场里的车 
void remove();   //出场之后删除停车场数组里的车牌 
bool isExits(); //查找车辆是否已经在停车场内 
int findTime();  //查找入场时间 
int findTime(deque<pair<string, int>> & ve, string number);
bool isExits(deque<pair<string, int>> & ve, string number);
void remove(deque<pair<string, int>> & ve, string number);

deque<pair<string, int>> park;
deque<pair<string, int>> access;


/* template <typename V1,V2>
struct pair
{
	V1 first; // string number
	V2 second; // int inTime
} */

int main(void)
{
	mainMenu();
	return 0;
}

void mainMenu() //主菜单
{
	char userInput;

	system("cls");
	system("title 停车场管理系统");
	printf("\n\n            ???????????  停车场管理程序  ??????????? \n");
	printf("?????????????????????????????????????????????????????????????????\n");
	printf(" $  $                                                        $  $\n");
	printf(" $  $        a --- 汽车进车场       b --- 汽车出车场         $  $\n");
	printf(" $  $                                                        $  $\n");
	printf(" $  $        c --- 显示车辆信息     d --- 退出程序           $  $\n");
	printf(" $  $                                                        $  $\n");
	printf(" $  $           停车场剩余%d车位，每小时收费%d刀               $  $\n", MAX_CARCOUNT-park.size(), MONEY);
	printf("?????????????????????????????????????????????????????????????????\n");
	printf(" 请选择 :(a,b,c,d): ");
	scanf("%c", &userInput);
	switch (userInput)
	{
		case 'a': in_Park();		 break;
		case 'b': out_Park();	 break;
		case 'c': display_Park(); break;
		case 'd': exit(-1);		 break;
		default:
			printf("输入错误，请重新输入\n");
			system("pause");
			getchar();
			mainMenu();
			break;
	}
}

void in_Park()
{
	string number;
	int inTime;
	system("title 汽车进场");
	/*
		能不能放入停车场，分为两种情况
			1. 有一样的车牌（停车场内不能有，通道里不能有）
			2. 没有一样的车牌，放入停车场或者通道
	*/
	printf("车牌为："); cin >> number; // string C++
	
	
	if (isExits(park, number) || isExits(access, number)) { // 停车场里有这辆车
		printf("车辆已在场内!\n");
		system("pause");
		getchar();
		mainMenu();
	}else{
		/*
			停车场情况分为两种：
				1. 停车场没满，直接放到停车场
				2. 停车场满，放到通道
		*/
		printf("车辆进场的时刻：");  cin >> inTime;
		if (park.size() < MAX_CARCOUNT) {  // 1. 停车场没满，直接放到停车场
			park.push_back(make_pair(number, inTime));
			cout << "该车已停入停车场,在" << park.size() << "号位置\n";
		}else {
			access.push_back(make_pair(number, inTime)); // 2. 停车场满，放到通道
			cout << "停车场已停满，已放置通道,在" << access.size() << "号位置\n";
		}
	}
	system("pause");
	getchar();
	mainMenu();
}

void out_Park()
{
	/*
		先判断是否在场内：
			1. 是，看在停车场内(直接删除)还是便道上(不删除)，
			2. 否，输出没在停车场内
	*/
	string number;
	int outTime;
	system("title 汽车出场");
	cout << "车牌为：";	 cin >> number;
	if (isExits(park, number) || isExits(access, number)) { // 在停车场或者通道里
		if (isExits(park, number)) {
			cout << "出停车场时刻为：";	 cin >> outTime; // 停车场内才要交钱
			int inTime = findTime(park, number);
			printf("                          收              据\n");
			printf("================================ 车牌号: %s==============================\n", number.c_str());
			printf("=========================================================================\n");
			printf("|进车场时刻         | 出车场时刻         | 停留时间         | 应付（刀）|\n");
			printf("| %d                | %d                 | %d               | %d刀\n",
				inTime, outTime, outTime - inTime, (outTime - inTime) * MONEY);
			printf("=========================================================================\n");
			remove(park, number);

			// 停车场现在有空缺了，把通道的车移到停车场
			if (!access.empty()) {
				park.push_back(access.front()); // 把通道的第一个压入 停车场
				access.pop_front();				// 把通道第一个删除
			}
		}		
		if (isExits(park, number))	cout << "该车在通道排队内！\n";
	}else {
		cout << "该车未在停车场内或者便道内！\n"; // 都不在
	}
	system("pause");
	getchar();
	mainMenu();
}

void display_Park()

{
	system("cls");
	system("title 停车场车辆车牌显示");
	printf("\n停车场内有%d辆车，分别是：\n",park.size());
	for (int i = 0; i < park.size(); i++) {
		printf("第%d辆车是%s,入场时间为%d\n", i + 1, park[i].first.c_str(), park[i].second);
	}

	printf("\n\n");
	
	printf("通道内有%d辆车，分别是：\n",access.size());
	for (int i = 0; i < access.size(); i++) {
		printf("第%d辆车是%s,入场时间为%d\n", i + 1, access[i].first.c_str(), access[i].second);
	}
	printf("\n\n");

	system("pause");
	
	getchar();
	mainMenu();
}

bool isExits(deque<pair<string, int>> & ve, string number)    //判断这个车牌有没有在车场内 
{
	for (int i = 0; i < ve.size(); i++) {
		if (ve[i].first == number)	
			return true;
	}
	return false;
}

void remove(deque<pair<string, int>> & ve, string number)  //出场之后删除停车场数组里的车牌
{
	for (auto it = ve.begin(); it != ve.end(); it++) {
		if (it->first == number) {
			ve.erase(it);
			break;
		}
	}
}

int findTime(deque<pair<string, int>> & ve, string number)    //找到进场时间 
{
	for (int i = 0; i < ve.size(); i++) {
		if (ve[i].first == number)	return ve[i].second;
	}
}
