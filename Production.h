#ifndef PRODUCTION_H
#define PRODUCTION_H
#include <vector>
#include <cstring>
#include <map>
#include <string>
using namespace std;
#define POINT -2
#define DOLLARS -3
#define my_null -4
#define ACC 999
#define BEGIN -500

#define MAX 1024


//pay attention to which argument you pass to function is want to change and which
//which are not
//pass by value or by &
class First
{
    public:
        First(int x, vector<int> set)
        {
            this->x = x;
            this->set = set;
        }

        int x;
        vector<int> set;

};

class item
{
    public:
        item()
        {

        }
        item(vector<int> pro, int follow)
        {
            this->pro = pro;
            this->follow = follow;
        }
        int size()
        {
            return pro.size();
        }
        vector<int> pro;
        int follow;
};

class set_of_items
{
    public:
        set_of_items()
        {
            list_item.clear();
        }
        int size()
        {
            return list_item.size();
        }
        item get(int i)
        {
            return list_item[i];
        }
        int insert(item i)
        {
            list_item.push_back(i);
        }
        vector<item> list_item;
};
class Ver_and_Ter
{
    public:
    Ver_and_Ter(int num, char* str , bool ter)
    {
        this->num = num;
        strcpy(this->str, str);
        this->ter = ter;
    }
    int num;
    char str[50];
    bool ter;
    bool can_derive_null;
    bool if_ter(int num2)
    {
        return ter;
    }
};

class Production
{
    public:
        Production();
        virtual ~Production();
        int insert_ter_ver(char* str);
        int insert_production(char* str);
        bool is_terminal(int num);
        int get_int_of_VT(char* str);
        set_of_items CLOSURE(set_of_items I);
        set_of_items GOTO(set_of_items I , int x);
        void items();
        int get_len(int i);
        int get_reduce(int i);
         char* get_str(int a);

        vector< vector<int> > list_action;
        vector< vector<int> > list_goto;
        vector< vector<int> > list_pro;
        map<int, int> map_ter;
        map<int, int> map_var;

    protected:
    private:



        int num;
        vector<set_of_items> list_set_of_items;
        vector<Ver_and_Ter> list_ver_ter;
        vector<First> list_first;
        int num_of_I;

        bool point_is_before_Var(item i);
        int find_after_point(item i);
        vector<int> first(int a, int b);
        vector<int>first(int a);
        int get_all_frist();
        bool is_after_point(int x , item i);
        int is_not_in_C(set_of_items set);
        bool item_is_in_set_of_items(item i, set_of_items set);
        bool is_null(int var);
        int insert_a_to_b(int a,int b);

        bool if_a_can_derive_null(int a);
        bool a_is_in_first(int a, First b);
        int creat_action();
        int find_pro_num(vector<int> pro);

};

#endif // PRODUCTION_H
