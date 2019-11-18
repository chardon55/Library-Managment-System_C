#pragma once
//					结构输出头文件						//
//								<> with ♥ by 董岳 (dy55)	//
//	2019-4-15, 4-21, 4-27, 4-28, 5-5, 5-8, 5-15


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

#if !LOCK
//净打印函数（图书）
void Pure_Print_BOOK(BOOK_INFO _item[], int Number) {
	for (int i = 0; i < Number; i++) {
		putchar('\t');
		printf("%lld", (_item[i].Code));
		printf("\t\t");
		no_feed_puts(_item[i].Name);
		putchar('\t');
		no_feed_puts(_item[i].Author);
		putchar('\t');
		no_feed_puts(_item[i].Publisher);
		putchar('\t');
		printf("%ld\n", _item[i].Inventory);
	}
	if (!Number)
		printf("\n\t\t无信息\n");
}

//净打印函数（读者）
void Pure_Print_READER(READER_INFO _item[], int Number) {
	for (int i = 0; i < Number; i++) {
		putchar('\t');
		printf("%lld", (_item[i].Code));
		printf("\t\t");
		no_feed_puts(_item[i].Name);
		putchar('\t');
		no_feed_puts(Char_Gender_to_String(_item[i].Gender));
		printf("\t%d\t\t%d\n", _item[i].Remain_Borrow, _item[i].Max_Borrow);
	}
	if (!Number)
		printf("\n\t\t无信息\n");
}

#endif
#define BOOK_PRINT 1		//打印图书信息
#define READER_PRINT 2		//打印读者信息

#define Print_ALL 0			//打印所有
#define Print_ONLY_ONE 1	//仅打印一项
//结构式打印总函数
void Structure_Print_Monitor(FILE* fp, const short print_type, const int _command) {
	BOOK_INFO temp_B;
	READER_INFO temp_R;
	int back_flag;
	switch (print_type) {
	case BOOK_PRINT:
		switch (_command) {
		case Print_ALL:
			back_flag = 0;
			while (!if_empty(fp))
				if (!feof(fp)) {
					Structure_Print_Monitor(fp, BOOK_PRINT, Print_ONLY_ONE);
					back_flag++;
				}
			if (!back_flag)
				printf("\n\t\t无信息\n");
			return;
		default:
			for (int i = 0; i < _command; i++) {
				fread(&temp_B, sizeof(BOOK_INFO), 1, fp);
				putchar('\t');
				printf("%lld", (temp_B.Code));
				printf("\t\t");
				no_feed_puts(temp_B.Name);
				putchar('\t');
				no_feed_puts(temp_B.Author);
				putchar('\t');
				no_feed_puts(temp_B.Publisher);
				putchar('\t');
				printf("%ld\n", temp_B.Inventory);
			}
		}
		return;
	case READER_PRINT:
		switch (_command) {
		case Print_ALL:
			back_flag = 0;
			rewind(fp);
			while (!if_empty(fp))
				if (!feof(fp)) {
					Structure_Print_Monitor(fp, READER_PRINT, Print_ONLY_ONE);
					back_flag++;
				}
			if (!back_flag)
				printf("\n\t\t无信息\n");
			return;
		default:
			for (int i = 0; i < _command; i++) {
				fread(&temp_R, sizeof(READER_INFO), 1, fp);
				putchar('\t');
				printf("%lld", temp_R.Code);
				printf("\t\t");
				no_feed_puts(temp_R.Name);
				putchar('\t');
				no_feed_puts(Char_Gender_to_String(temp_R.Gender));
				printf("\t%d\t\t%d\n", temp_R.Remain_Borrow, temp_R.Max_Borrow);
			}
			return;
		}
		break;
	default:
		return;
	}
}

#define PURE_Print -1		//净打印
#define AUTO_All -2			//自动打印全部
#define AUTO_ALL_LOG -3		//自动打印全部日志

