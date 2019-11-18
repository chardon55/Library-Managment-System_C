//////////////////////////////////////////////////////////////////////
/*																	*/
/*							 图书馆管理系统							*/
/*																	*/
/*												<> with ♥ by 董岳 (dy55)	*/
/*												  学号：2018204788	*/
/*																	*/
/*											   Qingdao University	*/
/*																	*/
/*			 	Created Date:	2019-4-8							*/
/*		  Main Coding Period:	2019-4-8  ~  2019-4-29				*/
/*			    Debug Period:	2019-4-29 ~	 2019-5-9				*/
/*					Recoding:	2019-5-2  ~	 2019-5-9				*/
/*		   Last Modification:	2019-5-20							*/	
/*																	*/
//////////////////////////////////////////////////////////////////////
//																	//
//////////////////////////////////////////////////////////////////////
//	Modified Date(Main.c):											//
//		2019-4-8, 4-10, 4-11, 4-15, 4-17, 4-21, 4-22, 4-23,	4-26,	//
//		 4-28, 4-29, 5-1, 5-4, 5-5, 5-8, 5-9, 5-11, 5-13, 5-15,		//
//		 5-17, 5-18, 5-20											//
//////////////////////////////////////////////////////////////////////

/*
 * Copyright (c) dy55
 *
 * 请勿将此项目用于个人课程项目
 * Please do not use this project for your individual / team course project.
 *
 *
 *
 *
 *
 *
 */

//库头文件引用声明
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<stdarg.h>						//标准参数头文件
#include<string.h>
#include<time.h>


#define MAIN_VERSION "1905.47"			//主系统版本


#define MODE_SWITCH	0			//	模式选择器

#if MODE_SWITCH

#define DEBUG_MODE	void		//	调试模式

#else

#define NORMAL	void			//	正常模式

#endif

#define LOCK 1

#define Framework_WIDTH 44		//	界面宽度

//自定义头文件引用声明
#include"Misc.h"				//	杂项

#include"Framework.h"			//	界面头文件

#include"Structure.h"			//	结构定义头文件
#include"Structure_Print.h"		//	结构输出头文件
#if 0	//	Abandoned ->
#include"Node_Structure.h"		//	节点结构头文件
#endif	//	<- Abandoned

//源引用声明
#include"BookManageModule.c"	//	图书管理模块源
#include"ReaderManageModule.c"	//	读者管理模块源
#include"Borrow_Return.c"		//	借书/还书模块源
#include"Advanced_Settings.c"	//	高级设置源


//程序初始化函数	(Since 1.4.0) (Framework: v2.1)
void file_Init() {
	General_Framework_Print_Monitor("正在初始化...", 3, "图书日志 ->", "读者日志 ->", "操作日志 ->");
	FILE* fp;
	long temp;
	fp = fopen("Books_Manage_Log.dat", "rb");
	if (!fp) {
		fp = fopen("Books_Manage_Log.dat", "wb");
		fclose(fp);
	}
	else {
		for (rewind(fp), temp = 0; fgetc(fp) != EOF; temp++);
		Books_Type_Stock = temp / sizeof(BOOK_INFO);
		fclose(fp);
	}
	General_Framework_Print_Monitor("正在初始化...", 3, "图书日志 -> √", "读者日志 ->", "操作日志 ->");
	fp = fopen("Readers_Manage_Log.dat", "rb");
	if (!fp) {
		fp = fopen("Readers_Manage_Log.dat", "wb");
		fclose(fp);
	}
	else {
		for (rewind(fp), temp = 0; fgetc(fp) != EOF; temp++);
		Readers_Sum = temp / sizeof(READER_INFO);
		fclose(fp);
	}
	General_Framework_Print_Monitor("正在初始化...", 3, "图书日志 -> √", "读者日志 -> √", "操作日志 ->");
	if (!(fp = fopen("Operation_Manage_Log.dat", "rb"))) {
		fp = fopen("Operation_Manage_Log.dat", "wb");
	}
	fclose(fp);
	General_Framework_Print_Monitor("初始化完成", 3, "图书日志 -> √", "读者日志 -> √", "操作日志 -> √");
	Countdown_Continue(2, "系统将于", "启动");
}

#if !LOCK
//主函数
int main() {
	file_Init();
	int _choice;
	while (1) {

		//主界面打印
		Framework_Print_Monitor("欢迎进入图书馆管理系统", "退出", 4, "图书管理", "读者管理", "借书/还书", "高级设置");
		
		CHOOSE_PLEASE(&_choice);
		switch (_choice)
		{
		case 1:					//选择图书管理
			Books_Manage_Choice();
			fcloseall();
			break;
		case 2:					//选择读者管理
			Readers_Manage_Choice();
			fcloseall();
			break;
		case 3:					//选择借书/还书
			Borrow_Return_main();
			fcloseall();
			break;
		case 4:
			Advanced_Settings();
			break;
		case 0:					//选择退出
			if (_Confirm_Frame("确认是否要退出系统")) {
				fcloseall();
				exit(0);			//正常退出
				quick_exit(0);		//正常退出失败时的特殊退出
			}
			break;
		default:
			_CAUTION_FRAME("无效选择");
			Countdown_Continue(3, "", "继续");
			break;
		}
	}
}
#endif

/*
 *
 *
 *
 *
 * Copyright (c) dy55
 *
 * 请勿将此项目用于个人课程项目
 * Please do not use this project for your individual / team course project.
 */