#include "Production.h"
#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
int map_goto[MAX][MAX];

Production::Production()
{
    list_ver_ter.push_back(Ver_and_Ter(-3,"$",true));
    list_ver_ter.push_back(Ver_and_Ter(-2,".",true));
    list_ver_ter.push_back(Ver_and_Ter(-4,"my_null",true));
    ifstream input;
    input.open("a2.txt");
    char str[50];
    input.getline(str,50,'\n');
    while(!input.eof())
    {
        if(str[0]!='%')
        {
            insert_ter_ver(str);
        }
        else
        {
            break;
        }
        input.getline(str,50,'\n');

    }

    input.getline(str,100,'\n');
    while(!input.eof())
    {
        if(str[0] == ' ')
            continue;
        insert_production(str);
        input.getline(str,100,'\n');
        if(str[0] == '#')
            break ;
    }
    cout<<"size of pro:"<<list_pro.size()<<endl;
    for(int i =0; i< list_pro.size();i++)
    {
        cout<<endl;
        for(int j =0 ; j< list_pro[i].size(); j++)
        {
            cout<<get_str(list_pro[i][j])<<" ";
        }
    }
    input.close();
    //creat items
    items();
    //creat tables
    creat_action();

    //ctor
}

//insert production
int Production::insert_production(char* str)
{
    char left_str[50];
    char temp_str[50];
    int i=0;
    int j;
    for(j=0; j<50 && str[i] != ' ' && str[i] != '='; j++, i++)
    {
        left_str[j] = str[i];
    }

    left_str[j] = '\0';
    int num = get_int_of_VT(left_str);
    vector<int> *t = new vector<int>();
    list_pro.push_back(*t);
    list_pro[list_pro.size()-1].push_back(num);
    cout<<num<<"=";

    if(str[i] == '=')
    {
        while(str[++i] == ' ')
            ;
    }
    else if(str[i] == ' ')
    {
        while(str[i++] != '=')
            ;
        while(str[++i] == ' ')
            ;
    }

    while(str[i] != '\0')
    {
        //½ØÈ¡×Ö·û´®

        for(j = 0; str[i] != ' '&&str[i] != '\0' ; j++,i++)
        {
            temp_str[j]=str[i];
        }
        temp_str[j]='\0';

        num=get_int_of_VT(temp_str);

        list_pro[list_pro.size()-1].push_back(num);

        cout<<num<<" ";
        if(str[i] == '\0')
        {
            cout<<endl;
            return 0;

        }
        i++;
    }
    cout<<endl;
}
// get the int number of varible or terminal
int Production::get_int_of_VT(char* str)
{
    for(int i=0; i<list_ver_ter.size(); i++)
    {
        if(!strcmp(str,list_ver_ter[i].str))
        {
            return list_ver_ter[i].num;
        }
    }
}
//insert the syble to the list
int Production::insert_ter_ver(char* str)
{

    bool if_ter;
    int num=0;
    char str2[50];
    int i=4;
    int j;

    if(str[0] == 'V')
    {
        if_ter = false;
    }
    else
    {
        if_ter = true;
    }
    while(str[i] == ' ')
    {
        i++;
    }

    for( j=0 ; str[i] != ' ' ; i++,j++)
    {
        str2[j] = str[i];
    }
    str2[j] = '\0';
    i++;
    while(str[i] == ' ')
    {
        i++;
    }
    while(str[i] != '\0')
    {
        num = num * 10 + str[i]-'0';
        i++;
    }

    cout<<str2<<" "<<num<<endl;
    Ver_and_Ter *tmp = new Ver_and_Ter(num,str2,if_ter);
    list_ver_ter.push_back(*tmp);
}

