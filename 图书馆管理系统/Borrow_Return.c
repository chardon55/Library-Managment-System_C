#pragma once
//////////////////////////////////////////////////////////////////
//																//
//						借书还书登记模块							//
//											<> with ♥ by 董岳	//
//////////////////////////////////////////////////////////////////
//	2019-4-24, 4-26, 4-27, 4-28, 4-29, 4-30, 5-4, 5-5, 5-13, 5-18,
//		5-20


extern long long Books_Type_Stock;

extern long long Readers_Sum;


//借书向导函数
void Borrow_Wizard() {
	READER_INFO Reader;
	BOOK_INFO Book;
	FILE* fp, * readerfp;
	while (1) {
		General_Framework_Print_Monitor("借书登记向导", 1, "请输入借书读者编号:");
		scanf("%lld", &Reader.Code);
		getchar();
		if (!(readerfp = Readers_Query_Tool(Reader, By_CODE))) {
			printf("未找到该读者信息\n");
			Press_Continue;
			continue;
		}
		fread(&Reader, sizeof(READER_INFO), 1, readerfp);
		History_oldR = Reader;
		if (Reader.Remain_Borrow <= 0) {
			_CAUTION_FRAME("剩余借书量不足");
			Countdown_Continue(3, "", "返回");
			return;
		}
			break;
	}
	General_Framework_Print_Monitor("借书登记向导", 1, "请输入图书编号:");
	scanf("%lld", &Book.Code);
	getchar();
	if(Books_Query_Tool(Book,By_CODE))
		fp = BOOK_Positioning(Book);
	else {
		_CAUTION_FRAME("未找到该图书");
		Countdown_Continue(2, "系统将于", "返回");
		return;
	}
	fread(&Book, sizeof(BOOK_INFO), 1, fp);
	if (Book.Inventory <= 0) {
		_CAUTION_FRAME("库存错误");
		Countdown_Continue(2, "系统将于", "返回");
		return;
	}
	History_oldB = Book;
	if (_Confirm_Frame("确认继续")) {
		Book.Inventory--;
		Reader.Remain_Borrow--;
		Readers_Modification_Tool(History_oldR, Reader, Modify_REMAIN);
		Books_Modification_Tool(History_oldB, Book);
		fcloseall();
		Log_generate(Book, Reader, _BORROW);

		_Operation_Complete;
	}
	else {
		_Operation_Canceled;
	}
}

#define BOOK_Targeted	1			//针对图书
#define READER_Targeted	2			//针对读者

#define Print_CERTAIN 0				//打印特定
#define Print_ALL 1					//打印全部

//日志查询函数
void Log_Query(BOOK_INFO* target_book, READER_INFO* target_reader, int target_option, int command) {
	FILE* fp = fopen("Operation_Manage_Log.dat", "rb");
	if (!fp) {
		_ERROR("Can't open the file\n");
		return;
	}
	switch (command) {
	case Print_CERTAIN:
		switch (target_option) {
		case BOOK_Targeted:
			Selected_Print_Log(fp, target_book, NULL);
			break;
		case READER_Targeted:
			Selected_Print_Log(fp, NULL, target_reader);
			break;
		default:
			_CAUTION_FRAME("无效指令");
			break;
		}
		break;
	case Print_ALL:
		Structure_Print_Navigator(NULL, NULL, NULL, 0, NULL, AUTO_ALL_LOG);
		break;
	default:
		_CAUTION_FRAME("无效指令");
		break;
	}
	if (fclose(fp)) {
		_ERROR("File close failed\n");
		return;
	}
	Press_Continue;
}

#define Search_Empty -1
#define BORROW_EXISTS -2

#define if_exist_Borrow(BOOK_CODE, READER_CODE) Log_Search_Tool(BOOK_CODE, READER_CODE, BORROW_EXISTS)

//日志搜索函数
int Log_Search_Tool(long long Book_Code, long long Reader_Code, int command_type) {
	int Log_count = 0;
	LOG_INFO temp_log;
	if (command_type == BORROW_EXISTS)
		return Log_Search_Tool(Book_Code, Reader_Code, _BORROW) > Log_Search_Tool(Book_Code, Reader_Code, _RETURN);
	FILE* fp = fopen("Operation_Manage_Log.dat", "rb");
	if (!fp)
		return -1;
	while(!if_empty(fp))
		if (!feof(fp)) {
			fread(&temp_log, sizeof(LOG_INFO), 1, fp);
			switch (command_type)
			{
			case _BORROW:
				if (
					temp_log.Book_code == Book_Code
					&& temp_log.Reader_code == Reader_Code
					&& temp_log.Operation == _BORROW
					)
					Log_count++;
				break;
			case _RETURN:
				if (
					temp_log.Book_code == Book_Code
					&& temp_log.Reader_code == Reader_Code
					&& temp_log.Operation == _RETURN
					)
					Log_count++;
				break;
			case Search_Empty:
				Log_count++;
			default:
				break;
			}
		}
	fclose(fp);
	return Log_count;
}


