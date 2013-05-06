#include <iostream>
#include "Production.h"
#include <vector>
#include <string>
#include <stack>
#include <fstream>
#include <queue>
#include "inter_code.h"
using namespace std;


//before coding , think use which data structure and algorithm can express the program more confidently
int main()
{
    cout << "Hello world!" << endl;
    Production p;

    ifstream in;
    char str[100];
    in.open("out.txt");
    queue<node_lex> lex;

    while(in.getline(str,100))
    {
        int len=0;
        for(int i=0 ; str[i]!='\0'; i++)
        {
            len++;
        }
        string str_tmp;
        int sum=0;
        int j;
        for(j =0 ; str[j] != ' ' ; j++)
        {
            sum = sum*10 + str[j]-'0';
        }
        j++;
        str_tmp.assign(str,j,len-j);
        lex.push(node_lex(sum,str_tmp));
    }

    stack<int> state;
    vector<node_lex> symble;
    state.push(0);





    lex.push(node_lex(DOLLARS,string("")));


    while(1)
    {
        cout<<endl<<endl;
        int S=state.top();

        node_lex tmp_node_lex;
        tmp_node_lex.key=lex.front().key;
        tmp_node_lex.value=lex.front().value;
        int a=lex.front().key;
        map<int,int>::iterator p_ter;
            p_ter=p.map_ter.find(a);
        int i= p.list_action[S][p_ter->second];
        cout<<"S:"<<S<<"  "<<"a:"<<a<<endl;
        if(i == BEGIN)
        {
            cout<<"error"<<endl;
        }
        else if( i == ACC)
        {
            cout<<"right"<<endl;
            break;
        }
        else if(i > 0)
        {
           symble.push_back(tmp_node_lex);
           state.push(i);
           lex.pop();
           cout<<"shift"<<endl;
            cout<<"state_top:"<<state.top();
            cout<<"  symnle_top:"<<symble.back().key;
            cout<<"  input_top:"<<lex.front().key<<endl;
        }
        else if(i < 0)
        {
            node_lex tmp = action(-i,symble);
            int len;
            if(p.list_pro[-i][1] == my_null)
            {
                len = 0;
            }
            else
            {
                len = p.get_len(-i);
            }

            for(int j =0; j < len ; j++)
            {
                state.pop();
                symble.pop_back();
            }
            int now_state= state.top();


            map<int,int>::iterator p_var;
            p_var=p.map_var.find(p.get_reduce(-i));

            tmp.key = p.get_reduce(-i);

            symble.push_back(tmp);
            state.push(p.list_goto[now_state][p_var->second]);

            cout<<p.get_str(p.list_pro[-i][0])<<"->";
            for(int j=1 ; j < p.list_pro[-i].size() ; j++ )
            {
                cout<<p.get_str(p.list_pro[-i][j])<<" ";
            }
            cout<<endl;
            cout<<"reduce"<<endl;
            cout<<"state_top:"<<state.top();
            cout<<"  symnle_top:"<<symble.back().key;
            cout<<"  input_top:"<<lex.front().key<<endl;
        }
    }

    for(int k =0 ; k < inter_code.size() ; k++)
    {
         cout<<k<<": ";
        for(int p =0 ; p < inter_code.at(k).size() ; p++)
        {
            cout<<inter_code.at(k).at(p)<<" ";
        }
        cout<<endl;
    }
    return 0;
}