set_of_items Production::CLOSURE(set_of_items I)
{
    for(int num = 0; num < I.size() ; num ++)
    {

        item p = I.get(num);
        if(point_is_before_Var(p))
            for(int i =0 ; i < list_pro.size() ; i ++ )
            {
            /*     cout<<endl;
            for(int j =0 ; j< list_pro[i].size(); j++)
            {
                cout<<get_str(list_pro[i][j])<<" ";
            }*/
                if(find_after_point(p) == list_pro[i][0])
                {
                    int alpha;
                    for(int j =0 ; j < p.pro.size() ; j++  )
                    {
                        if(p.pro[j] ==  POINT)
                        {
                            if(j == p.pro.size()-2 )
                                alpha = my_null;
                            else
                                alpha = p.pro[j+2];
                            break;
                        }
                    }
                    vector<int> follow = first(alpha,p.follow);

                    for(int q =0 ; q < follow.size() ; q++)
                    {
                        vector<int> temp_pro;
                        temp_pro.push_back(list_pro[i][0]);
                        temp_pro.push_back(POINT);

                        if(list_pro[i][1] != my_null)
                        for(int m=1 ; m < list_pro[i].size() ; m++)
                        {
                            temp_pro.push_back(list_pro[i][m]);
                        }
                        //do nothing   conrespond to A->null


                        item tmp_item(temp_pro,follow[q]);

                        if(!item_is_in_set_of_items(tmp_item, I))
                            I.insert(tmp_item);
                    }
                }
            }
    }

    return I;
}

set_of_items Production::GOTO(set_of_items I, int x)
{
    set_of_items J ;
    for(int i=0 ; i < I.size() ; i++)
    {
        if(is_after_point(x,I.get(i)))
        {
            vector<int> temp_pro;
            temp_pro.push_back(I.get(i).pro[0]);
            int p;

            for(p = 1; p < I.get(i).pro.size(); p++)
            {
                if(I.get(i).pro[p] != POINT)
                {
                    temp_pro.push_back(I.get(i).pro[p]);
                }
                else
                    break;
            }
            temp_pro.push_back(x);
            temp_pro.push_back(POINT);

            for(int q=p+2 ; q < I.get(i).pro.size() ; q++)
            {
                temp_pro.push_back(I.get(i).pro[q]);
            }
            item temp_item(temp_pro,I.get(i).follow);
            J.insert(temp_item);
        }
    }
    return CLOSURE(J);
}
void Production::items()
{
    //initialize the GOTO function
    for(int i =0 ; i < MAX ; i++)
    {
        for( int j =0 ; j < MAX ; j++)
        {
            map_goto[i][j] = BEGIN;
        }
    }
    get_all_frist();

    vector<int> temp_pro;
    temp_pro.push_back(list_pro[0][0]);
    temp_pro.push_back(POINT);

    temp_pro.push_back(list_pro[0][1]);
    item tmp(temp_pro,DOLLARS);
    set_of_items start_set;
    start_set.insert(tmp);

    num_of_I = 0;


    list_set_of_items.push_back(CLOSURE(start_set));
    set_of_items tmp_goto2=CLOSURE((start_set));
    cout<<"I:"<<0<<endl;
    num_of_I++;


    for(int m=0; m<tmp_goto2.size(); m++)
    {

        cout<<"Pro"<<endl;
        for(int n=0; n<tmp_goto2.get(m).size(); n++)
        {
            cout<<get_str(tmp_goto2.get(m).pro[n])<<" ";
            if(n == 0)
                cout<<"->";
        }
        cout<<" ,"<<get_str(tmp_goto2.get(m).follow)<<endl;
    }

    for(int i=0; i < list_set_of_items.size() ; i++)
    {
        for(int j=0; j < list_ver_ter.size() ; j++)
        {
            set_of_items tmp_goto=GOTO(list_set_of_items[i],list_ver_ter[j].num);

            if(tmp_goto.size() != 0 )
            {
                int I_num=is_not_in_C(tmp_goto);
                if(I_num == -1)
                {
                    map_goto[i][list_ver_ter[j].num]=list_set_of_items.size();
                    list_set_of_items.push_back(tmp_goto);

                    /*cout<<"I:"<<list_set_of_items.size()-1<<endl;
                    for(int m=0; m<tmp_goto.size(); m++)
                    {

                        cout<<"Pro"<<endl;
                        for(int n=0; n<tmp_goto.get(m).size(); n++)
                        {
                            cout<<get_str(tmp_goto.get(m).pro[n])<<" ";
                            if(n == 0)
                                cout<<"->";
                        }
                        cout<<" ,"<<get_str(tmp_goto.get(m).follow)<<endl;
                    }*/


                }
                else
                {
                    map_goto[i][list_ver_ter[j].num] = I_num;
                }
            }
        }
    }
    return;
}
//judge whether the set of items in the C
int Production::is_not_in_C(set_of_items set)
{
    int k;
    for(int i=0; i < list_set_of_items.size() ; i++)
    {
        for( k = 0; k < set.size() ; k++)
        {
            if(item_is_in_set_of_items(set.get(k),list_set_of_items[i]))
            {
                continue;
            }
            else
            {
                break;
            }
        }
        if( k == set.size() && (set.size() == list_set_of_items[i].size()))
            return i;
    }
    return -1;
}
//whether a set in the items
bool Production::item_is_in_set_of_items(item i , set_of_items set)
{
    int k;
    for(int j =0; j < set.size(); j++)
    {
        for( k =0 ; k < set.get(j).size() && k < i.size(); k++)
        {
            if(i.pro[k] == set.get(j).pro[k])
                continue;
            else
            {
                break;
            }
        }
        if(k == i.size() && i.follow == set.get(j).follow && k == set.get(j).size())
            return true;
    }
    return false;
}
bool Production::is_after_point(int x , item i)
{
    if(x == find_after_point(i))
    {
        return true;
    }
    else
        return false;
}
vector<int> Production::first(int a, int b)
{
    if(a == my_null)
    {
        return first(b);
    }
    else
    {
        First temp(0,first(a));
        if(is_null(a))
        {
            int k=0;

            for(k=0 ; k < list_first.size() ; k++)
            {
                if(list_first[k].x == b)
                    break;
            }
            for(int i =0 ; i < list_first[k].set.size(); i++)
            {

               // cout<<list_first[k].set[i]<<endl;
                int f = list_first[k].set[i];
                if(  ! a_is_in_first( f , temp))
                {
                    temp.set.push_back(f);
                }
            }
        }
        return temp.set;
    }
    /*
    if(a == my_null)
        return first(b);
    else
        return first(a);
        */
}

