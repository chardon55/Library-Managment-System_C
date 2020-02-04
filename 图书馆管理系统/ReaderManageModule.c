#pragma once
//////////////////////////////////////////////////////////////////
//																//
//							读者管理模块							//
//											<> with ♥ by 董岳 (dy55)	//
//////////////////////////////////////////////////////////////////
//	2019-4-22, 4-24, 4-26, 5-1, 5-2, 5-5, 5-8, 5-13, 5-17, 5-18

extern long long Readers_Sum;

#if 0	//Deprecated
extern int Readers_count;
#endif

int Reader_Query_Count;				//查询结果（读者信息）


//读者查询工具函数 (Extensively recoded at 1905.12)
FILE* Readers_Query_Tool(READER_INFO info, const int _operation) {
	Reader_Query_Count = 0;
	FILE* fp = fopen("Readers_Manage_Log.dat", "ab+"), * temp_p = NULL;
	READER_INFO temp;
	if (!fp) {
		_ERROR("File opening failed");
		perror("Readers_Manage_Log.dat");
		Countdown_Continue(3, "", "返回");
		return NULL;
	}
	int flag = 0;
	switch (_operation) {
	case By_CODE:
		while (!if_empty(fp)) {
			if (!feof(fp)) {
				fread(&temp, sizeof(READER_INFO), 1, fp);
			}
			else
				break;
			if (temp.Code == info.Code) {
				flag = 1;
				Reader_Query_Count = 1;
				fseek(fp, -sizeof(READER_INFO), SEEK_CUR);
				return fp;
			}
		}
		break;
	case By_NAME:
		printf("\t读者编号\t姓名\t性别\t剩余借书量\t最大借书量\n\n");
		while (!if_empty(fp)) {
			if (!feof(fp)) {
				temp_p = fp;
				fread(&temp, sizeof(READER_INFO), 1, fp);
			}
			else
				break;
			if (!strcmp(temp.Name, info.Name)) {
				flag = 1;
				Reader_Query_Count++;
				Pure_Print_READER(&temp, 1);
			}
		}
		break;
	default:
		break;
	}
	if(!flag)
		return NULL;
	return temp_p;
}

//读者添加工具函数
void Reader_Add_Tool(READER_INFO Info) {
	FILE* fp = fopen("Readers_Manage_Log.dat", "ab+");
	READER_INFO temp;
	if (!fp) {
		_ERROR("File opening failed");
		Countdown_Continue(3, "", "返回");
		return;
	}
	EMPTY_OPERATION(fp, rewind(fp));
	nEMPTY_OPERATION(fp, fseek(fp, 0L, SEEK_END));
	Fill_mode(fp);
	fwrite(&Info, sizeof(READER_INFO), 1, fp);
	if (fclose(fp)) {
		_ERROR("File close failed");
		Countdown_Continue(3, "", "直接退出");
		return;
	}
	Readers_Sum++;
}

//读者添加总函数
void Reader_Add_Main() {
	READER_INFO Reader_Info;
	char char_temp[5];
	while (1) {
		while (1) {
			General_Framework_Print_Monitor("读者添加向导", 3, "请输入读者信息", "", "输入读者编号:");
			scanf("%lld", &Reader_Info.Code);
			getchar();
			if (Readers_Query_Tool(Reader_Info, By_CODE)) {
				_CAUTION_FRAME("已存在该读者");
				Countdown_Continue(2, "请于", "重新输入");
				continue;
			}
			General_Framework_Print_Monitor("读者添加向导", 3, "请输入读者信息", "", "输入读者姓名:");
			gets_s(Reader_Info.Name, PEOPLE_NAME_MAXLEN);
			while (1) {
				General_Framework_Print_Monitor("读者添加向导", 4, "请输入读者信息", "", "输入性别:", "（输入男 / 女 或 M / F）");
				gets_s(char_temp, 3);
				Reader_Info.Gender = String_Gender_to_Char(char_temp);
				if (!Reader_Info.Gender) {
					_CAUTION_FRAME("格式错误");
					Countdown_Continue(1, "", "重新输入");
					continue;
				}
				break;
			}
			while (1) {
				Framework_Print_Monitor("选择读者身份:", "退出", 5, "本科生", "研究生", "博士生", "教师", "教授");
				scanf("%d", &Reader_Info.ID_Code);
				getchar();
				if (!Reader_Info.ID_Code)
					if (_Confirm_Frame("确认是否退出"))
						return;
					else
						continue;
				if (Reader_Info.ID_Code > 5 || Reader_Info.ID_Code < 0) {
					_CAUTION_FRAME("无效选择");
					Press_Continue;
					continue;
				}
				Reader_Info.Max_Borrow = Max_Borrow_By_Identity(Id_Classify_fromCode(Reader_Info.ID_Code));
				Reader_Info.Remain_Borrow = Reader_Info.Max_Borrow;
				break;
			}
			break;
		}
		if (_Confirm_Frame("是否执行操作")) {
			Reader_Add_Tool(Reader_Info);
			_Operation_Complete;
			if (!_Confirm_Frame("是否继续添加"))
				return;
		}
		else {
			_Operation_Canceled;
		}
	}
}