//还书向导函数
void Return_Wizard() {
	BOOK_INFO book;
	READER_INFO reader;
	FILE* fpb, * fpr;
	while (1) {
		General_Framework_Print_Monitor("还书登记向导", 1, "请输入读者编号:");
		scanf("%lld", &reader.Code);
		if (!Readers_Query_Tool(reader, By_CODE)) {
			_CAUTION_FRAME("未找到该读者");
			Countdown_Continue(2, "请在", "重新输入");
			continue;
		}
		getchar();
		General_Framework_Print_Monitor("还书登记向导", 1, "请输入所还图书编号:");
		scanf("%lld", &book.Code);
		getchar();
		if (!Books_Query_Tool(book, By_CODE)) {
			_CAUTION_FRAME("未找到该图书");
			Countdown_Continue(2, "请在", "重新输入");
			continue;
		}
		if (!if_exist_Borrow(book.Code, reader.Code)) {
			_CAUTION_FRAME("未找到对应借书记录");
			Countdown_Continue(2, "请在", "重新输入");
			continue;
		}
		fpb = BOOK_Positioning(book);
		fpr = Readers_Query_Tool(reader, By_CODE);
		fread(&book, sizeof(BOOK_INFO), 1, fpb);
		History_oldB = book;
		fread(&reader, sizeof(READER_INFO), 1, fpr);
		History_oldR = reader;
		if (_Confirm_Frame("确认继续")) {
			book.Inventory++;
			reader.Remain_Borrow++;
			Books_Modification_Tool(History_oldB, book);
			Readers_Modification_Tool(History_oldR, reader, Modify_REMAIN);
			Log_generate(book, reader, _RETURN);
		}
		else {
			_Operation_Canceled;
			return;
		}
		_Operation_Complete;
		break;
	}
}


//日志查询向导函数
void Log_Query_Wizard() {
	int _choice;
	BOOK_INFO tar_Book;
	READER_INFO tar_Reader;
	while (1) {
		Framework_Print_Monitor("日志查询", "返回", 3, "查询特定图书日志", "查询特定读者日志", "显示全部日志信息");
		CHOOSE_PLEASE(&_choice);
		switch (_choice) {
		case 1:
			system("cls");
			printf("\n\n\t请输入图书编号");
			scanf("%lld", &(tar_Book.Code));
			getchar();
			Log_Query(&tar_Book, NULL, BOOK_Targeted, Print_CERTAIN);
			break;
		case 2:
			system("cls");
			printf("\n\n\t请输入读者编号");
			scanf("%lld", &(tar_Reader.Code));
			getchar();
			Log_Query(NULL, &tar_Reader, READER_Targeted, Print_CERTAIN);
			break;
		case 3:
			system("cls");
			Log_Query(NULL, NULL, NULL, Print_ALL);
			break;
		case 0:
			return;
		default:
			_CAUTION_FRAME("无效选项");
			Countdown_Continue(2, "系统将于", "继续");
			break;
		}
	}
}

//借书 / 还书总函数
void Borrow_Return_main() {
	if (!Books_Type_Stock && !Readers_Sum) {
		_CAUTION_FRAME("无图书和读者信息");
		Countdown_Continue(2, "系统将于", "返回");
	}
	else if (!Books_Type_Stock) {
		_CAUTION_FRAME("无图书信息");
		Countdown_Continue(2, "系统将于", "返回");
	}
	else if (!Readers_Sum) {
		_CAUTION_FRAME("无读者信息");
		Countdown_Continue(2, "系统将于", "返回");
	}
	else
		while (1) {
			int choice;
			Framework_Print_Monitor("借书还书登记模块", "返回", 3, "借书登记", "还书登记", "日志查询");
			CHOOSE_PLEASE(&choice);
			switch (choice) {
			case 1:
				Borrow_Wizard();
				fcloseall();
				break;
			case 2:
				Return_Wizard();
				fcloseall();
				break;
			case 3:
				Log_Query_Wizard();
				break;
			case 0:
				return;
			default:
				_CAUTION_FRAME("无效选择");
				Countdown_Continue(2, "系统将于", "继续");
				break;
			}
		}
}