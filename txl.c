#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define BUFSIZE 128

char *file_name = "saveNodeFile.txt";    //保存链表节点的文件名


const char* fmt_title = "%-20s%-5s%-25s%12s\n"; //标题行格式化控制字符串
const char* fmt_str = "%-20s%-5u%-25s%12s\n"; //节点数据格式化控制字符串
void print_title()
{
    printf(fmt_title, "姓名", "年龄", "地址", "电话号码");
    printf("-------------------------------------------------------------------\n");
}


/**
 * 一、定义通讯录节点  ------>引入txl.h头文件中的定义
 */ 
typedef struct _addr_book
{
    //数据域部分
    char _name[20]; //姓名
    unsigned _age; //年龄
    char _address[128]; //地址
    char _phone_num[12]; //电话号码
    //指针域部分
    struct _addr_book* next;  //结构体类型的指针，用来指向下一节点
}ADDRBOOK, *PADDRBOOK;


/**
 * 二、定义头指针-------->引入txl.h头文件中的定义
 */ 
PADDRBOOK head = NULL;


/**
 * 三、创建节点
 */ 
PADDRBOOK create_addrbook_node(const char* name, unsigned age, const char* addr, const char* phnum)
{
    PADDRBOOK p = (PADDRBOOK)malloc(sizeof(ADDRBOOK));
    if (p == NULL)
        return NULL;
    strcpy(p->_name, name);
    p->_age = age;
    strcpy(p->_address, addr);
    strcpy(p->_phone_num, phnum);
    p->next = NULL;
    return p;
}


/**
 * 四、向链表添加节点
 */ 
void add_addrbook(const char* name, unsigned age, const char* addr, const char* phnum)
{
    PADDRBOOK p = create_addrbook_node(name, age, addr, phnum);
    if (head == NULL) //链表为空
        head = p;
    else            //链表不为空
    {
        PADDRBOOK pPreNode = head;
        while (pPreNode->next) //下一节点不为空
            pPreNode = pPreNode->next; //让pPreNode指向下一节点
        pPreNode->next = p; //将新创建节点的地址赋给链表最后节点的next成员
    }
}


/**
 * 五、按姓名查找节点
 */
PADDRBOOK find_by_name(const char* name)
{
    PADDRBOOK p = head;
    while (p)
    {
        if (!strcmp(p->_name, name)) //若比较结果相同，strcmp返回0
            return p; //比较结果相同，即查找到，返回该节点指针
        p = p->next; //让p指向下一个节点
    }
    return NULL; //没有查找过，返回NULL（返回p亦可）
}


/**
 * 六、打印指定姓名的节点
 */
void print_by_name(const char* name)
{
    PADDRBOOK p;
    print_title(); //打印标题行
    if ((p = find_by_name(name)) != NULL) //检查返回指针是否为NULL
        printf(fmt_str, p->_name, p->_age, p->_address, p->_phone_num);
}


/**
 * 七、打印所有节点
 */
void print_all()
{
    PADDRBOOK p = head;
    print_title();
    while (p)
    {
        printf(fmt_str, p->_name, p->_age, p->_address, p->_phone_num);
        p = p->next; //让p指向下一个节点
    }
}


/**
 * 八、删除指定姓名的节点
 */
void del_addrbook(PADDRBOOK pNode) //参数为欲删除节点的指针
{
    PADDRBOOK p = head; //保存第一个节点
    if (head == NULL || pNode == NULL) //判断链表或者欲删除节点是否为空
        return;
    else if (head == pNode) //如果删除的是第一个节点
    {
        head = head->next; //让头指针指向第二个节点
        free(p); //释放第一个节点内存空间
    }
    else                //如果删除的不是第一个节点
    {
        while (p->next) //判断是否有下一节点
        {
            if (p->next == pNode) //下一节点是否为要删除的节点
            { //如果是的话
                p->next = pNode->next; //让指针p所指节点的下一节点为删除节点的下一节点
                free(pNode); //释放删除节点的内存空间
                break; //终止循环
            }
            p = p->next; //让指针p指向下一节点
        }
    }
}


/**
 * 九、删除所有节点
 */
