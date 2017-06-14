#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
void main(){
    ifstream fin("init_synctable.sql"); //打开文件流操作
	ofstream out("result.h",ios::app);
	string line;
	string line2;
	string line3;
	vector<string> vTable;
	vector<string> vColumn;
	//vector<string> vec[50];
	map <string,string> mTable;
    while (getline(fin, line))   //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取  
    {
		if (line.compare(0,33,"insert into kt_synctable values (") == 0){
			int pos = line.find(",");
			
			string id = line.substr(33,pos-32-1);
			int pos2 = line.find("\'");
			int pos3 = line.rfind("\'");
			string tablename = line.substr(pos2+1,pos3-pos2-1);
	    	mTable.insert(make_pair(id,tablename));
		}
	}

	
    map<string, string>::iterator it;
// 	for (it = mTable.begin(); it != mTable.end(); it++){
// 		ifstream fin2("kamt.sql");
// 		int flag = 0;
// 		whlie(getline(fin2,line2)){
// 			if(line2.compare(0,12,"CREATE TABLE ")){
// 				flag = 1;
// 			}
// 			else if(flag == 1){
// 				int pos = line.find(",");
// 				string str = line.substr(3,pos-1);
// 			}
// 		}
// 		vec[it->first.parseInt()]
// 	}
	for (it = mTable.begin(); it != mTable.end(); it++){
		vColumn.clear();
		string tablename = it->second;
    	ifstream fin2("kamt.sql");
		while(getline(fin2,line2)){
			char tt[1024];
			sprintf(tt,"CREATE TABLE %s",tablename.c_str());
			if (line2.find(tt) != std::string::npos){
				while(getline(fin2,line3)){
					int pos = line3.find(' ',3);
					string column;
					column = line3.substr(3,pos-3);
					if (column != "CONSTRAINT"){
						vColumn.push_back(column);
					}
					else{
					  break;
					}
				}
			break;
			}
		}

		char head[4096];
		sprintf(head,"CREATE OR REPLACE TRIGGER TR_%s BEFORE UPDATE OR INSERT ON %s FOR EACH ROW\n"
				     "declare\n"
			         "v_str varchar(32766);\n"
			         "BEGIN\n"
					 ":NEW.SYNCSNO := SS_%s.NEXTVAL;\n",tablename.c_str(),tablename.c_str(),tablename.c_str());
		out << head;

		int i = 0;
		cout<< "vColumn -->"<<vColumn.size() <<endl;
		while(i != vColumn.size()){
			char str[1024];
			if (i == 0)
		    	out << "v_str := "; 
			if (i != vColumn.size()-1)
			  sprintf(str," \'%s=\' || to_char(:new.%s) || \'|\' ||\n",vColumn[i].c_str(),vColumn[i].c_str());
			else sprintf(str," \'%s=\' || to_char(:new.%s);\n",vColumn[i].c_str(),vColumn[i].c_str());
			out << str;
			i++;
		}

		char str2[1024];
		strcpy(str2, "if inserting then\n");
		out << str2;
		sprintf(str2,"    insert into kt_synclog(SYNCSNO,TABLEID,ACTION,SYNCSTR) values(:NEW.SYNCSNO,%s,\'i\',v_str);\n",(it->first).c_str());
		out << str2;
		strcpy(str2,"elsif updating then\n");
		out << str2;
		sprintf(str2,"    insert into kt_synclog(SYNCSNO,TABLEID,ACTION,SYNCSTR) values(:NEW.SYNCSNO,%s,\'u\',v_str);\n",(it->first).c_str());
		out << str2;
		strcpy(str2,"end if;\n"
	           "END;\n");
		out << str2;
		out << "/\n";
	}
}