//读者查询导航函数 (Extensively recoded at 1905.12) 
FILE* Readers_Query_Navigator(const short choice) {
	READER_INFO _target;
	FILE* temp_fp, * target_fp;
	char _tempchar[6];
	switch (choice) {
	default:
		_CAUTION_FRAME("无效选择");
		Countdown_Continue(3, "", "重新选择");
		return 0;
	case By_CODE:
		General_Framework_Print_Monitor("读者查询向导", 1, "输入读者编号:");
		scanf("%lld", &_target.Code);
		getchar();
		if (!(temp_fp = Readers_Query_Tool(_target, By_CODE))) {
			_CAUTION_FRAME("未找到读者信息");
			Countdown_Continue(3, "系统将于", "返回");
			return 0;
		}
		else {
			target_fp = temp_fp;
			system("cls");
			Structure_Print_Navigator(target_fp, NULL, NULL, 1, READER_PRINT, Print_ONLY_ONE);
			Press_Continue;
		}
		break;
	case By_NAME:
		General_Framework_Print_Monitor("读者查询向导", 1, "输入读者姓名:");
		gets_s(_target.Name, PEOPLE_NAME_MAXLEN);
		system("cls");
		if (!(Readers_Query_Tool(_target, By_NAME))) {
			_CAUTION_FRAME("未找到读者信息");
			Countdown_Continue(3, "系统将于", "返回");
			return NULL;
		}
		else
			Press_Continue;
		return NULL;
	}
	return target_fp;
}

//读者删除工具函数
void Readers_Delete_Tool(READER_INFO Target) {
	FILE* fp = fopen("Readers_Manage_Log.dat", "ab+");
	if (!Readers_Sum) {
		_CAUTION_FRAME("未找到读者信息");
		return;
	}
	READER_INFO t1;
	FILE* ptr = fopen(".temp", "wb");
	rewind(fp);
	while (!if_empty(fp))
		if (!feof(fp)) {
			fread(&t1, sizeof(READER_INFO), 1, fp);
			if (Target.Code != t1.Code) {
				Fill_mode(ptr);
				fwrite(&t1, sizeof(READER_INFO), 1, ptr);
			}
		}
	fcloseall();
	remove("Readers_Manage_Log.dat");
	rename(".temp", "Readers_Manage_Log.dat");
	Readers_Sum--;
}

//读者删除函数
void Readers_Delete_Main() {
	int i, Del_choice;
	READER_INFO target;
	int _choice;
	while (1) {
		General_Framework_Print_Monitor("读者删除向导", 1, "输入编号");
		scanf("%lld", &target.Code);
		getchar();
		if (!Readers_Query_Tool(target, By_CODE)) {
			_CAUTION_FRAME("未找到");
			Countdown_Continue(1, "", "重新输入");
			continue;
		}
		while (1) {
			if (_Confirm_Frame("确认删除")) {
				Readers_Delete_Tool(target);
				_Operation_Complete;
			}
			fcloseall();
			return;
		}
	}
}

#define Modify_GENDER 3					//修改性别
#define Modify_REMAIN 4					//修改剩余可借书量
#define Modify_MAX_ 5					//修改最大可借书量

READER_INFO History_oldR, History_newR;	//历史记录

