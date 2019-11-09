//////////////////////////////////////////////////////////////////
//																//
//							高级设置								//
//											<> with ♥ by 董岳	//
//////////////////////////////////////////////////////////////////
//	2019-5-5, 5-8, 5-18

extern long long Books_Type_Stock;
extern long long Readers_Sum;

//日志重置 (Since 1905.16)
void Reset_Logs() {
	int choice, flag = 1;
	FILE* fp;
	Framework_Print_Monitor("日志重置", "返回", 4, "重置图书日志", "重置读者日志", "重置操作日志", "重置所有日志");
	CHOOSE_PLEASE(&choice);
	if (_Confirm_Frame("确认重置")) {
		switch (choice) {
		case 0:
			return;
		case 4:
			flag = 0;
		case 1:
			fp = fopen("Books_Manage_Log.dat", "wb");
			Books_Type_Stock = 0;
			fclose(fp);
			if (flag)
				break;
		case 2:
			fp = fopen("Readers_Manage_Log.dat", "wb");
			Readers_Sum = 0;
			fclose(fp);
			if (flag)
				break;
		case 3:
			fp = fopen("Operation_Manage_Log.dat", "wb");
			fclose(fp);
			if (flag)
				break;
		default:
			break;
		}
		_Operation_Complete;
	}
}

//日志清除 (Since 1905.21)
void Clear_LOGS() {
	remove("Books_Manage_Log.dat");
	remove("Readers_Manage_Log.dat");
	remove("Operation_Manage_Log.dat");
}

//高级设置向导
void Advanced_Settings() {
	int Choice;
	fcloseall();
	while (1) {
		Framework_Print_Monitor("高级设置", "返回", 2, "日志重置", "清除日志");
		CHOOSE_PLEASE(&Choice);
		switch(Choice) {
		case 0:
			return;
		case 1:
			Reset_Logs();
			break;
		case 2:
			if (_Confirm_Frame("确认清除日志"))
				Clear_LOGS();
			_Operation_Complete;
			break;
		default:
			break;
		}
	}
}