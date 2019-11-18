#pragma once
//////////////////////////////////////////////////////////////////
//																//
//							图书管理模块							//
//											<> with ♥ by 董岳 (dy55)	//
//////////////////////////////////////////////////////////////////
// 2019-4-10, 4-11, 4-12, 4-14, 4-15, 4-20, 4-21, 4-23, 5-1, 
//		5-2, 5-3, 5-4, 5-5, 5-7, 5-8, 5-9, 5-13, 5-15, 5-16, 5-17
//		5-18


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

extern long long Books_Type_Stock;

#if 0	//Abandoned
extern int Book_count;
#endif

int Book_Query_count;


#define By_CODE 1			//按编号查询
#define By_NAME 2			//按名查询

//图书查询工具函数 (Totally Modified from 1905.10)
int Books_Query_Tool(const BOOK_INFO target, const int _Command) {
	FILE* fp = fopen("Books_Manage_Log.dat", "rb");
	BOOK_INFO temp_Book[80];
	int Num = 0, flag = 0;
	if (!fp)
		return -1;
	fseek(fp, -1L, SEEK_CUR);
	switch (_Command) {
	case By_CODE:
		while (!if_empty(fp))
			if (!feof(fp)) {
				fread(&(temp_Book[0]), sizeof(BOOK_INFO), 1, fp);
				if (temp_Book[0].Code == target.Code) {
					Num++;
					return 1;
				}
			}
		break;
	case By_NAME:
		for (Num = 0; !if_empty(fp);)
			if (!feof(fp)) {
				fread(&(temp_Book[Num]), sizeof(BOOK_INFO), 1, fp);
				if (Num > 100)
					break;
				if (!strcmp(temp_Book[Num].Name, target.Name)) {
					Num++;
					flag = 1;
				}
			}
		break;
	default:
		return 0;
	}
	system("cls");
	fcloseall();
	Structure_Print_Navigator(NULL, temp_Book, NULL, Num, BOOK_PRINT, PURE_Print);
	return flag;
}

//获取编号
long long get_Code() {
	BOOK_INFO temp;
	temp.Code = 1;
	while (Books_Query_Tool(temp, By_CODE))
		temp.Code++;
	return (long long)temp.Code;
}


//图书添加工具函数
void Book_Add_Tool(BOOK_INFO* Info, int Number) {
	FILE* fp = fopen("Books_Manage_Log.dat", "ab+");
	if (!fp) {
		_ERROR("File opening failed");
		Countdown_Continue(3, "", "返回");
		return;
	}
	if (!Info) {
		printf("No info\n");
		Countdown_Continue(2, "系统将于", "退出");
		return;
	}
	Fill_mode(fp);
	fwrite(Info, sizeof(BOOK_INFO), Number, fp);
	Books_Type_Stock += Number;
	if (fclose(fp)) {
		_ERROR("File close failed");
		Countdown_Continue(3, "", "直接退出");
		exit(0);
	}
}

//图书添加总函数
void Book_Add_Main() {
	int id_state;
	BOOK_INFO Book_Info;
	while (1) {
		General_Framework_Print_Monitor("图书添加向导", 3, "请输入图书信息", "", "输入书名:");
		printf("\n\n\t");
		gets_s(Book_Info.Name, NAME_MAXLEN);
		General_Framework_Print_Monitor("图书添加向导", 3, "请输入图书信息", "", "输入作者名:");
		printf("\n\n\t");
		gets_s(Book_Info.Author, PEOPLE_NAME_MAXLEN);
		General_Framework_Print_Monitor("图书添加向导", 3, "请输入图书信息", "", "输入出版商名:");
		printf("\n\n\t");
		gets_s(Book_Info.Publisher, PUBLISHER_MAXLEN);
		General_Framework_Print_Monitor("图书添加向导", 3, "请输入图书信息", "", "输入库存数:");
		printf("\n\n\t");
		scanf("%ld", &(Book_Info.Inventory));
		getchar();
		Book_Info.Code = get_Code();
		if (_Confirm_Frame("是否执行操作"))
			Book_Add_Tool(&Book_Info, 1);
		else {
			_Operation_Canceled;
			continue;
		}
		system("cls");
		Structure_Print_Navigator(NULL, &Book_Info, NULL, 1, BOOK_PRINT, PURE_Print);
		putchar('\n');
		Press_Continue;
		if (!_Confirm_Frame("是否继续添加"))
			return;
	}
}