//读者信息修改工具
FILE* Readers_Modification_Tool(READER_INFO Old, READER_INFO New, const short Content_toUpdate) {
	FILE* fp_pointer, * tfp = fopen(".temp", "wb+");
	READER_INFO temp, t_old_new;
	fp_pointer = Readers_Query_Tool(Old, By_CODE);
	fread(&Old, sizeof(READER_INFO), 1, fp_pointer);
	History_oldR = t_old_new = Old;
	rewind(fp_pointer);
	
	switch (Content_toUpdate) {
	case Modify_CODE:
		t_old_new.Code = New.Code;
		break;
	case Modify_NAME:
		strcpy(t_old_new.Name, New.Name);
		break;
	case Modify_GENDER:
		t_old_new.Gender = New.Gender;
		break;
	case Modify_REMAIN:
		t_old_new.Remain_Borrow = New.Remain_Borrow;
		break;
	case Modify_MAX_:
		t_old_new.Max_Borrow = New.Max_Borrow;
		break;
	default:return NULL;
	}
	while(!if_empty(fp_pointer))
		if (!feof(fp_pointer)) {
			Fill_mode(tfp);
			fread(&temp, sizeof(READER_INFO), 1, fp_pointer);
			if (temp.Code == Old.Code)
				fwrite(&t_old_new, sizeof(READER_INFO), 1, tfp);
			else
				fwrite(&temp, sizeof(READER_INFO), 1, tfp);
		}
	fcloseall();
	remove("Readers_Manage_Log.dat");
	rename(".temp", "Readers_Manage_Log.dat");
	History_newR = t_old_new;
	return fp_pointer;
}
//读者信息修改向导
int Readers_Modification_Wizard(READER_INFO * _Destination, int type) {
	system("cls");
	READER_INFO target;
	FILE* fp;
	int temp, beyond_Flag = 0;
	char gender_Str[3], temp_str[8];
	printf("\n\n输入待修改读者编号:\n");
	General_Framework_Print_Monitor("读者修改向导", 1, "输入待修改读者编号");
	scanf("%lld", &target.Code);
	getchar();
	if (!(fp = Readers_Query_Tool(target, By_CODE))) {
		system("cls");
		return 0;
	}
	General_Framework_Print_Monitor("读者修改向导", 1, "输入新信息");
	switch (type) {
	case Modify_CODE:
		while (1) {
			scanf("%lld", &_Destination->Code);
			getchar();
			if (Readers_Query_Tool(*_Destination, By_CODE)) {
				_CAUTION_FRAME("已存在该编号的读者");
				Countdown_Continue(2, "请于", "重新输入");
				General_Framework_Print_Monitor("读者修改向导", 1, "输入新信息");
				continue;
			}
			break;
		}
		break;
	case Modify_NAME:
		gets_s(_Destination->Name, PEOPLE_NAME_MAXLEN);
		break;
	case Modify_GENDER:
		fread(&target, sizeof(READER_INFO), 1, fp);
		switch (target.Gender) {
		case 'M':
			if (_Confirm_Frame("确认修改为 女")) {
				_Destination->Gender = 'F';
				Readers_Modification_Tool(target, *_Destination, type);
				system("cls");
				printf("\n\n修改完成\n");
				printf("\nBefore:\n");
				Structure_Print_Navigator(NULL, NULL, &History_oldR, 1, READER_PRINT, PURE_Print);
				printf("\nAfter:\n");
				Structure_Print_Navigator(NULL, NULL, &History_newR, 1, READER_PRINT, PURE_Print);
				putchar('\n');
				Countdown_Continue(2, "系统将于", "继续");
			}
			return 1;
		case 'F':
			if (_Confirm_Frame("确认修改为 男")) {
				_Destination->Gender = 'M';
				Readers_Modification_Tool(target, *_Destination, type);
				system("cls");
				printf("\n\n修改完成\n");
				printf("\nBefore:\n");
				Structure_Print_Navigator(NULL, NULL, &History_oldR, 1, READER_PRINT, PURE_Print);
				printf("\nAfter:\n");
				Structure_Print_Navigator(NULL, NULL, &History_newR, 1, READER_PRINT, PURE_Print);
				putchar('\n');
			}
			return 1;
		default:
			return 0;
		}
	case Modify_REMAIN:
		while (1) {
			scanf("%d", &temp);
			fread(&target, sizeof(READER_INFO), 1, fp);
			if (temp > target.Max_Borrow) {
				_CAUTION_FRAME("超出最大可借书量");
				Countdown_Continue(2, "请于", "重新输入");
				General_Framework_Print_Monitor("读者修改向导", 1, "输入新信息");
			}
			else {
				_Destination->Remain_Borrow = temp;
				break;
			}
		}
		break;
	case Modify_MAX_:
		if (_Confirm_Frame("确认是否修改读者身份")) {
			Framework_Print_Monitor("选择读者身份:", "退出", 5, "本科生", "研究生", "博士生", "教师", "教授");
			scanf("%d", &(_Destination->ID_Code));
			getchar();
			_Destination->Max_Borrow = Max_Borrow_By_Identity(Id_Classify_fromCode(_Destination->ID_Code));
			if (_Destination->Remain_Borrow > _Destination->Max_Borrow)
				_Destination->Remain_Borrow = _Destination->Max_Borrow;
		}
		else {
			General_Framework_Print_Monitor("读者修改向导", 1, "输入自定义借书量");
			scanf("%d", &(_Destination->Max_Borrow));
			getchar();
			fread(&target, sizeof(READER_INFO), 1, fp);
			if (target.Remain_Borrow > _Destination->Max_Borrow) {
				_Destination->Remain_Borrow = _Destination->Max_Borrow;
				beyond_Flag = 1;
			}
		}
		break;
	default:
		_CAUTION_FRAME("无效输入");
		Countdown_Continue(2, "系统将于", "返回");
		return 0;
	}
	if (_Confirm_Frame("确认修改")) {
		Readers_Modification_Tool(target, *_Destination, type);
		if (beyond_Flag)
			Readers_Modification_Tool(target, *_Destination, Modify_REMAIN);
		system("cls");
		printf("\n\n修改完成\n");
		printf("\nBefore:\n");
		Structure_Print_Navigator(NULL, NULL, &History_oldR, 1, READER_PRINT, PURE_Print);
		printf("\nAfter:\n");
		Structure_Print_Navigator(NULL, NULL, &History_newR, 1, READER_PRINT, PURE_Print);
		putchar('\n');
		Countdown_Continue(2, "系统将于", "继续");
	}
	return 1;
}
//读者信息修改函数
void Readers_Modification_Main() {
	int choice;
	READER_INFO target, New_info_;
	Framework_Print_Monitor("修改读者信息", "返回", 5, "修改读者编号", "修改读者姓名", "修改读者性别", "修改剩余可借书量", "修改最大可借书量");
	CHOOSE_PLEASE(&choice);
	if (!choice && _Confirm_Frame("确认取消操作"))
		return;
	if (!Readers_Modification_Wizard(&New_info_, choice)) {
		_CAUTION_FRAME("未找到");
		Countdown_Continue(3, "系统将于", "返回");
	}
}