bool Production::point_is_before_Var(item i)
{
    if(i.pro.size() == 2)
    {
        return false;
    }
    if(i.pro[i.pro.size()-1] != POINT)
        for(int p = 0; p <i.pro.size() ; p++)
        {
            if(i.pro[p] == POINT)
            {
                if(!is_terminal(i.pro[p+1]))
                {
                    return true;
                }
                else
                    return false;
            }

        }
    return false;
}

int Production::find_after_point(item i)
{
    for(int j =0 ; j< i.pro.size() ; j++)
    {
        if(i.pro[j] == POINT)
        {
            if(j+1 < i.pro.size())
            {
                return i.pro[j+1];
            }
            else
                return -1;
        }
    }
    return -1;
}

vector<int> Production::first(int a)
{
    for(int i=0 ; i < list_first.size() ; i++)
    {
        if(list_first[i].x == a)
        {
            return list_first[i].set;
        }
    }

}
bool Production::is_terminal(int num2)
{
    for(int i=0 ; i < list_ver_ter.size() ; i++)
    {
        if(list_ver_ter[i].num == num2)
            return list_ver_ter[i].ter;
    }
    return false;
}
int Production::get_all_frist()
{
    for(int i=0 ; i < list_ver_ter.size() ; i++)
    {
        if(if_a_can_derive_null((list_ver_ter.at(i)).num))
        {
            list_ver_ter[i].can_derive_null=true;
            cout<<list_ver_ter[i].num<<"->"<<"NULL"<<endl;
        }
        else
            list_ver_ter[i].can_derive_null=false;
    }
    for(int i=0; i < list_ver_ter.size() ; i++)
    {
        vector<int> tmp;
        if(is_terminal(list_ver_ter[i].num))
        {
            tmp.push_back(list_ver_ter[i].num);
            First ter_first(list_ver_ter[i].num,tmp);
            list_first.push_back(ter_first);
        }
        else
        {
            tmp.clear();
            list_first.push_back(First(list_ver_ter[i].num,tmp));
        }
    }

    int j;
    int num_changed=0;

    while(1)
    {
        for(int i=0; i < list_pro.size(); i++)
        {
            for( j=1; j < list_pro[i].size() ; j++)
            {
                if(list_pro[i][j] == list_pro[i][0])
                    continue;
                num_changed+=insert_a_to_b(list_pro[i][j],list_pro[i][0]);

                if(!is_null(list_pro[i][j]))
                {
                    break;
                }
                else
                {
                    continue;
                }
            }

            if(j == list_pro[i].size())
            {
                num_changed+=insert_a_to_b(my_null,list_pro[i][0]);
            }
        }
        if(num_changed == 0)
            break;
        else
        {
            num_changed=0;
            continue;
        }
    }

    cout<<"first:"<<endl;
    for(int i=0 ; i < list_first.size(); i++)
    {
        cout<<get_str(list_first[i].x)<<": ";
        for(int num =0 ; num < list_first[i].set.size(); num ++)
        {
            cout<<get_str(list_first[i].set[num])<<" ";
        }
        cout<<endl;
    }
}
//return the number of elements from F(a) to insert to F(b)
//if a is my_null then add null to first(b)
//else add no my_null to first(b)
int Production::insert_a_to_b(int a,int b)
{
    int i;
    int j;
    int num=0;
    for( i=0 ; i < list_first.size() ; i++)
    {
        if(list_first[i].x == a)
            break;
    }
    for( j=0 ; j <list_first.size() ; j++)
    {
        if(list_first[j].x == b)
            break;
    }

    if(a == my_null)
    {
        if(!a_is_in_first(my_null,list_first[j]))
        {
            list_first[j].set.push_back(my_null);
            return 1;
        }
        else
            return 0;

    }
    else
    {
        for(int k =0 ; k <list_first[i].set.size() ; k++)
        {

            if( ( !a_is_in_first(list_first[i].set[k] , list_first[j]) ) && (list_first[i].set[k] != my_null) )
            {
                list_first[j].set.push_back(list_first[i].set[k]);
                num++;
            }

        }
        return num;
    }
}
bool Production::a_is_in_first(int a, First b)
{
    for(int i = 0 ; i < b.set.size(); i++)
    {
        if(a == b.set[i])
            return true;
    }
    return false;
}
//whether var can derive null
bool Production::is_null(int var)
{
    for(int i =0; i < list_ver_ter.size() ; i++)
    {
        if(list_ver_ter[i].num == var)
        {
            return list_ver_ter[i].can_derive_null;
        }
    }
    return false;
}
char* Production::get_str(int a)
{
    for(int i =0; i < list_ver_ter.size(); i++)
    {
        if(list_ver_ter[i].num == a)
            return list_ver_ter[i].str;
    }
    return "abc";
}
// if the production don't have the left recursion , depth first is ok.
//but when have left recursion , it needs wide first or a symble table to show whether a production have be
// used
bool Production::if_a_can_derive_null(int a)
{
    if(a == my_null)
        return true;
    if(is_terminal(a))
        return false;
    for(int i =0; i <list_pro.size() ; i++)
    {
        if(list_pro[i][0] == a && list_pro[i][1] != a)
        {
            int j;
            for( j =1 ; j <list_pro[i].size() ; j++)
            {
                if(if_a_can_derive_null(list_pro[i][j]))
                    continue;
                else
                    break;
            }

            if(j == list_pro[i].size())
                return true;
        }
    }
    return false;
}

