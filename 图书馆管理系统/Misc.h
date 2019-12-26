#pragma once
//						杂项							//
//								<> with ♥ by 董岳 (dy55)	//
//	2019-4-10, 4-14, 4-17, 4-21, 4-26, 4-27, 4-28, 4-29,
//		5-1, 5-5, 5-9, 5-15, 5-16, 5-18

/*	Former name of the header: Response.h	*/


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


	//版本打印
void Edition_print() {
	printf("Version: ");
	puts(MAIN_VERSION);
}
//格式化版本打印
void Edition_print_formatted() {
	int Space = Framework_WIDTH - strlen("Version: ") - strlen(MAIN_VERSION);
	if (Space > 0)
		while (Space--)
			putchar(' ');
	Edition_print();

#ifdef DEBUG_MODE
	Space = Framework_WIDTH - strlen("Debug Mode");
	while (Space--)
		putchar(' ');
	printf("Debug Mode");
#endif // DEBUG_MODE

}

//“请选择”
void CHOOSE_PLEASE(int* target) {
	printf("\n\t请选择：");
	scanf_s("%d", target);
	getchar();
}

#ifdef DEBUG_MODE
#define _ERROR(DETAIL_STRING) printf("Error Number: %d\n",errno);perror("Lastest error while debugging");printf("\n%s\n",DETAIL_STRING)//错误报告（调试模式）
#else
#define _ERROR(DETAIL_STRING) printf("错误 ");printf(DETAIL_STRING);putchar('\n')	//错误报告
#endif // DEBUG_MODE_SWITCH

#if !LOCK
//倒计时后继续函数
void Countdown_Continue(short _Countdown_from_sec, const char Prompt_String_Head[], const char Prompt_String_Gear[]) {
	putchar('\n');
	while (_Countdown_from_sec + 1) {
		printf("\r\t%s %d 秒后%s", Prompt_String_Head, _Countdown_from_sec--, Prompt_String_Gear);
		Sleep(1000);
	}
	putchar('\n');
}
#endif

#define _Operation_Complete _CAUTION_FRAME("操作完成");Countdown_Continue(2,"系统将于","继续")	//操作完成
#define _Operation_Canceled	_CAUTION_FRAME("操作已取消");Countdown_Continue(2,"系统将于","继续")//操作取消

//确认函数
short _Confirm(const char prompt[]) {
	while (1) {
		system("cls");
		printf("\n\n\t确认");
		printf("%s？\n\t(Y - 是 / N - 否)\n\n\t\t -> ", prompt);
		switch (getchar())
		{
		case '\n':
			break;
		case 'Y':
		case 'y':
			getchar();
			return 1;
		case 'N':
		case 'n':
			getchar();
			return 0;
		default:
			getchar();
			_ERROR("无效选择");
			Countdown_Continue(3, "请在", "重新选择");
			break;
		}
	}
}
//自定义确认函数
short _Confirm_Custom(const char Full_prompt[]) {
	while (1) {
		system("cls");
		printf("%s？\n\t(Y - 是 / N - 否)\n\n\t\t -> ", Full_prompt);
		switch (getchar())
		{
		case '\n':
			break;
		case 'Y':
		case 'y':
			getchar();
			return 1;
		case 'N':
		case 'n':
			getchar();
			return 0;
		default:
			getchar();
			_ERROR("无效选择");
			Countdown_Continue(3, "请在", "重新选择");
			break;
		}
	}
}

#define Press_Continue printf("Press \"Enter\" to continue\n");while(getchar()!='\n') //按回车继续

//无换行puts
void no_feed_puts(const char Buffer[]) {
	int str_count = strlen(Buffer);
	for (int i = 0; i < str_count; i++)
		putchar(Buffer[i]);
}

//数组求和（int）
int grp_Sum(const int Num_Grp[], const int number) {
	int sum = 0;
	for (int i = 0; i < number; i++)
		sum += Num_Grp[i];
	return sum;
}

//是否为结束符
int if_empty(FILE* fp) {
	if (fgetc(fp) == EOF)
		return 1;
	return 0;
}

#define EMPTY_OPERATION(target, OPERATION) if(if_empty(target)) OPERATION	//文件为空时操作
#define nEMPTY_OPERATION(target, OPERATION) if(!if_empty(target)) OPERATION	//文件不为空时操作

#define Fill_mode(fp) fputc('\0',fp)										//占位填充


//数字转字符串
char* Num_to_String(int Target, const char* _format) {
	char str_t1[30] = "", str_t2[30] = "", temp_c[2];
	int D_count = 0;
	temp_c[1] = 0;
	if (!Target) {
		strcpy(str_t2, "0");
		D_count = 1;
	}
	else
		while (Target) {
			temp_c[0] = Target % 10 + '0';
			strcat(str_t1, temp_c);
			strcat(str_t1, str_t2);
			strcpy(str_t2, str_t1);
			strcpy(str_t1, "");
			Target /= 10;
			D_count++;
		}
	if (!_format || !strcmp(_format, ""));
	else if (_format[0] >= '1' && _format[0] <= '9')
		for (int i = 0; i < _format[1] - '0' - D_count; i++)
			strcat(str_t1, " ");
	else if (_format[0] == '0' && _format[1] >= '1' && _format[1] <= '9')
		for (int i = 0; i < _format[1] - '0' - D_count; i++)
			strcat(str_t1, "0");
	else if (_format[0] == '-' && _format[1] >= '1' && _format[1] <= '9')
		for (int i = 0; i < _format[1] - '0' - D_count; i++)
			strcat(str_t2, " ");
	strcat(str_t1, str_t2);

	return str_t1;
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