//结构式打印导航函数
void Structure_Print_Navigator(FILE* fp, BOOK_INFO print_item_1[], READER_INFO print_item_2[], int print_Number, const short print_type, const int _command) {
	if (_command == AUTO_ALL_LOG) {
		long long log_count = 0;
		LOG_INFO temp_log;
		fp = fopen("Operation_Manage_Log.dat", "rb");
		if (!fp) {
			_ERROR("File is not existent\n");
			Countdown_Continue(3, "系统将于", "返回");
			return;
		}
		printf("\n操作时间\t\t图书编号\t读者编号\t操作\n\n");
		while(!if_empty(fp))
			if (!feof(fp)) {
				fread(&temp_log, sizeof(LOG_INFO), 1, fp);
				no_feed_puts(temp_log._DateTime_Str);
				putchar('\t');
				printf("%lld", (temp_log.Book_code));
				printf("\t\t");
				printf("%lld", (temp_log.Reader_code));
				printf("\t\t");
				no_feed_puts(Operation_Type_Convert(temp_log.Operation));
				putchar('\n');
				log_count++;
			}
		printf("\n\t\t共%ld条日志信息\n",log_count);
		return;
	}
	if (print_type == BOOK_PRINT && _command == AUTO_All) {
		fp = fopen("Books_Manage_Log.dat", "r");
		if (fp == NULL) {
			_ERROR("File is not existent\n");
			Countdown_Continue(3, "系统将于", "返回");
			return;
		}
	}
	else if (print_type == READER_PRINT && _command == AUTO_All) {
		fp = fopen("Readers_Manage_Log.dat", "r");
		if (fp == NULL) {
			_ERROR("File is not existent\n");
			Countdown_Continue(3, "系统将于", "返回");
			return;
		}
	}
	switch (print_type) {
	case BOOK_PRINT:
		printf("\t图书编号\t书名\t作者\t出版商\t库存\n\n");
		switch (_command) {
		case PURE_Print:
			Pure_Print_BOOK(print_item_1, print_Number);
			break;
		case AUTO_All:
			rewind(fp);
			Structure_Print_Monitor(fp, BOOK_PRINT, Print_ALL);
			printf("\n\n\t总数: %lld\n\n", Books_Type_Stock);
			Press_Continue;
			break;
		default:
			Structure_Print_Monitor(fp, print_type, _command);
			break;
		}
		return;
	case READER_PRINT:
		printf("\t读者编号\t姓名\t性别\t剩余借书量\t最大借书量\n\n");
		switch (_command) {
		case PURE_Print:
			Pure_Print_READER(print_item_2, print_Number);
			break;
		case AUTO_All:
			rewind(fp);
			Structure_Print_Monitor(fp, READER_PRINT, Print_ALL);
			printf("\n\n\t总数: %lld\n\n", Readers_Sum);
			Press_Continue;
			break;
		default:
			Structure_Print_Monitor(fp, print_type, _command);
		}
		return;
	default:
		return;
	}
}

//选择打印函数（日志）
void Selected_Print_Log(FILE* fp, BOOK_INFO* _book, READER_INFO* _reader) {
	long long code_info;
	LOG_INFO temp_log;
	rewind(fp);
	printf("\n操作时间\t\t图书编号\t读者编号\t操作\n\n");
	if (_book) {
		long long log_count = 0;
		LOG_INFO temp_log;
		fp = fopen("Operation_Manage_Log.dat", "rb");
		if (!fp) {
			_ERROR("File is not existent\n");
			Countdown_Continue(3, "系统将于", "返回");
			return;
		}
		while (!if_empty(fp))
			if (!feof(fp)) {
				fread(&temp_log, sizeof(LOG_INFO), 1, fp);
				if (temp_log.Book_code == _book->Code) {
					no_feed_puts(temp_log._DateTime_Str);
					putchar('\t');
					printf("%lld", (temp_log.Book_code));
					printf("\t\t");
					printf("%lld", (temp_log.Reader_code));
					printf("\t\t");
					no_feed_puts(Operation_Type_Convert(temp_log.Operation));
					putchar('\n');
					log_count++;
				}
			}
		printf("\n\t\t共%ld条日志信息\n", log_count);
	}
	if (_reader) {
		long long log_count = 0;
		LOG_INFO temp_log;
		fp = fopen("Operation_Manage_Log.dat", "rb");
		if (!fp) {
			_ERROR("File is not existent\n");
			Countdown_Continue(3, "系统将于", "返回");
			return;
		}
		while (!if_empty(fp))
			if (!feof(fp)) {
				fread(&temp_log, sizeof(LOG_INFO), 1, fp);
				if (temp_log.Reader_code == _reader->Code) {
					no_feed_puts(temp_log._DateTime_Str);
					putchar('\t');
					printf("%lld", (temp_log.Book_code));
					printf("\t\t");
					printf("%lld", (temp_log.Reader_code));
					printf("\t\t");
					no_feed_puts(Operation_Type_Convert(temp_log.Operation));
					putchar('\n');
					log_count++;
				}
			}
		printf("\n\t\t共%ld条日志信息\n", log_count);
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