//图书查询导航函数 (Totally Modified from: 1905.11)
void Books_Query_Navigator(const int _Command) {
	BOOK_INFO _info;
	FILE* fp;
	system("cls");
	switch (_Command)
	{
	default:
		break;
	case By_CODE:
		General_Framework_Print_Monitor("图书查询向导", 3, "请输入图书信息", "", "输入编号:");
		printf("\n\n\t");
		scanf("%lld", &(_info.Code));
		getchar();
		if (!Books_Query_Tool(_info, By_CODE)) {
			_CAUTION_FRAME("No info");
			Countdown_Continue(3, "系统将于", "返回");
			return;
		}
		fp = BOOK_Positioning(_info);
		fread(&_info, sizeof(BOOK_INFO), 1, fp);
		system("cls");
		Structure_Print_Navigator(NULL, &_info, NULL, 1, BOOK_PRINT, PURE_Print);
		Press_Continue;
		break;
	case By_NAME:
		General_Framework_Print_Monitor("图书查询向导", 3, "请输入图书信息", "", "输入书名:");
		printf("\n\n\t");
		gets_s(_info.Name, NAME_MAXLEN);
		Books_Query_Tool(_info, By_NAME);
		Press_Continue;
		break;
	}
}

//图书删除工具函数
void Books_Delete_Tool(BOOK_INFO target) {
	BOOK_INFO t1;
	FILE* fp = fopen("Books_Manage_Log.dat", "ab+");
	if (!Books_Type_Stock) {
		_CAUTION_FRAME("没有图书");
		return;
	}
	FILE* ptr = fopen(".temp", "wb");
	rewind(fp);
	while (!if_empty(fp))
		if (!feof(fp)) {
			fread(&t1, sizeof(BOOK_INFO), 1, fp);
			if (t1.Code != target.Code) {
				Fill_mode(ptr);
				fwrite(&t1, sizeof(BOOK_INFO), 1, ptr);
			}
		}
	fcloseall();
	remove("Books_Manage_Log.dat");
	rename(".temp", "Books_Manage_Log.dat");
	Books_Type_Stock--;
}

//图书删除函数 (Extinsively Modified at 1905.11)
void Books_Delete_Main() {
	int i;
	int _choice;
	BOOK_INFO Target;
	while (1) {
		General_Framework_Print_Monitor("图书删除向导", 1, "输入编号:");
		printf("\n\n\t");
		scanf("%lld", &(Target.Code));
		getchar();
		if (!Books_Query_Tool(Target,By_CODE)) {
			_CAUTION_FRAME("未找到");
			putchar('\n');
			Countdown_Continue(3, "请于", "重新输入");
			continue;
		}
		if (_Confirm_Frame("确认删除")) {
			Books_Delete_Tool(Target);
			_Operation_Complete;
		}
		fcloseall();
		return;
	}
}

#define Modify_CODE 1					//修改编号
#define Modify_NAME 2					//修改名
#define Modify_AUTHOR 3					//修改作者
#define Modify_PUBLISHER 4				//修改出版商
#define Modify_INVENTORY 5				//修改库存


BOOK_INFO History_oldB, History_newB;		//历史记录