int Production::creat_action()
{


    int num_var=0;
    int num_ter=0;
    for(int i =0 ; i < list_ver_ter.size() ; i++)
    {
        if(list_ver_ter[i].ter == true)
        {
            map_ter.insert(map<int, int>::value_type(list_ver_ter[i].num , num_ter));
            num_ter++;
        }
        else
        {
            map_var.insert( pair<int, int>(list_ver_ter[i].num, num_var) );
            num_var++;
        }
    }

    vector<int> tmp_action(num_ter+1 , BEGIN);
    vector<int> tmp_goto(num_var+1 , BEGIN);

    for(int i =0 ; i < list_set_of_items.size() ; i++)
    {
        list_action.push_back(tmp_action);
        list_goto.push_back(tmp_goto);
    }
    for(int i =0 ; i < list_set_of_items.size(); i++)
    {
        for( int j =0 ; j < list_set_of_items[i].size() ; j++)
        {

            int after=find_after_point(list_set_of_items[i].get(j));
            map<int,int>::iterator p;


            if(list_set_of_items[i].get(j).pro[1] == POINT && list_set_of_items[i].get(j).size() == 2)
            {

                p=map_ter.find(list_set_of_items[i].get(j).follow);

                vector<int> tmp_pro = list_set_of_items[i].list_item[j].pro;
                    tmp_pro.pop_back();
                    tmp_pro.push_back(my_null);

                list_action[i][p->second] = -find_pro_num(tmp_pro);
                //cout<<"list_action "<<i<<" "<<p->second<<":"<<-find_pro_num(tmp_pro)<<endl;
                continue;
            }


            if(is_terminal(after))
            {

                p=map_ter.find(after);

                //cout<<list_action[i][p->second]<<endl<< map_goto[i][after]<<endl;
                if( (list_action[i][p->second] != BEGIN) && (list_action[i][p->second] !=  map_goto[i][after] ))
                    exit(-1);
                list_action[i][p->second] = map_goto[i][after];
            }
            else if(after == -1)
            {
                p=map_ter.find(list_set_of_items[i].get(j).follow);
                string P(get_str(list_set_of_items[i].get(j).pro[0]));

                if( P.compare(string(list_ver_ter[3].str)) )
                {

                    vector<int> tmp_pro = list_set_of_items[i].list_item[j].pro;
                    tmp_pro.pop_back();

                    if(list_action[i][p->second] != BEGIN && list_action[i][p->second] != -find_pro_num(tmp_pro) )
                        {
                            cout<<"error 2"<<endl;
                            exit(-2);
                        }


                    //(list_set_of_items[i].list_item[j].pro).pop_back();
                    list_action[i][p->second] = -find_pro_num(tmp_pro);
                }
                else
                {
                    list_action[i][p->second] = ACC;
                }
            }
        }

    }

    for(int i =0 ; i < list_set_of_items.size() ; i++)
    {
        for(int j =0 ; j < list_ver_ter.size() ; j++)
        {
            if( !is_terminal(list_ver_ter[j].num) )
            {

                map<int,int>::iterator p;
                p=map_var.find(list_ver_ter[j].num);

                if( list_goto[i][p->second] != BEGIN && list_goto[i][p->second] != map_goto[i][list_ver_ter[j].num])
                    {
                        cout<<"error 3"<<endl;
                        exit(-3);
                    }
                if((map_goto[i][list_ver_ter[j].num]) != BEGIN && (!is_terminal(list_ver_ter[j].num)))
                {
                    list_goto[i][p->second] = map_goto[i][list_ver_ter[j].num];
                }

            }
        }
    }


    /*for(int i =0; i < list_set_of_items.size() ; i++)
    {
        for(int j =0 ; j < list_action[0].size() ; j++)
        {
            cout<<list_action[i][j]<<"  ";
        }
        cout<<endl;
    }*/

    /*for(int i =0; i < list_set_of_items.size() ; i++)
    {
        cout<<i<<" ";
        for(int j =0 ; j < list_goto[0].size() ; j++)
        {
            cout<<list_goto[i][j]<<"  ";
        }
        cout<<endl;
    }*/

}
int Production::find_pro_num(vector<int> pro)
{
    for(int i =0 ; i < list_pro.size(); i++)
    {
        int k;
        for( k = 0 ; k <list_pro[i].size() && k < pro.size(); k++)
        {
            if(list_pro[i][k] == pro[k])
                continue;
            else
                break;
        }
        if(k == list_pro[i].size() && k == pro.size())
            return i;
    }
    return -1;
}

int Production::get_len(int i)
{
    return list_pro[i].size()-1;
}

int Production::get_reduce(int i)
{
    return list_pro[i][0];
}

Production::~Production()
{
    //dtor
}