//读者查询向导函数
void Readers_Query_Wizard() {
	int choice;
	while (1) {
		Framework_Print_Monitor("查询读者信息", "返回", 2, "输入编号", "输入姓名");
		CHOOSE_PLEASE(&choice);
		if (!choice)
			return;
		Readers_Query_Navigator(choice);
		if (!_Confirm_Frame("是否继续查询"))
			return;
	}
}

//读者管理模块函数
void Readers_Manage_Choice() {
	int _choice;
	while (1) {
		Framework_Print_Monitor("读者管理模块", "返回", 5, "新增读者信息", "删除读者信息", "修改读者信息", "查询读者信息", "显示所有读者信息");
		CHOOSE_PLEASE(&_choice);
		switch (_choice) {
		case 1:
			Reader_Add_Main();
			break;
		case 2:
			if (!Readers_Sum) {
				_CAUTION_FRAME("没有读者信息");
				Countdown_Continue(3, "系统将于", "返回");
				break;
			}
			Readers_Delete_Main();
			break;
		case 3:
			if (!Readers_Sum) {
				_CAUTION_FRAME("没有读者信息");
				Countdown_Continue(3, "系统将于", "返回");
				break;
			}
			Readers_Modification_Main();
			break;
		case 4:
			if (!Readers_Sum) {
				_CAUTION_FRAME("没有读者信息");
				Countdown_Continue(3, "系统将于", "返回");
				break;
			}
			Readers_Query_Wizard();
			break;
		case 5:
			system("cls");
			Structure_Print_Navigator(NULL, NULL, NULL, 0, READER_PRINT, AUTO_All);
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