void del_all()
{
    PADDRBOOK p;
    while(head)
    {
        p = head;
        head = head->next;
        free(p);
    }
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * 主要实现与用户的交互功能
 */

void clear_istream();

/*
一、定义枚举-------->引入txl.h头文件中的定义
*/
enum { ADD, DEL, MOD, FIND, SAVE, LOAD, SHOW, EXIT };

/*
二、显示菜单
*/
unsigned show_menu()
{
    unsigned choose;
    system("CLS"); //清屏
    printf("///////////////////////通讯录系统.V1.0/////////////////////////////\n\n");
    printf("0.添加通讯录\t1.删除通讯录\t2.修改通讯录\t3.查找通讯录\n");
    printf("4.保存到文件\t5.从文件加载\t6.显示通讯录\t7.退出系统\n\n");
    printf("//////////////////////////////////////////////////////////////////\n\n");
    printf("请输入菜单编号(0-%u):\n", EXIT);
    while (!scanf("%u", &choose) || EXIT < choose)
    {
        clear_istream();
        printf("输入有误，请重新输入:");
    }
    return choose;
}

/*
三、清楚标准输入流
*/
void clear_istream()
{
    while (getchar() != '\n');
}


/*
四、清除标准输入流前导空白字符
*/
void clear_space()
{
    int c;
    while (isspace(c = getchar()));
    ungetc(c, stdin);
}


/*
五、获取用户输入的字符串
*/
size_t get_input_string(char* buf, size_t buf_sz, const char* prompt)
{
    printf("%s", prompt);
    size_t sz = 0;
    int c;
    clear_space(); //忽略前导空白字符
    while (sz < buf_sz - 1)
    {
        if ((c = getchar()) == '\n')
        {
            ungetc(c, stdin);
            break;
        }
        else
            buf[sz++] = c;

    }
    buf[sz] = '\0';
    clear_istream();
    return sz;
}


/*
六、获取用户输入的无符号整数
*/
void get_input_digit(unsigned* num, const char* prompt)
{
    printf("%s", prompt);
    unsigned c;
    *num = 0;
    clear_space();
    while (c = getchar())
    {
        if (isdigit(c))
        {
            *num *= 10;
            *num += c - '0';
        }
        else
        {
            ungetc(c, stdin);
            break;
        }
    }
    clear_istream();
}


/*
七、修改指定姓名的节点
*/
void modify_by_name(const char* name)
{
    PADDRBOOK p = find_by_name(name);
    if (p == NULL)
        printf("没有发现姓名为 '%s'的通讯录\n", name);
    else
    {
        char buf[BUFSIZE];
        unsigned age;
        get_input_string(buf, 20, "请输入新的姓名:");
        strcpy(p->_name, buf);
        get_input_digit(&age, "请输入年龄:");
        p->_age = age;
        get_input_string(buf, 128, "请输入地址:");
        strcpy(p->_address, buf);
        get_input_string(buf, 12, "请输入电话号码:");
        strcpy(p->_phone_num, buf);
    }
}


/*
八、将链表保存至文件
*/
void save_to_file()
{
    FILE* pfile = fopen(file_name, "wb");
    if (pfile == NULL)
    {
        printf("打开文件失败\n");
        return;
    }
    PADDRBOOK p = head;
    while (p)
    {
        fwrite(p, sizeof(ADDRBOOK), 1, pfile);
        p = p->next;
    }
    fclose(pfile);
}

/*
九、从文件中加载数据
*/
void load_from_file()
{
    ADDRBOOK ab;
    FILE* pfile = fopen(file_name, "rb");
    if (pfile == NULL)
    {
        printf("打开文件失败\n");
        return;
    }
    del_all(); //删除当前所有节点
    while (fread(&ab, sizeof(ADDRBOOK), 1, pfile))
        add_addrbook(ab._name, ab._age, ab._address, ab._phone_num);
    fclose(pfile);
}


// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * 一个简单的同学通讯录程序入口
 */
int main()
{
    char name[20]; //姓名
    unsigned age = 0; //年龄
    char address[128]; //地址
    char phone_num[12]; //电话号码
    int running = 1;
    while(running)
    {
        switch (show_menu())
        {
        case ADD:
            get_input_string(name, 20, "请输入姓名:");
            get_input_digit(&age, "请输入年龄:");
            get_input_string(address, 128, "请输入地址:");
            get_input_string(phone_num, 12, "请输入电话号码:");
            add_addrbook(name, age, address, phone_num);
            break;
        case DEL:
            get_input_string(name, 20, "想要删除通讯录中的姓名为:");
            del_addrbook(find_by_name(name));
            break;
        case MOD:
            get_input_string(name, 20, "想要修改的通讯录中的姓名为:");
            modify_by_name(name);
            break;
        case FIND:
            get_input_string(name, 20, "想要查找的通讯录中的姓名为:");
            print_by_name(name);
            break;
        case SAVE:
            save_to_file();
            break;
        case LOAD:
            load_from_file();
            break;
        case SHOW:
            print_all();
            break;
        case EXIT:
            running = 0;
            break;
        }
        system("PAUSE");
    }
    del_all();
    return 0;
}


