#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

const char * fillwithspace(string *str,const int &len){
	static char cStr[2048];
	*str = string("\'")+(*str)+string("\',");
	while(len != str->length()){
		str->append(" ");
	}
	strcpy(cStr,str->c_str());
	return cStr;
}
string Trim(string& str)  
{   
    str.erase(0,str.find_first_not_of(" \t\r\n"));  
    str.erase(str.find_last_not_of(" \t\r\n") + 1);  
    return str;  
}  

int main()  
{
	int num = 0;
	vector<string> vTable;
	vector<string> vTable2;
	int pos;
	int pos2;
    ifstream fin("kamm.csv"); //打开文件流操作
    string line;   
    while (getline(fin, line))   //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取  
    {   
        istringstream sin(line); //将整行字符串line读入到字符串流istringstream中  
        vector<string> fields; //声明一个字符串向量 
		
        string field;  
        while (getline(sin, field, ',')) //将字符串流sin中的字符读入到field字符串中，以逗号为分隔符  
        {  
            fields.push_back(field); //将刚刚读取的字符串添加到向量fields中
        }  
        string name = Trim(fields[0]); //清除掉向量fields中第一个元素的无效字符，并赋值给变量name  
        string tablename = Trim(fields[1]); //清除掉向量fields中第二个元素的无效字符，并赋值给变量age   
		vTable.push_back(tablename);
        cout <<num++<<"-------->"<<"tablename--->"<< tablename << "\t"<<endl;   
    }
	cout<<"vTable.size()---->"<<vTable.size()<<endl;

	num = 0;
	ifstream fin2("init_memcachetable.sql");
	ofstream out("init_memcachetable.sql",ios::app);
	string line2;
	while (getline(fin2,line2)){
		string str(line2);
		pos = str.find("\(\'");
		if (pos != -1){
			pos2 = str.find("\',");
			if (pos2 > pos){
				string tbname = str.substr(pos+2,pos2-pos-2);
				vTable2.push_back(tbname);
				cout << num++ <<"---->" << "tbname--->"<<tbname<<endl;
			}
		}
	}
	cout<<"vTable2.size()---->"<<vTable2.size()<<endl;

	vector<string> vDouble;
	num = 1;
	vector<string>::iterator it  = vTable.begin();
	vector<string>::iterator it2 = vTable2.begin();
	while (it != vTable.end()){
		it2 = find(vTable2.begin(),vTable2.end(),*it);
		if (it2 == vTable2.end()){
			char sql[2048];
			sprintf(sql,"insert into memcachetable values \(%s%s0,              \'1\'\)\;\n",fillwithspace(it,33),fillwithspace(it,54));
			cout<<num++<<"."<<sql<<endl;
			out << sql;
		}
		else{
			vDouble.push_back(*it);
		}
		it++;
	}

	out.close();
	fin.close();
	fin2.close();
    return EXIT_SUCCESS;  
}  
