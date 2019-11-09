#pragma hdrstop										//	Abandoned
//					节点结构头文件						//
//								<> with ♥ by 董岳	//
//	2019-4-15, 4-17, 4-24, 4-27, 5-1


//书类信息节点
typedef struct BOOK_node_mono {
	char Name[NAME_MAXLEN];
	char Code[CODE_MAXLEN];
	char Author[PEOPLE_NAME_MAXLEN];
	char Publisher[PUBLISHER_MAXLEN];
	long Inventory;
	struct BOOK_node_mono* next;
} BOOK_INFO_NODE;


int Book_count = 0;		//待插入书数量

//节点插入函数（书类管理）
short Insert_Node_BOOKS(BOOK_INFO_NODE* target) {

	int id_state;
	
	while (1) {
		printf("\n输入编号:\n");
		gets_s(target->Code, CODE_MAXLEN);
		id_state = ID_IF_Same(_BOOK, target->Code, NULL);
		if (!id_state);
		else if (id_state == 1) {
			printf("\n编号重复，请重新输入\n");
			Press_Continue;
			continue;
		}
		else if (id_state == -1) {
			printf("\n文件错误\n");
			Countdown_Continue(2, "", "继续");
		}
		break;
	}
	printf("\n输入书名:\n");
	gets_s(target->Name, NAME_MAXLEN);
	printf("\n输入作者名:\n");
	gets_s(target->Author, PEOPLE_NAME_MAXLEN);
	printf("\n输入出版商名:\n");
	gets_s(target->Publisher, PUBLISHER_MAXLEN);
	printf("\n输入库存数:\n");
	scanf("%ld", &(target->Inventory));
	getchar();
	target->next = NULL;
	return _Confirm("继续输入");
}

//节点创建函数（书类管理）
BOOK_INFO_NODE* Node_Create_BOOKS() {
	BOOK_INFO_NODE* head, * p, * tail;
	head = tail = NULL;
	short whether_Continue = 0;
	do {
		if ((p = (BOOK_INFO_NODE*)malloc(sizeof(BOOK_INFO_NODE))) == NULL) {
			_ERROR("Memory Allocation Failed\n");
			Countdown_Continue(3, "系统将于", "退出进程");
			return NULL;
		}
		printf("\n\t第%2d本:\n", ++Book_count);
		whether_Continue = Insert_Node_BOOKS(p);
		if (head == NULL)
			head = p;
		else
			tail->next = p;
		tail = p;
	} while (whether_Continue);
	return head;
}

//链表删除函数
void Node_Free(BOOK_INFO_NODE* Head_B) {
	BOOK_INFO_NODE* p, * pnext;
	p = Head_B;
	pnext = p->next;
	while (p) {
		free(p);
		p = pnext;
		if (pnext)
			pnext = pnext->next;
	}
}

//链表转换复制工具（书类）
void Convert_CpyTool_FromNode(BOOK_INFO* _Destination, BOOK_INFO_NODE* _Source) {
	strcpy(_Destination->Code, _Source->Code);
	strcpy(_Destination->Name, _Source->Name);
	strcpy(_Destination->Author, _Source->Author);
	strcpy(_Destination->Publisher, _Source->Publisher);
	_Destination->Inventory = _Source->Inventory;
}
//链表转换函数（书类）
void Node_Convert_BOOK(BOOK_INFO* _Destination, BOOK_INFO_NODE* _Source_Node) {
	if (_Source_Node == NULL)
		return;
	BOOK_INFO_NODE * p = _Source_Node;
	int i;
	if ((_Destination = (BOOK_INFO*)calloc(Book_count, sizeof(BOOK_INFO))) == NULL) {
		_ERROR("Memory Allocation Failed\n");
		Countdown_Continue(3, "系统将于", "返回");
		return;
	}
	for (p = _Source_Node, i = 0; i < Book_count; p = p->next)
		Convert_CpyTool_FromNode(&(_Destination[i++]), _Source_Node);
	return;
}