//图书信息修改工具 (Totally Modified from 1905.11,14)
void Books_Modification_Tool(BOOK_INFO Old, BOOK_INFO New) {
	FILE* fp, * tfp;
	BOOK_INFO temp;
	fp = BOOK_Positioning(Old);
	fseek(fp, -1L, SEEK_CUR);
	if (!fp) {
		_CAUTION_FRAME("No info");
		Press_Continue;
		return;
	}
	
	tfp = fopen(".temp", "wb+");
	rewind(tfp);
	rewind(fp);
	if (!fp) {
		_ERROR("Temp file creating failed\n");
		return;
	}
	while(!if_empty(fp))
		if (!feof(fp)) {
			Fill_mode(tfp);
			fread(&temp, sizeof(BOOK_INFO), 1, fp);
			if (temp.Code == Old.Code)
				fwrite(&New, sizeof(BOOK_INFO), 1, tfp);
			else
				fwrite(&temp, sizeof(BOOK_INFO), 1, tfp);
		}
	fcloseall();
	remove("Books_Manage_Log.dat");
	rename(".temp", "Books_Manage_Log.dat");
	History_newB = New;
}
//图书修改向导函数 (Extinsively Modified from 1905.11)
void Books_Modification_Wizard() {
	int choice;
	Framework_Print_Monitor("图书信息修改向导", "返回", 5, "修改编号", "修改书名", "修改作者", "修改出版商", "修改库存量");
	CHOOSE_PLEASE(&choice);
	if (!choice)
		return;
	BOOK_INFO target, New;
	FILE* fp;
	General_Framework_Print_Monitor("图书信息修改向导", 1, "输入待修改图书编号:");
	printf("\n\n\t");
	scanf("%lld", &target.Code);
	getchar();
	if (!Books_Query_Tool(target,By_CODE)) {
		_CAUTION_FRAME("未找到此图书");
		Countdown_Continue(2, "系统将于", "返回");
		return;
	}
	fp = BOOK_Positioning(target);
	if (!fp) {
		_CAUTION_FRAME("No info");
		Press_Continue;
		return;
	}
	fread(&New, sizeof(BOOK_INFO), 1, fp);
	History_oldB = target = New;
	rewind(fp);
	switch (choice) {
	default:
		_CAUTION_FRAME("无效输入");
		Countdown_Continue(2, "系统将于", "返回");
		return;
	case Modify_CODE:
		while (1) {
			General_Framework_Print_Monitor("图书修改向导", 1, "输入新编号:");
			printf("\n\n\t");
			scanf("%lld", &New.Code);
			getchar();
			if (Books_Query_Tool(New, By_CODE)) {
				_CAUTION_FRAME("该编号已存在");
				Countdown_Continue(2, "请于", "重新输入");
				continue;
			}
			break;
		}
		break;
	case Modify_NAME:
		General_Framework_Print_Monitor("图书修改向导", 1, "输入新名称:");
		printf("\n\n\t");
		gets_s(New.Name, NAME_MAXLEN);
		break;
	case Modify_AUTHOR:
		General_Framework_Print_Monitor("图书修改向导", 1, "输入新作者名:");
		printf("\n\n\t");
		gets_s(New.Author, PEOPLE_NAME_MAXLEN);
		break;
	case Modify_PUBLISHER:
		General_Framework_Print_Monitor("图书修改向导", 1, "输入新出版商名:");
		printf("\n\n\t");
		gets_s(New.Publisher, PUBLISHER_MAXLEN);
		break;
	case Modify_INVENTORY:
		General_Framework_Print_Monitor("图书修改向导", 1, "输入新库存数:");
		printf("\n\n\t");
		scanf("%ld", &(New.Inventory));
		getchar();
		break;
	}
	if (_Confirm_Frame("确认执行操作")) {
		Books_Modification_Tool(target, New);
		system("cls");
		printf("\nBefore:\n\n");
		Structure_Print_Navigator(NULL, &History_oldB, NULL, 1, BOOK_PRINT, PURE_Print);
		printf("\nAfter:\n\n");
		Structure_Print_Navigator(NULL, &History_newB, NULL, 1, BOOK_PRINT, PURE_Print);
		putchar('\n');
		Press_Continue;
	}

	return;
}

//图书查询向导函数 (Extensively Modified from 1905.11)
void Books_Query_Wizard() {
	int choice;
	while (1) {
		Framework_Print_Monitor("查询图书信息", "返回", 2, "输入编号", "输入书名");
		CHOOSE_PLEASE(&choice);
		if (!choice)
			return;
		Books_Query_Navigator(choice);
		if (!_Confirm_Frame("是否继续查询"))
			return;
	}
}

//图书管理模块函数
void Books_Manage_Choice() {
	int _choice;
	while(1){
		Framework_Print_Monitor("图书管理模块", "返回", 5, "新增图书信息", "删除图书信息", "修改图书信息", "查询图书信息", "显示所有图书信息");
		CHOOSE_PLEASE(&_choice);
		switch (_choice) {
		case 1:
			Book_Add_Main();
			break;
		case 2:
			if (!Books_Type_Stock) {
				_CAUTION_FRAME("没有图书");
				Countdown_Continue(3,"系统将于","返回");
				break;
			}
			Books_Delete_Main();
			break;
		case 3:
			if (!Books_Type_Stock) {
				_CAUTION_FRAME("没有图书");
				Countdown_Continue(3, "系统将于", "返回");
				break;
			}
			Books_Modification_Wizard();
			break;
		case 4:
			if (!Books_Type_Stock) {
				_CAUTION_FRAME("没有图书");
				Countdown_Continue(3, "系统将于", "返回");
				break;
			}
			Books_Query_Wizard();
			break;
		case 5:
			system("cls");
			Structure_Print_Navigator(NULL, NULL, NULL, 0, BOOK_PRINT, AUTO_All);
			break;
		case 0:
			fcloseall();
			return;
		default:
			_CAUTION_FRAME("无效选择");
			Countdown_Continue(3, "", "继续");
			break;
		}
	}
}


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