#ifndef INTER_CODE_H_INCLUDED
#define INTER_CODE_H_INCLUDED
#include<string>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
int offset;

vector< vector<string> > inter_code;
//in a word,the complier techniques is about how to convert one language to another
//language ,no matter what means you use.
//in this program it's transform the c to  assemble

vector<int>* makelist(int i)
{
    vector<int> *list = new vector<int>;
    list->push_back(i);
    return list;
}

int backpatch(vector<int>* list,string label)
{
    for(int i =0 ; i < list->size() ; i++)
    {
       inter_code.at(list->at(i)).push_back(label);
    }
}
class node_symble
{
    public:
        node_symble(string name,string type, int offset)
        {
            this->name = name;
            this->type = type;
            this->offset = offset;
        }
        string name;
        string type;
        int offset;
        string op;
};

vector<node_symble>  symble_table;
class node_lex
{
    public:
    node_lex()
    {

    }
    node_lex(int key, string value)
    {

        this->key = key;
        this->value = value;
        offset = "";
    }
    vector<string> list_value;
    int key;//key number
    string value;//the string of key number
    int num;//num of id;
    int width;
    string type;
    string offset;
    string addr;//temp addr
    string op;
    string array;
    string quad;
    vector<int>* nextlist;
    vector<int>* truelist;
    vector<int>* falselist;

};
int num_temp=0;
string newtemp()
{
    num_temp++;
    char buffer[20];
    sprintf(buffer,"%s%d","t",num_temp);
    return string(buffer);
}
int enter(string name, string type, int offset)
{
    for(int i =0 ; i < symble_table.size() ; i++)
    {
        if(symble_table[i].name == name)
            return -1;
    }
    symble_table.push_back(node_symble(name,type,offset));
}

