#pragma once
//						界面头文件					//
//								<> with ♥ by 董岳 (dy55)	//
//	2019-4-26(Separated from Misc.h), 5-1, 5-4, 5-13


#define PADDING_TOP printf("\n\n")														//顶部补白
#define HORIZONTAL_LINE_TOP printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n")	//上框线
#define HORIZONTAL_LINE_BOTTOM printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n")	//下框线
#define VERTICAL_LINE printf("┃                                          ┃\n")			//左右框线（空白）

#define HEAD_FRONT_SPACE 30
#define CHOICE_FRONT_SPACE 25
#define GENERAL_FRONT_SPACE 30

//题目打印函数
void Head_Print(const char* _Head_Content) {
	int Space = HEAD_FRONT_SPACE - strlen(_Head_Content);
	printf("┃            ");
	no_feed_puts(_Head_Content);
	while (Space--)
		putchar(' ');
	printf("┃\n");
}
//选项打印函数
void Choice_Print(const short _Choice, const char _Content[]) {
	int Space = CHOICE_FRONT_SPACE - strlen(_Content);
	printf("┃            %2d - ", _Choice);
	no_feed_puts(_Content);
	while (Space--)
		putchar(' ');
	printf("┃\n");
}
//内容打印函数 (Since: 1.4.1)
void Content_Print(const char _Content[]) {
	int Space = GENERAL_FRONT_SPACE - strlen(_Content);
	printf("┃            ");
	no_feed_puts(_Content);
	while (Space--)
		putchar(' ');
	printf("┃\n");
}

//界面打印总函数(题目, 退出选项名称, 选项数, 选项名称)
void Framework_Print_Monitor(const char _Head[], const char _Final_Exit[], const int Choice_Number, ...) {
	system("cls");	//清屏
	va_list ap;
	va_start(ap, Choice_Number);
	PADDING_TOP;
	Edition_print_formatted();
	HORIZONTAL_LINE_TOP;
	Head_Print(_Head);
	VERTICAL_LINE;
	for (int Number = 1; Number <= Choice_Number; Number++)
		Choice_Print(Number, va_arg(ap, char*));
	va_end(ap);
	VERTICAL_LINE;
	Choice_Print(0, _Final_Exit);
	VERTICAL_LINE;
	HORIZONTAL_LINE_BOTTOM;
}

//普通界面打印总函数 (Since 1.4.1)
void General_Framework_Print_Monitor(const char _Head[], const int Row_Number, ...) {
	system("cls");	//清屏
	va_list ap;
	va_start(ap, Row_Number);
	PADDING_TOP;
	Edition_print_formatted();
	HORIZONTAL_LINE_TOP;
	if (_Head)
		Head_Print(_Head);
	VERTICAL_LINE;
	for (int Number = 1; Number <= Row_Number; Number++)
		Content_Print(va_arg(ap, char*));
	va_end(ap);
	VERTICAL_LINE;
	HORIZONTAL_LINE_BOTTOM;
}

//通知框架
#define _CAUTION_FRAME(PROMPT) General_Framework_Print_Monitor("系统通知",1,PROMPT)

//确认函数框架
short _Confirm_Frame(const char prompt[]) {
	while (1) {
		General_Framework_Print_Monitor("系统通知", 4, prompt, "", "Y - 是", "N - 否");
		printf("\n\n\t");
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
