#pragma once
//					结构定义头文件						//
//								<> with ♥ by 董岳 (dy55)	//
//	2019-4-10, 4-11, 4-24, 4-27, 5-2, 5-5, 5-9, 5-15


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


#define NAME_MAXLEN 50					//书名最大长度
#define PEOPLE_NAME_MAXLEN 30			//姓名最大长度
#define PUBLISHER_MAXLEN 30				//出版商名称最大长度

#define IDENTITY_MAXLEN 7				//身份名称最大长度

#define MAX_BORROW_NUMBER_1 10;			//1类读者最大借书量
#define MAX_BORROW_NUMBER_2 20;			//2类读者最大借书量
#define MAX_BORROW_NUMBER_3 30;			//3类读者最大借书量

#define TIME_STR_LEN 30					//时间长度

long long Books_Type_Stock = 0;				//书类总数
long long Readers_Sum = 0;					//读者总数

#if !LOCK
//书类
typedef struct BOOK{
	char Name[NAME_MAXLEN];
	long long Code;
	char Author[PEOPLE_NAME_MAXLEN];
	char Publisher[PUBLISHER_MAXLEN];
	long Inventory;
} BOOK_INFO;

//读者
typedef struct READER {
	char Name[PEOPLE_NAME_MAXLEN];
	long long Code;
	char Gender;
	int ID_Code;
	int Remain_Borrow;
	int Max_Borrow;
} READER_INFO;

#endif
#define _BORROW 1
#define _RETURN 0

//日志
typedef struct LOG_ {
	char _DateTime_Str[TIME_STR_LEN];
	long long Reader_code;
	long long Book_code;
	short Operation;
} LOG_INFO;

//身份分类至代码
int Id_Classify(const char Id_Name[]) {
	if (!strcmp(Id_Name, "本科生"))
		return 1;
	if (!strcmp(Id_Name, "研究生"))
		return 2;
	if (!strcmp(Id_Name, "博士生"))
		return 3;
	if (!strcmp(Id_Name, "教师"))
		return 4;
	if (!strcmp(Id_Name, "教授"))
		return 5;
	printf("\n\t未知身份\n");
	return 0;
}
//身份分类至名称
char* Id_Classify_fromCode(const int IdCode) {
	switch(IdCode){
	default:	break;
	case 1:		return "本科生";
	case 2:		return "研究生";
	case 3:		return "博士生";
	case 4:		return "教师";
	case 5:		return "教授";
	}
	printf("\n\t未知身份\n");
	return NULL;
}

//按身份确定最大借书量
int Max_Borrow_By_Identity(const char Identity_Name[]) {
	if (!strcmp(Identity_Name, "本科生"))
		return MAX_BORROW_NUMBER_1;
	if (!strcmp(Identity_Name, "研究生"))
		return MAX_BORROW_NUMBER_2;
	if (!strcmp(Identity_Name, "博士生") || !strcmp(Identity_Name, "教师") || !strcmp(Identity_Name, "教授"))
		return MAX_BORROW_NUMBER_3;
	printf("\n\t未知身份\n");
	return 0;
}

//性别转换（字符 -> 文字）
char* Char_Gender_to_String(char gen_char) {
	switch (gen_char) {
	case 'M':
	case 'm':
		return "男";
	case 'F':
	case 'f':
		return "女";
	default:
		return "?";
	}
}
//性别转换（文字 -> 字符）
char String_Gender_to_Char(char gen_string[]) {
	if (!strcmp(gen_string, "男") || !strcmp(gen_string, "M") || !strcmp(gen_string, "m"))
		return 'M';
	if (!strcmp(gen_string, "女") || !strcmp(gen_string, "F") || !strcmp(gen_string, "f"))
		return 'F';
	printf("错误性别");
	return 0;
}

char* Custom_Datetime_Generator(time_t _target);
//日志生成
int Log_generate(BOOK_INFO Book, READER_INFO Reader, int _operation) {
	LOG_INFO temp_log;
	if (_operation == 1 || _operation == 0)
		temp_log.Operation = _operation;
	else
		return 0;
	FILE* fp = fopen("Operation_Manage_Log.dat", "ab+");
	if (fp == NULL) {
		_ERROR("File opening failed\n");
		return 0;
	}
	time_t Cur_time;
	time(&Cur_time);
	strcpy(temp_log._DateTime_Str, Custom_Datetime_Generator(Cur_time));
	temp_log.Book_code = Book.Code;
	temp_log.Reader_code = Reader.Code;
	Fill_mode(fp);
	fwrite(&temp_log, sizeof(LOG_INFO), 1, fp);
	if (fclose(fp)) {
		printf("File close failed\n");
		return 0;
	}
	return 1;
}

#define _BOOK 1
#define _READER 2

//操作类型
char* Operation_Type_Convert(int Op) {
	if (Op == _BORROW)
		return "借书";
	if (Op == _RETURN)
		return "还书";
}

//图书定位函数 (Since 1905.10)
FILE* BOOK_Positioning(BOOK_INFO Info) {
	FILE* fp = fopen("Books_Manage_Log.dat", "ab+"), * temp_fp;
	BOOK_INFO temp;
	if (fp == NULL)
		return NULL;
	rewind(fp);
	while (!if_empty(fp))
		if(!feof(fp)){
			fread(&temp, sizeof(BOOK_INFO), 1, fp);
			fseek(fp, -sizeof(BOOK_INFO), SEEK_CUR);
			if (temp.Code == Info.Code)
				return fp;
		}
	return NULL;
}

//自定义日期时间字符串生成
char* Custom_Datetime_Generator(time_t _target) {
	struct tm time_bunch;
	localtime_s(&time_bunch, &_target);
	char t_datetime[30];
	strcpy(t_datetime, "");
	strcat(t_datetime, Num_to_String(time_bunch.tm_year + 1900, ""));
	strcat(t_datetime, "-");
	strcat(t_datetime, Num_to_String(time_bunch.tm_mon + 1, ""));
	strcat(t_datetime, "-");
	strcat(t_datetime, Num_to_String(time_bunch.tm_mday, ""));
	strcat(t_datetime, "  ");
	strcat(t_datetime, Num_to_String(time_bunch.tm_hour, ""));
	strcat(t_datetime, ":");
	strcat(t_datetime, Num_to_String(time_bunch.tm_min, "02"));
	strcat(t_datetime, ":");
	strcat(t_datetime, Num_to_String(time_bunch.tm_sec, "02"));
	return t_datetime;
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