node_lex action(int i,vector<node_lex> symble)
{
    ofstream out;
    out.open("inter.txt",ios::app);
    node_lex tmp;
    char buffer[100];
    char str_num[30];
    vector<string> tmp_code;
    switch(i)
    {
        case 0:
            break;
        case 1:
            break;
        case 2:
            offset = 0;
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;

        case 8://init_decl_list = init_decl_list , init_declarator
            enter(symble.back().value,symble.at(symble.size()-3).type,offset);
            cout<<symble.at(symble.size()-3).type<<" "<<symble.back().value<<endl;
            offset = offset + symble.at(symble.size()-3).width;
            tmp.type =symble.at(symble.size()-3).type;
            tmp.width = symble.at(symble.size()-2).width;
            break;
        case 9://init_decl_list = type init_declarator
            enter(symble.back().value,symble.at(symble.size()-2).type,offset);
           cout<<symble.at(symble.size()-2).type<<" "<<symble.back().value<<endl;
           offset = offset + symble.at(symble.size()-2).width;
           tmp.type = symble.at(symble.size()-2).type;
           tmp.width = symble.at(symble.size()-2).width;
            break;
        case 10://type = int
            tmp.type="int";
            tmp.width = 4;
            break;
        case 11://type = float
            tmp.type = "float";
            tmp.width = 4;
            break;
        case 12://init_declarator = declarator
            tmp.key = symble.back().key;
            tmp.value = symble.back().value;
            break;
        case 13://declarator = id
            tmp.num = 1;
            tmp.value = symble.back().value;
            tmp.array = tmp.value;
            break;
        case 14://declarator = id [num]
            //why I use string in this point but it's not right
            cout<< symble.at(symble.size()-2).value<<endl;
            sprintf(buffer,"%s%s%s%s",symble.at(symble.size()-4).value.c_str(),"[", symble.at(symble.size()-2).value.c_str(),"]");
            tmp.num = atoi(symble.at(symble.size()-2).value.c_str());
            tmp.value = symble.at(symble.size()-4).value;
            tmp.array = buffer;
            break;
        case 15://declarator = id [ id ]
            tmp.num = symble.at(symble.size()-2).num;
            tmp.value = symble.at(symble.size()-4).value;
            sprintf(buffer,"%s%s%s%s",symble.at(symble.size()-4).value.c_str(),"[", symble.at(symble.size()-2).value.c_str(),"]");
            tmp.array = buffer;
            break;
        case 16://init_declarator = declarator = initializer
            if(symble.back().list_value.size() == 0)
            {
                tmp_code.push_back(symble.at(symble.size()-3).value);
                tmp_code.push_back("=");
                tmp_code.push_back(symble.back().value);

                inter_code.push_back(tmp_code);
                out<<symble.at(symble.size()-3).value<<"="<<symble.back().value<<endl;
                tmp.value = symble.at(symble.size()-3).value;
            }
            else
            {
                tmp.value = symble.at(symble.size()-3).value;
                tmp.list_value = symble.back().list_value;

                for(int i =0 ; i< tmp.list_value.size(); i++)
                {
                    tmp_code.clear();
                    tmp_code.push_back(tmp.value);
                    sprintf(buffer,"%s%d%s","[",i,"]");
                    tmp_code.push_back(buffer);
                    tmp_code.push_back("=");
                    tmp_code.push_back(tmp.list_value.at(i));
                    inter_code.push_back(tmp_code);
                    out<<tmp.value<<"["<<i<<"]"<<"="<<tmp.list_value.at(i)<<endl;
                }
            }
            break;
        case 17://initializer = num

            tmp.value = symble.back().value;
            cout<<"value "<<tmp.value<<endl;
            break;
        case 18://initializer = { initializer_list }
            tmp.list_value=symble.at(symble.size()-2).list_value;
            for(int i=0 ;i<tmp.list_value.size(); i++)
                cout<<tmp.list_value.at(i)<<endl;
            break;
        case 19://initializer_list = initializer
            tmp.list_value.push_back(symble.back().value);
            for(int i=0 ;i<tmp.list_value.size(); i++)
                cout<<tmp.list_value.at(i)<<endl;
            break;
        case 20://initializer_list = initializer , initializer_list
            tmp.list_value.push_back(symble.at(symble.size()-3).value);
            for(int i =0 ; i < symble.back().list_value.size(); i++)
            {
                tmp.list_value.push_back(symble.back().list_value[i]);
            }
            for(int i=0 ;i<tmp.list_value.size(); i++)
                cout<<tmp.list_value.at(i)<<endl;
            break;
        case 21://statement = expression_statement
            break;
        case 22://statement = if ( expression ) M2 statement ELSE statement
            backpatch(symble.at(symble.size()-6).truelist,symble.at(symble.size()-4).quad);
            backpatch(symble.at(symble.size()-6).falselist,symble.at(symble.size()-2).quad);
            sprintf(buffer,"%d",inter_code.size());
            backpatch(symble.at(symble.size()-2).nextlist,buffer);
            break;

        case 23://statement =  return num
            tmp_code.push_back("end");
            inter_code.push_back(tmp_code);
            out<<"end"<<endl;
            break;
        case 24://statement =  { statement_list }
            break;
        case 25://expression_statement = ;
            break;
        case 26://expression_statement = expression ;
            break;
        case 27://statement_list = statement
            break;
        case 28://tatement_list = statement statement_list
            break;
        case 29://expression = assignment_expression
            break;
        //the reason why I wrong is i forget to change the string to char*
        case 30://expression = id op num

            tmp.truelist = makelist(inter_code.size());
            tmp.falselist = makelist(inter_code.size()+1);
            tmp_code.push_back("if");
            sprintf(buffer,"%s%s%s",symble.at(symble.size()-3).value.c_str(),symble.at(symble.size()-2).op.c_str(),symble.back().value.c_str());
            tmp_code.push_back(buffer);
            tmp_code.push_back("goto");

            inter_code.push_back(tmp_code);

            tmp_code.clear();
            tmp_code.push_back("goto");
            inter_code.push_back(tmp_code);
            out<<"if"<<" "<<symble.at(symble.size()-3).value<<" "<<symble.at(symble.size()-2).op
            <<" "<<symble.back().value<<" "<<"goto -"<<endl;
            break;

        case 31://expression = id ++
            tmp_code.push_back(symble.at(symble.size()-2).value);
            tmp_code.push_back("=");
              tmp_code.push_back(symble.at(symble.size()-2).value);
                tmp_code.push_back("+");
                tmp_code.push_back("1");
                inter_code.push_back(tmp_code);
            out<<symble.at(symble.size()-2).value<<"="<<symble.at(symble.size()-2).value<<"+"<<"1"<<endl;
            break;
        case 32://assignment_expression = declarator = assignment_expression
            if(symble.at(symble.size()-3).offset == "")
            {
                tmp_code.push_back(symble.at(symble.size()-3).value);
                tmp_code.push_back("=");
                tmp_code.push_back(symble.back().addr);

                inter_code.push_back(tmp_code);
                out<<symble.at(symble.size()-3).value<<" "<<"= "<<symble.back().addr<<endl;
            }
            else
            {
                tmp_code.push_back(symble.at(symble.size()-3).array);
                tmp_code.push_back("=");
                tmp_code.push_back(symble.back().addr);

                inter_code.push_back(tmp_code);
                out<<symble.at(symble.size()-3).array<<" "
                <<"="<<" "<<symble.back().addr<<endl;
            }
            break;
        case 33://assignment_expression = unary_expression unary_op assignment_expression
            tmp.addr=newtemp();
            tmp.array= tmp.addr;

            tmp_code.push_back(tmp.addr);
            tmp_code.push_back("=");
            tmp_code.push_back(symble.at(symble.size()-3).array);
            tmp_code.push_back(symble.at(symble.size()-2).op);
            tmp_code.push_back(symble.back().array);

            inter_code.push_back(tmp_code);
            out<<tmp.addr<<" "<<"="<<" "<<symble.at(symble.size()-3).array<<" "<<symble.at(symble.size()-2).op<<
            " "<<symble.back().array<<endl;
            break;
        case 34://assignment_expression = num
            tmp.addr=newtemp();
            tmp.array=symble.back().value;

            tmp_code.push_back(tmp.addr);
            tmp_code.push_back("=");
            tmp_code.push_back(tmp.array);
            inter_code.push_back(tmp_code);
            out<<tmp.addr<<" "<<"="<<" "<<tmp.array<<endl;
            break;
        case 35://assignment_expression = declarator
            tmp.array = symble.back().array;

            break;
        case 36://unary_expression = declarator
            tmp.array = symble.back().array;
            break;
        case 37://unary_expression = num
            tmp.value=symble.back().value;
            tmp.array =tmp.value;
            break;
        case 38://op = <
            tmp.op="<";
            break;
        case 39://op = >=
            tmp.op=">=";
            break;
        case 40://unary_op = +
            tmp.op="+";
            break;
        case 41://unary_op = -
            tmp.op="-";
            break;
        case 42://unary_op = *
            tmp.op="*";
            break;
        case 43://unary_op = /
            tmp.op="/";
            break;
        case 44://M2 = my_null
            sprintf(buffer,"%d",inter_code.size());
            tmp.quad = buffer;
            break;
        case 45://ELSE = else
            tmp.nextlist = makelist(inter_code.size());
            tmp_code.push_back("goto");
            inter_code.push_back(tmp_code);
            sprintf(buffer,"%d",inter_code.size());
            tmp.quad = buffer;
            break;
        case 46://statement = for ( expression ; expression ; expression ) M3 statement
            backpatch(symble.at(symble.size()-6).truelist,symble.at(symble.size()-2).quad);


            sprintf(buffer,"%d",   atoi(symble.at(symble.size()-2).quad.c_str())-2);

            tmp_code.push_back("goto");
            tmp_code.push_back(buffer);
            inter_code.push_back(tmp_code);
            sprintf(buffer,"%d",inter_code.size());
            backpatch(symble.at(symble.size()-6).falselist,buffer);

            break;
        case 47://M3 = my_null
            sprintf(buffer,"%d",inter_code.size()-3);
            tmp_code.push_back("goto");
            tmp_code.push_back(buffer);
            inter_code.push_back(tmp_code);
            sprintf(buffer,"%d",inter_code.size());
            tmp.quad = buffer;
            break;

    }
    out.close();
    return tmp;
}


#endif // INTER_CODE_H_INCLUDED
