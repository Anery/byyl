#include <stdio.h>  
#include <stdlib.h>  
#include <iostream>  
#include <string>  
#include <vector>  
#include <algorithm> 
#include <iomanip>
using namespace std;

struct Token{
	string type;
	int poin;
	Token(string type, int p){
		this->type = type;
		this->poin = p;
	}
};

vector<int> consTable;   //常量表
vector<string> idTable;   //标识符表
int error = 0;            //记录错误
int line = 1;          //第几行

string Append(string a, char b){  //拼单词
	a += b;
	return a;
};

int KeyWord(string str){           //查保留字表
	if (str == "integer")
		return 1;
	if (str == "char")
		return 1;
	if (str == "program")
		return 1;
	if (str == "array")
		return 1;
	if (str == "of")
		return 1;
	if (str == "record")
		return 1;
	if (str == "end")
		return 1;
	if (str == "var")
		return 1;
	if (str == "procedure")
		return 1;
	if (str == "begin")
		return 1;
	if (str == "if")
		return 1;
	if (str == "then")
		return 1;
	if (str == "else")
		return 1;
	if (str == "fi")
		return 1;
	if (str == "while")
		return 1;
	if (str == "do")
		return 1;
	if (str == "endwh")
		return 1;
	if (str == "read")
		return 1;
	if (str == "write")
		return 1;
	if (str == "return")
		return 1;
	if (str == "type")
		return 1;
	if (str == "repeat")
		return 1;
	return 0;
}

int AddIdTable(string s){                  //返回值是poin指针
	for (int i = 0; i < idTable.size(); i++){
		if (s == idTable[i])
			return i;
	}
	idTable.push_back(s);
	return idTable.size() - 1;
}
int AddConsTable(string s){
	int number = atoi(s.c_str());
	for (int i = 0; i < consTable.size(); i++){
		if (number == consTable[i])
			return i;
	}
	consTable.push_back(number);
	return idTable.size() - 1;
}

Token *scanner(FILE *file){
	char current = 0;
	string info = "";
LS0:                         //总控制，从输入的第一个字符判断
	current = fgetc(file);

	if (current != EOF){
		if ((current >= 'A' &&current <= 'Z') || (current >= 'a' && current <= 'z'))
			goto LS1;
		if (current >= '0' &&current <= '9')
			goto LS2;
		if (current == '+')
			goto LS3;
		if (current == '-')
			goto LS4;
		if (current == '*')
			goto LS5;
		if (current == '/')
			goto LS6;
		if (current == '<')
			goto LS7;
		if (current == ';')
			goto LS8;
		if (current == ':')
			goto LS9;
		if (current == ',')
			goto LS10;
		if (current == '.')
			goto LS11;
		if (current == '=')
			goto LS12;
		if (current == '[')
			goto LS13;
		if (current == ']')
			goto LS14;
		if (current == '(')
			goto LS15;
		if (current == ')')
			goto LS16;
		if (current == ' ' || current == '\n' || current == '\r' || current == '\t')
			goto LS17;
		if (current == '{')
			goto LS18;
		if (current == '\'')
			goto LS19;

		 goto LS20;
	}
	return NULL;
LS1:             //字符串

	info = Append(info, current);

	current = fgetc(file);

	if ((current >= 'A' &&current <= 'Z') || (current >= 'a' && current <= 'z') || (current >= '0' &&current <= '9'))
		goto LS1;
	if (!((current >= 'A' &&current <= 'Z') || (current >= 'a' && current <= 'z') || (current >= '0' &&current <= '9'))){
		ungetc(current, file);
		int flag = KeyWord(info);
		if (flag == 1)
		{
			Token *token = new Token("$" + info, -1);
			printf("(%-10s, \"  \" )\n", token->type.c_str());   //边处理边打印 可以打印到出错位置
			return token;
		}
		else
		{
			Token *token = new Token("$id", AddIdTable(info));
			printf("(%-10s, [%2d] )\n", token->type.c_str(), token->poin);
			return token;
		}

	}

LS2:                  //数字
	info = Append(info, current);
	current = fgetc(file);
	if (current >= '0' && current <= '9')
		goto LS2;
	else{
		ungetc(current, file);
		Token *token = new Token("$INTC", AddConsTable(info));
		printf("(%-10s, [%2d] )\n", token->type.c_str(), token->poin);
		return token;
	}

LS3:                  //'+'  
	{
		Token *token = new Token("$+", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS4:				  //'-'  
	{
		Token *token = new Token("$-", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS5:				//'*'  
	{
		Token *token = new Token("$*", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS6:				//'/'  
	{
		Token *token = new Token("$/", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS7:				//'<'  
	{
		Token *token = new Token("$<", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS8:				//';'  
	{
		Token *token = new Token("$;", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS9:				//':'  
	{
		current = fgetc(file);
		if (current == '=')
		{
			Token *token = new Token("$:=", -1);
			printf("(%-10s, \"  \" )\n", token->type.c_str());
			return token;
		}
		else
		{
			error++;
			printf("错误:第 %d 行 “:”不是合法的符号,请检查是否缺少‘=’号\n",line);
			ungetc(current, file);

			Token *token = new Token("$:=", -1);                     //跳过错误，也生成:=符
			printf("(%-10s, \"  \" )\n", token->type.c_str());
			return token;

			goto LS0;
			//********************************************************加错误处理！
		}
	}
LS10:				//','  
	{
		Token *token = new Token("$,", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS11:				//'.'  
	{
		if ((current = fgetc(file)) == '.')
		{
			Token *token = new Token("$..", -1);
			printf("(%-10s, \"  \" )\n", token->type.c_str());
			return token;
		}
		else if ((current = fgetc(file)) == EOF){
			ungetc(current, file);
			Token *token = new Token("$#", -1);
			printf("(%-10s, \"  \" )\n", token->type.c_str());
			return token;

		}
		else
		{
			ungetc(current, file);
			Token *token = new Token("$.", -1);
			printf("(%-10s, \"  \" )\n", token->type.c_str());
			return token;
		}

	}
LS12:				//'='  
	{
		Token *token = new Token("$=", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS13:				//'['  
	{
		Token *token = new Token("$[", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS14:				//']'  
	{
		Token *token = new Token("$]", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS15:				//'('  
	{
		Token *token = new Token("$(", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS16:				//')'  
	{
		Token *token = new Token("$)", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS17:				//空白符,直接跳过
	{
		if (current == '\n')
			line++;
		goto LS0;
	}

LS18:              //注释
	{
		while ((current = fgetc(file)) != '}');
		current = fgetc(file);//把}读掉
		goto LS0;
	}

LS19:              //'
	{
		Token *token = new Token("$'", -1);
		printf("(%-10s, \"  \" )\n", token->type.c_str());
		return token;
	}
LS20:				//其他，非法  
	{
		error++;
		printf("错误：第 %d 行 含有非法符号( %c )\n", line,current);//打印错误信息  
		goto LS0;
		////********************************************************************错误处理
	}
}



class Bds{     //表达式结点 left->right
public:
	string left;
	vector<string> right;
};

class Gram{
protected:
	Bds bds[105];   //预定义105个产生式
	vector<string> follow[69];      //follow集
	vector<string> predict[105];    //predict集

	vector<string> analy; //分析栈
	vector<Token> input;//剩余输入串
	vector<string> act; // 动作

	int table[100][100];    //预测分析表:横坐标为非终结符，纵坐标为终结符标号

public:
	static string final[];
	static string unfinal[];
	Gram(){
		//写入follow,用于错误处理
		follow[1].push_back("#");
		follow[2].push_back("#");  follow[2].push_back("type");
		follow[3].push_back("#");  follow[3].push_back("type");
		follow[4].push_back("#");  follow[4].push_back("begin"); 
		follow[5].push_back("#");  follow[5].push_back("var"); follow[5].push_back("begin"); follow[5].push_back("procedure");
		follow[6].push_back("#");  follow[6].push_back("var"); follow[6].push_back("begin");  follow[6].push_back("procedure");
		follow[7].push_back("#");  follow[7].push_back("var"); follow[7].push_back("begin"); follow[7].push_back("procedure");
		follow[8].push_back("#");  follow[8].push_back("var"); follow[8].push_back("begin"); follow[8].push_back("procedure");
		follow[9].push_back("=");
		follow[10].push_back("var"); follow[10].push_back("begin"); follow[10].push_back("procedure");
		follow[10].push_back("#"); follow[10].push_back(";"); follow[10].push_back("id");

		follow[11].push_back("var"); follow[11].push_back("begin"); follow[11].push_back("end");
		follow[11].push_back("#"); follow[11].push_back(";"); follow[11].push_back("id");

		follow[12].push_back("var"); follow[12].push_back("begin");
		follow[12].push_back("#"); follow[12].push_back(";"); follow[12].push_back("id");

		follow[13].push_back("var"); follow[13].push_back("begin"); follow[13].push_back("end");
		follow[13].push_back("#"); follow[13].push_back(";"); follow[13].push_back("id");

		follow[14].push_back("..");
		follow[15].push_back("]");
		follow[16].push_back("#"); follow[16].push_back(";"); follow[16].push_back("id"); follow[16].push_back("var");
		follow[17].push_back("end"); follow[17].push_back("#");
		follow[18].push_back("end"); follow[18].push_back("#");
		follow[19].push_back(";"); follow[19].push_back("#");
		follow[20].push_back(";"); follow[20].push_back("#");
		follow[21].push_back("begin"); follow[21].push_back("procedure");follow[21].push_back("#");
		follow[22].push_back("begin"); follow[22].push_back("procedure"); follow[22].push_back("#");
		follow[23].push_back("begin"); follow[23].push_back("procedure"); follow[23].push_back("#");
		follow[24].push_back("begin"); follow[24].push_back("procedure"); follow[24].push_back("#");
		follow[25].push_back(";"); follow[25].push_back("#");
		follow[26].push_back(";"); follow[26].push_back("#");
		follow[27].push_back("begin"); follow[27].push_back("#");
		follow[28].push_back("begin"); follow[28].push_back("#");
		follow[29].push_back("begin");  follow[29].push_back("#");
		follow[30].push_back("begin");  follow[30].push_back("#"); follow[30].push_back("(");  
		follow[31].push_back(")");  follow[31].push_back("#");
		follow[32].push_back(")");  follow[32].push_back("#");
		follow[33].push_back(")");  follow[33].push_back("#");
		follow[34].push_back(")");  follow[34].push_back("#"); follow[34].push_back(";");
		follow[35].push_back(")");  follow[35].push_back("#"); follow[35].push_back(";");
		follow[36].push_back(")");  follow[36].push_back("#"); follow[36].push_back(";");
		follow[37].push_back("begin");  follow[37].push_back("#");
		follow[38].push_back("begin");  follow[38].push_back("#"); follow[38].push_back("procedure"); 
		follow[39].push_back("id");  follow[39].push_back("#");
		follow[40].push_back("end");  follow[40].push_back("#"); follow[40].push_back("else"); 
		follow[40].push_back("fi"); follow[40].push_back("endwh");

		follow[41].push_back("end");  follow[41].push_back("#"); follow[41].push_back("else");
		follow[41].push_back("fi"); follow[41].push_back("endwh");

		follow[42].push_back("end");  follow[42].push_back("#"); follow[42].push_back("else");
		follow[42].push_back("fi"); follow[42].push_back("endwh");

		follow[43].push_back("end");  follow[43].push_back("#"); follow[43].push_back("else");
		follow[43].push_back("fi"); follow[43].push_back("endwh");

		follow[44].push_back("end");  follow[44].push_back("#"); follow[44].push_back("else");
		follow[44].push_back("fi"); follow[44].push_back("endwh");

		follow[45].push_back("end");  follow[45].push_back("#"); follow[45].push_back("else");
		follow[45].push_back("fi"); follow[45].push_back("endwh");

		follow[46].push_back("end");  follow[46].push_back("#"); follow[46].push_back("else");
		follow[46].push_back("fi"); follow[46].push_back("endwh");

		follow[47].push_back("end");  follow[47].push_back("#"); follow[47].push_back("else");
		follow[47].push_back("fi"); follow[47].push_back("endwh");
		follow[48].push_back(")");

		follow[49].push_back("end");  follow[49].push_back("#"); follow[49].push_back("else");
		follow[49].push_back("fi"); follow[49].push_back("endwh");

		follow[50].push_back("end");  follow[50].push_back("#"); follow[50].push_back("else");
		follow[50].push_back("fi"); follow[50].push_back("endwh");

		follow[51].push_back("end");  follow[51].push_back("#"); follow[51].push_back("else");
		follow[51].push_back("fi"); follow[51].push_back("endwh");
		follow[52].push_back(")"); follow[52].push_back("#");
		follow[53].push_back(")"); follow[53].push_back("#");
		follow[54].push_back("then"); follow[54].push_back("#"); follow[54].push_back("do"); 
		follow[55].push_back("then"); follow[55].push_back("do");
		follow[56].push_back("end"); follow[56].push_back("else"); follow[56].push_back("fi");
		follow[56].push_back("then"); follow[56].push_back("do"); follow[56].push_back("endwh");
		follow[56].push_back("]"); follow[56].push_back(")"); follow[56].push_back(",");
		follow[56].push_back("<"); follow[56].push_back("="); follow[56].push_back("+"); follow[56].push_back("#");

		follow[57].push_back("end"); follow[57].push_back("else"); follow[57].push_back("fi");
		follow[57].push_back("then"); follow[57].push_back("do"); follow[57].push_back("endwh");
		follow[57].push_back("]"); follow[57].push_back(")"); follow[57].push_back(",");
		follow[57].push_back("<"); follow[57].push_back("="); follow[57].push_back("#");

		follow[58].push_back("end"); follow[58].push_back("else"); follow[58].push_back("fi");
		follow[58].push_back("then"); follow[58].push_back("do"); follow[58].push_back("endwh");
		follow[58].push_back("]"); follow[58].push_back(")"); follow[58].push_back(",");
		follow[58].push_back("<"); follow[58].push_back("="); follow[58].push_back("#");

		follow[59].push_back("end"); follow[59].push_back("else"); follow[59].push_back("fi");
		follow[59].push_back("then"); follow[59].push_back("do"); follow[59].push_back("endwh");
		follow[59].push_back("]"); follow[59].push_back(")"); follow[59].push_back(","); follow[59].push_back("-");
		follow[59].push_back("<"); follow[59].push_back("="); follow[59].push_back("#"); follow[59].push_back("+");

		follow[60].push_back("end"); follow[60].push_back("else"); follow[60].push_back("fi");
		follow[60].push_back("then"); follow[60].push_back("do"); follow[60].push_back("endwh");
		follow[60].push_back("]"); follow[60].push_back(")"); follow[60].push_back(","); follow[60].push_back("-");
		follow[60].push_back("<"); follow[60].push_back("="); follow[60].push_back("#"); follow[60].push_back("+");

		follow[61].push_back("end"); follow[61].push_back("else"); follow[61].push_back("fi"); follow[61].push_back("*"); 
		follow[61].push_back("then"); follow[61].push_back("do"); follow[61].push_back("endwh"); follow[61].push_back("/");
		follow[61].push_back("]"); follow[61].push_back(")"); follow[61].push_back(","); follow[61].push_back("-");
		follow[61].push_back("<"); follow[61].push_back("="); follow[61].push_back("#"); follow[61].push_back("+");

		follow[62].push_back("end"); follow[62].push_back("else"); follow[62].push_back("fi"); follow[62].push_back("*");
		follow[62].push_back("then"); follow[62].push_back("do"); follow[62].push_back("endwh"); follow[62].push_back("/");
		follow[62].push_back("]"); follow[62].push_back(")"); follow[62].push_back(","); follow[62].push_back("-");
		follow[62].push_back("<"); follow[62].push_back("="); follow[62].push_back("#"); follow[62].push_back("+");
		follow[62].push_back(":=");

		follow[63].push_back("end"); follow[63].push_back("else"); follow[63].push_back("fi"); follow[63].push_back("*");
		follow[63].push_back("then"); follow[63].push_back("do"); follow[63].push_back("endwh"); follow[63].push_back("/");
		follow[63].push_back("]"); follow[63].push_back(")"); follow[63].push_back(","); follow[63].push_back("-");
		follow[63].push_back("<"); follow[63].push_back("="); follow[63].push_back("#"); follow[63].push_back("+");
		follow[63].push_back(":=");

		follow[64].push_back("end"); follow[64].push_back("else"); follow[64].push_back("fi"); follow[64].push_back("*");
		follow[64].push_back("then"); follow[64].push_back("do"); follow[64].push_back("endwh"); follow[64].push_back("/");
		follow[64].push_back("]"); follow[64].push_back(")"); follow[64].push_back(","); follow[64].push_back("-");
		follow[64].push_back("<"); follow[64].push_back("="); follow[64].push_back("#"); follow[64].push_back("+");
		follow[64].push_back(":=");

		follow[65].push_back("end"); follow[65].push_back("else"); follow[65].push_back("fi"); follow[65].push_back("*");
		follow[65].push_back("then"); follow[65].push_back("do"); follow[65].push_back("endwh"); follow[65].push_back("/");
		follow[65].push_back("]"); follow[65].push_back(")"); follow[65].push_back(","); follow[65].push_back("(");
		follow[65].push_back("<"); follow[65].push_back("="); follow[65].push_back("#"); follow[65].push_back("INTC");
		follow[65].push_back(":=");

		follow[66].push_back("end"); follow[66].push_back("else"); follow[66].push_back("fi"); follow[66].push_back("*");
		follow[66].push_back("then"); follow[66].push_back("do"); follow[66].push_back("endwh"); follow[66].push_back("/");
		follow[66].push_back("]"); follow[66].push_back(")"); follow[66].push_back(","); follow[66].push_back("(");
		follow[66].push_back("<"); follow[66].push_back("="); follow[66].push_back("#"); follow[66].push_back("INTC");
		follow[66].push_back(":=");

		follow[67].push_back("end"); follow[67].push_back("else"); follow[67].push_back("fi"); follow[67].push_back("*");
		follow[67].push_back("then"); follow[67].push_back("do"); follow[67].push_back("endwh"); follow[67].push_back("/");
		follow[67].push_back("]"); follow[67].push_back(")"); follow[67].push_back(","); follow[67].push_back("(");
		follow[67].push_back("<"); follow[67].push_back("="); follow[67].push_back("#"); follow[67].push_back("INTC");
		follow[67].push_back(":="); follow[67].push_back("+"); follow[67].push_back("-");

		//写入predict		
		predict[1].push_back("program");
		predict[2].push_back("program");

		predict[3].push_back("id");

		predict[4].push_back("type");            predict[4].push_back("var");
		predict[4].push_back("procedure");       predict[4].push_back("begin");

		predict[5].push_back("var");             predict[5].push_back("procedure");
		predict[5].push_back("begin");			 predict[5].push_back("#");

		predict[6].push_back("type");

		predict[7].push_back("type");

		predict[8].push_back("id");

		predict[9].push_back("var");             predict[9].push_back("procedure");
		predict[9].push_back("begin");			 predict[9].push_back("#");

		predict[10].push_back("id");

		predict[11].push_back("id");

		predict[12].push_back("integer");        predict[12].push_back("char");

		predict[13].push_back("array");          predict[13].push_back("record");

		predict[14].push_back("id");

		predict[15].push_back("integer");

		predict[16].push_back("char");

		predict[17].push_back("array");

		predict[18].push_back("record");

		predict[19].push_back("array");

		predict[20].push_back("INTC");

		predict[21].push_back("INTC");

		predict[22].push_back("record");

		predict[23].push_back("integer");        predict[23].push_back("char");

		predict[24].push_back("array");

		predict[25].push_back("end");			 predict[25].push_back("#");

		predict[26].push_back("integer");        predict[26].push_back("char");
		predict[26].push_back("array");

		predict[27].push_back("id");

		predict[28].push_back(";");

		predict[29].push_back(",");

		predict[30].push_back("procedure");      predict[30].push_back("begin");
		predict[30].push_back("#");

		predict[31].push_back("var");

		predict[32].push_back("var");

		predict[33].push_back("integer");        predict[33].push_back("char");
		predict[33].push_back("array");          predict[33].push_back("record");
		predict[33].push_back("id");

		predict[34].push_back("procedure");      predict[34].push_back("begin");
		predict[34].push_back("#");

		predict[35].push_back("integer");        predict[35].push_back("char");
		predict[35].push_back("array");          predict[35].push_back("record");
		predict[35].push_back("id");

		predict[36].push_back("id");

		predict[37].push_back(";");				  predict[37].push_back("#");

		predict[38].push_back(",");

		predict[39].push_back("begin");			   predict[39].push_back("#");

		predict[40].push_back("procedure");

		predict[41].push_back("procedure");

		predict[42].push_back("begin");			   predict[42].push_back("#");

		predict[43].push_back("procedure");

		predict[44].push_back("id");

		predict[45].push_back(")");				 predict[45].push_back("#");

		predict[46].push_back("integer");        predict[46].push_back("char");
		predict[46].push_back("array");          predict[46].push_back("record");
		predict[46].push_back("id");             predict[46].push_back("var");

		predict[47].push_back("integer");        predict[47].push_back("char");
		predict[47].push_back("array");          predict[47].push_back("record");
		predict[47].push_back("id");             predict[47].push_back("var");

		predict[48].push_back(")");				  predict[48].push_back("#");

		predict[49].push_back(";");

		predict[50].push_back("integer");        predict[50].push_back("char");
		predict[50].push_back("array");          predict[50].push_back("record");
		predict[50].push_back("id");

		predict[51].push_back("var");

		predict[52].push_back("id");

		predict[53].push_back(";");           predict[53].push_back(")");
		predict[53].push_back("#");

		predict[54].push_back(",");

		predict[55].push_back("type");           predict[55].push_back("var");
		predict[55].push_back("procedure");      predict[55].push_back("begin");

		predict[56].push_back("begin");

		predict[57].push_back("begin");

		predict[58].push_back("id");             predict[58].push_back("if");
		predict[58].push_back("while");          predict[58].push_back("return");
		predict[58].push_back("read");           predict[58].push_back("write");
		predict[58].push_back("end");            predict[58].push_back(";");

		predict[59].push_back("else");           predict[59].push_back("fi");
		predict[59].push_back("end");            predict[59].push_back("endwh");
		predict[59].push_back("#");

		predict[60].push_back(";");

		predict[61].push_back("if");

		predict[62].push_back("while");

		predict[63].push_back("read");

		predict[64].push_back("write");

		predict[65].push_back("return");

		predict[66].push_back("id");

		predict[67].push_back(":=");         predict[67].push_back(".");
		predict[67].push_back("[");

		predict[68].push_back("(");

		predict[69].push_back("[");      predict[69].push_back(".");
		predict[69].push_back(":=");

		predict[70].push_back("if");

		predict[71].push_back("while");

		predict[72].push_back("read");

		predict[73].push_back("id");

		predict[74].push_back("write");

		predict[75].push_back("return");

		predict[76].push_back("(");

		predict[77].push_back(")");			 predict[77].push_back("#");

		predict[78].push_back("(");         predict[78].push_back("INTC");
		predict[78].push_back("id");

		predict[79].push_back(")");			 predict[79].push_back("#");

		predict[80].push_back(",");

		predict[81].push_back("(");         predict[81].push_back("INTC");
		predict[81].push_back("id");

		predict[82].push_back("<");             predict[82].push_back("=");

		predict[83].push_back("(");         predict[83].push_back("INTC");
		predict[83].push_back("id");

		predict[84].push_back("<");             predict[84].push_back("=");
		predict[84].push_back("]");      predict[84].push_back("then");
		predict[84].push_back("else");           predict[84].push_back("fi");
		predict[84].push_back("do");             predict[84].push_back("endwh");
		predict[84].push_back(")");         predict[84].push_back("end");
		predict[84].push_back(";");           predict[84].push_back(",");
		predict[84].push_back("#");

		predict[85].push_back("+");           predict[85].push_back("-");

		predict[86].push_back("(");         predict[86].push_back("INTC");
		predict[86].push_back("id");

		predict[87].push_back("+");           predict[87].push_back("-");
		predict[87].push_back("<");             predict[87].push_back("=");
		predict[87].push_back("]");      predict[87].push_back("then");
		predict[87].push_back("else");           predict[87].push_back("fi");
		predict[87].push_back("do");             predict[87].push_back("endwh");
		predict[87].push_back(")");         predict[87].push_back("end");
		predict[87].push_back(";");           predict[87].push_back(",");
		predict[87].push_back("#");

		predict[88].push_back("*");          predict[88].push_back("/");

		predict[89].push_back("(");

		predict[90].push_back("INTC");

		predict[91].push_back("id");

		predict[92].push_back("id");

		predict[93].push_back(":=");         predict[93].push_back("*");
		predict[93].push_back("/");           predict[93].push_back("+");
		predict[93].push_back("-");          predict[93].push_back("<");
		predict[93].push_back("=");             predict[93].push_back("then");
		predict[93].push_back("else");           predict[93].push_back("fi");
		predict[93].push_back("do");             predict[93].push_back("endwh");
		predict[93].push_back(")");         predict[93].push_back("end");
		predict[93].push_back(";");           predict[93].push_back(",");
		predict[93].push_back("]");			 predict[93].push_back("#");

		predict[94].push_back("[");

		predict[95].push_back(".");

		predict[96].push_back("id");

		predict[97].push_back(":=");         predict[97].push_back("*");
		predict[97].push_back("/");           predict[97].push_back("+");
		predict[97].push_back("-");          predict[97].push_back("<");
		predict[97].push_back("=");             predict[97].push_back("then");
		predict[97].push_back("else");           predict[97].push_back("fi");
		predict[97].push_back("do");             predict[97].push_back("endwh");
		predict[97].push_back(")");         predict[97].push_back("end");
		predict[97].push_back(";");           predict[97].push_back(",");
		predict[97].push_back("]");			  predict[97].push_back("#");

		predict[98].push_back("[");

		predict[99].push_back("<");

		predict[100].push_back("=");

		predict[101].push_back("+");

		predict[102].push_back("-");

		predict[103].push_back("*");

		predict[104].push_back("/");

		//写入产生式
		bds[1].left=("Program") ;               bds[1].right.push_back("ProgramHead") ;  
        bds[1].right.push_back("DeclarePart") ;     bds[1].right.push_back("ProgramBody") ;  
          
        bds[2].left=("ProgramHead") ;           bds[2].right.push_back("program") ;  
        bds[2].right.push_back("ProgramName") ;  
          
        bds[3].left=("ProgramName") ;           bds[3].right.push_back("id") ;  
          
        bds[4].left=("DeclarePart") ;           bds[4].right.push_back("TypeDecpart") ;  
        bds[4].right.push_back("VarDecpart") ;      bds[4].right.push_back("ProcDecpart") ;  
          
        bds[5].left=("TypeDecpart") ;  
          
        bds[6].left=("TypeDecpart") ;           bds[6].right.push_back("TypeDec") ;  
          
        bds[7].left=("TypeDec") ;               bds[7].right.push_back("type") ;  
        bds[7].right.push_back("TypeDecList") ;  
          
        bds[8].left=("TypeDecList") ;           bds[8].right.push_back("TypeId") ;  
        bds[8].right.push_back("=") ;                   bds[8].right.push_back("TypeDef") ;  
        bds[8].right.push_back(";") ;                 bds[8].right.push_back("TypeDecMore") ;  
          
        bds[9].left=("TypeDecMore") ;             
          
        bds[10].left=("TypeDecMore") ;          bds[10].right.push_back("TypeDecList") ;  
          
        bds[11].left=("TypeId") ;               bds[11].right.push_back("id") ;  
          
        bds[12].left=("TypeDef") ;              bds[12].right.push_back("BaseType") ;  
          
        bds[13].left=("TypeDef") ;              bds[13].right.push_back("StructureType") ;  
          
        bds[14].left=("TypeDef") ;              bds[14].right.push_back("id") ;  
          
        bds[15].left=("BaseType") ;             bds[15].right.push_back("integer") ;  
          
        bds[16].left=("BaseType") ;             bds[16].right.push_back("char") ;  
          
        bds[17].left=("StructureType") ;        bds[17].right.push_back("ArrayType") ;  
          
        bds[18].left=("StructureType") ;        bds[18].right.push_back("RecType") ;  
          
        bds[19].left=("ArrayType") ;            bds[19].right.push_back("array") ;  
        bds[19].right.push_back("[") ;           bds[19].right.push_back("Low") ;  
        bds[19].right.push_back("..") ;           bds[19].right.push_back("Top") ;  
        bds[19].right.push_back("]") ;           bds[19].right.push_back("of") ;                    
        bds[19].right.push_back("BaseType") ;  
          
        bds[20].left=("Low") ;                  bds[20].right.push_back("INTC") ;  
          
        bds[21].left=("Top") ;                  bds[21].right.push_back("INTC") ;  
          
        bds[22].left=("RecType") ;              bds[22].right.push_back("record") ;  
        bds[22].right.push_back("FieldDecList") ;   bds[22].right.push_back("end") ;  
          
        bds[23].left=("FieldDecList") ;         bds[23].right.push_back("BaseType") ;  
        bds[23].right.push_back("IdList") ;         bds[23].right.push_back(";") ;  
        bds[23].right.push_back("FieldDecMore") ;  
          
        bds[24].left=("FieldDecList") ;         bds[24].right.push_back("ArrayType") ;  
        bds[24].right.push_back("IdList") ;         bds[24].right.push_back(";") ;  
        bds[24].right.push_back("FieldDecMore") ;  
          
        bds[25].left=("FieldDecMore") ;  
          
        bds[26].left=("FieldDecMore") ;         bds[26].right.push_back("FieldDecList") ;  
          
        bds[27].left=("IdList") ;               bds[27].right.push_back("id") ;  
        bds[27].right.push_back("IdMore") ;  
          
        bds[28].left=("IdMore") ;  
          
        bds[29].left=("IdMore") ;               bds[29].right.push_back(",") ;  
        bds[29].right.push_back("IdList") ;  
          
        bds[30].left=("VarDecpart") ;  
          
        bds[31].left=("VarDecpart") ;           bds[31].right.push_back("VarDec") ;  
          
        bds[32].left=("VarDec") ;               bds[32].right.push_back("var") ;  
        bds[32].right.push_back("VarDecList") ;  
          
        bds[33].left=("VarDecList") ;           bds[33].right.push_back("TypeDef") ;  
        bds[33].right.push_back("VarIdList") ;      bds[33].right.push_back(";") ;  
        bds[33].right.push_back("VarDecMore") ;  
          
        bds[34].left=("VarDecMore") ;  
          
        bds[35].left=("VarDecMore") ;           bds[35].right.push_back("VarDecList") ;  
          
        bds[36].left=("VarIdList") ;            bds[36].right.push_back("id") ;  
        bds[36].right.push_back("VarIdMore") ;  
          
        bds[37].left=("VarIdMore") ;  
          
        bds[38].left=("VarIdMore") ;            bds[38].right.push_back(",") ;  
        bds[38].right.push_back("VarIdList") ;  
          
        bds[39].left=("ProcDecpart") ;            
          
        bds[40].left=("ProcDecpart") ;          bds[40].right.push_back("ProcDec") ;  
          
        bds[41].left=("ProcDec") ;              bds[41].right.push_back("procedure") ;  
        bds[41].right.push_back("ProcName") ;       bds[41].right.push_back("(") ;  
        bds[41].right.push_back("ParamList") ;      bds[41].right.push_back(")") ;  
        bds[41].right.push_back(";") ;                bds[41].right.push_back("ProcDecPart") ;  
        bds[41].right.push_back("ProcBody") ;       bds[41].right.push_back("ProcDecMore") ;  
          
        bds[42].left=("ProcDecMore") ;  
          
        bds[43].left=("ProcDecMore") ;          bds[43].right.push_back("ProcDec") ;  
          
        bds[44].left=("ProcName") ;             bds[44].right.push_back("id") ;  
          
        bds[45].left=("ParamList") ;              
          
        bds[46].left=("ParamList") ;            bds[46].right.push_back("ParamDecList") ;  
          
        bds[47].left=("ParamDecList") ;         bds[47].right.push_back("Param") ;  
        bds[47].right.push_back("ParamMore") ;  
          
        bds[48].left=("ParamMore") ;  
          
        bds[49].left=("ParamMore") ;            bds[49].right.push_back(";") ;  
        bds[49].right.push_back("ParamDecList") ;  
          
        bds[50].left=("Param") ;                bds[50].right.push_back("TypeDef") ;  
        bds[50].right.push_back("FormList") ;  
          
        bds[51].left=("Param") ;                bds[51].right.push_back("var") ;  
        bds[51].right.push_back("TypeDef") ;        bds[51].right.push_back("FormList") ;  
          
        bds[52].left=("FormList") ;             bds[52].right.push_back("id") ;  
        bds[52].right.push_back("FidMore") ;  
          
        bds[53].left=("FidMore") ;  
          
        bds[54].left=("FidMore") ;              bds[54].right.push_back(",") ;  
        bds[54].right.push_back("FormList") ;  
          
        bds[55].left=("ProcDecPart") ;          bds[55].right.push_back("DeclarePart") ;  
          
        bds[56].left=("ProcBody") ;             bds[56].right.push_back("ProgramBody") ;  
          
        bds[57].left=("ProgramBody") ;          bds[57].right.push_back("begin") ;  
        bds[57].right.push_back("StmList") ;        bds[57].right.push_back("end") ;  
          
        bds[58].left=("StmList") ;              bds[58].right.push_back("Stm") ;  
        bds[58].right.push_back("StmMore") ;  
          
        bds[59].left=("StmMore") ;  
          
        bds[60].left=("StmMore") ;              bds[60].right.push_back(";") ;  
        bds[60].right.push_back("StmList") ;  
          
        bds[61].left=("Stm") ;                  bds[61].right.push_back("ConditionalStm") ;  
          
        bds[62].left=("Stm") ;                  bds[62].right.push_back("LoopStm") ;  
          
        bds[63].left=("Stm") ;                  bds[63].right.push_back("InputStm") ;  
          
        bds[64].left=("Stm") ;                  bds[64].right.push_back("OutputStm") ;  
          
        bds[65].left=("Stm") ;                  bds[65].right.push_back("ReturnStm") ;  
          
        bds[66].left=("Stm") ;                  bds[66].right.push_back("id") ;  
        bds[66].right.push_back("AssCall") ;  
          
        bds[67].left=("AssCall");               bds[67].right.push_back("AssignmentRest") ;  
          
        bds[68].left=("AssCall") ;              bds[68].right.push_back("CallStmRest") ;  
          
        bds[69].left=("AssignmentRest") ;       bds[69].right.push_back("VariMore") ;   
        bds[69].right.push_back(":=") ;              bds[69].right.push_back("Exp") ;  
          
        bds[70].left=("ConditionalStm") ;       bds[70].right.push_back("if") ;  
        bds[70].right.push_back("RelExp") ;         bds[70].right.push_back("then") ;   
        bds[70].right.push_back("StmList") ;        bds[70].right.push_back("else") ;  
        bds[70].right.push_back("StmList") ;        bds[70].right.push_back("fi") ;  
          
        bds[71].left=("LoopStm") ;              bds[71].right.push_back("while") ;  
        bds[71].right.push_back("RelExp") ;         bds[71].right.push_back("do") ;  
        bds[71].right.push_back("StmList") ;        bds[71].right.push_back("endwh") ;  
          
        bds[72].left=("InputStm") ;             bds[72].right.push_back("read") ;  
        bds[72].right.push_back("(") ;              bds[72].right.push_back("Invar") ;  
        bds[72].right.push_back(")") ;  
          
        bds[73].left=("Invar") ;                bds[73].right.push_back("id") ;  
          
        bds[74].left=("OutputStm") ;            bds[74].right.push_back("write") ;  
        bds[74].right.push_back("(") ;              bds[74].right.push_back("Exp") ;  
        bds[74].right.push_back(")") ;  
          
        bds[75].left=("ReturnStm") ;            bds[75].right.push_back("return") ;  
          
        bds[76].left=("CallStmRest") ;          bds[76].right.push_back("(");  
        bds[76].right.push_back("ActParamList") ;   bds[76].right.push_back(")") ;  
          
        bds[77].left=("ActParamList") ;   
          
        bds[78].left=("ActParamList") ;         bds[78].right.push_back("Exp") ;  
        bds[78].right.push_back("ActParamMore") ;  
          
        bds[79].left=("ActParamMore") ;   
          
        bds[80].left=("ActParamMore") ;         bds[80].right.push_back(",") ;  
        bds[80].right.push_back("ActParamList") ;  
          
        bds[81].left=("RelExp") ;               bds[81].right.push_back("Exp") ;  
        bds[81].right.push_back("OtherRelE") ;  
          
        bds[82].left=("OtherRelE") ;            bds[82].right.push_back("CmpOp") ;  
        bds[82].right.push_back("Exp") ;  
          
        bds[83].left=("Exp") ;                  bds[83].right.push_back("Term") ;  
        bds[83].right.push_back("OtherTerm") ;  
          
        bds[84].left=("OtherTerm") ;              
          
        bds[85].left=("OtherTerm") ;            bds[85].right.push_back("AddOp") ;  
        bds[85].right.push_back("Exp") ;  
          
        bds[86].left=("Term") ;                 bds[86].right.push_back("Factor") ;  
        bds[86].right.push_back("OtherFactor") ;  
          
        bds[87].left=("OtherFactor") ;            
          
        bds[88].left=("OtherFactor") ;          bds[88].right.push_back("MultOp") ;  
        bds[88].right.push_back("Term") ;  
          
        bds[89].left=("Factor") ;               bds[89].right.push_back("(") ;  
        bds[89].right.push_back("Exp") ;            bds[89].right.push_back(")") ;  
          
        bds[90].left=("Factor") ;               bds[90].right.push_back("INTC") ;  
          
        bds[91].left=("Factor") ;               bds[91].right.push_back("Variable") ;  
          
        bds[92].left=("Variable") ;             bds[92].right.push_back("id") ;  
        bds[92].right.push_back("VariMore") ;  
          
        bds[93].left=("VariMore") ;   
          
        bds[94].left=("VariMore") ;             bds[94].right.push_back("[") ;  
        bds[94].right.push_back("Exp") ;            bds[94].right.push_back("]") ;  
          
        bds[95].left=("VariMore") ;             bds[95].right.push_back(".") ;  
        bds[95].right.push_back("FieldVar") ;  
          
        bds[96].left=("FieldVar") ;             bds[96].right.push_back("id") ;  
        bds[96].right.push_back("FieldVarMore") ;  
          
        bds[97].left=("FieldVarMore") ;  
          
        bds[98].left=("FieldVarMore") ;         bds[98].right.push_back("[") ;  
        bds[98].right.push_back("Exp") ;            bds[98].right.push_back("]") ;  
          
        bds[99].left=("CmpOp") ;                bds[99].right.push_back("<") ;  
          
        bds[100].left=("CmpOp") ;               bds[100].right.push_back("=") ;  
          
        bds[101].left=("AddOp") ;               bds[101].right.push_back("+") ;  
          
        bds[102].left=("AddOp") ;               bds[102].right.push_back("-") ;  
          
        bds[103].left=("MultOp") ;              bds[103].right.push_back("*") ;  
          
        bds[104].left=("MultOp") ;              bds[104].right.push_back("/") ;



	}

	int getFinalIndex(string str){   //得到终结符号
		for (int i = 0; i < 41; i++){
			if (final[i].compare(str) == 0)
				return i;
		}
		return -1;
	}
	int getUnfinalIndex(string str){   //得到非终结符号
		for (int i = 0; i < 68; i++){
			if (unfinal[i].compare(str)==0)
				return i;
		}
		return -1;
	}
	void buildTable(){			
		//初始化
		for (int i = 0; i < 100; i++)
		   for (int j = 0; j < 100;j++)
				table[i][j] = -1;

		//得到follow集，用于错误处理 table[产生式头][它的follow]=-2
		   for (int i = 1; i < 105; i++)
				for(int j=0;j<follow[i].size();j++){
					table[getUnfinalIndex(bds[i].left) + 1][getFinalIndex(follow[i][j])] = -2;
		   }
		  
		//得到predict集，predict[表达式号]=a,b,c，则table[表达式号.left][a]=表达式号
		for (int i = 1; i < 105; i++)
			for (int j = 0; j < predict[i].size();j++){
				table[getUnfinalIndex(bds[i].left)+1][getFinalIndex(predict[i][j])] = i;
		}

		//加入错误处理！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	}

	void analysis(vector<Token> res){   
		/* 分析过程，同时处理分析栈和剩余输入。res是tokenlist。
		** 函数需要定义tree，边分析边构造树，在分析栈每次删除元素时构造该结点
	    */
		
		/*初始化*/
		string action=""; // 动作
		analy.push_back("#");
		analy.push_back(unfinal[0]); 
		input = res;
		reverse(input.begin(), input.end());  //剩余输入,倒置

		/*处理三个表*/
		while (input.size() > 0)
		{
			//act
			string ch1 = analy[analy.size() - 1];
			string t = input[input.size() - 1].type;
			if(t[0]=='$')
					t=t.erase(0, 1); //去掉$符号
			 
			int i, j;        //i:非终极符索引 j:终极符索引
			int bdsNum;      //表达式号
			if ((i = getUnfinalIndex(ch1)+1)!=0)   //非终极符，找表达式
			{
				action = "";
				j = getFinalIndex(t);  //0开始
				
				bdsNum = table[i][j];
				
				if (bdsNum == -1)
				{
					cout << endl;
					cout << "   警告: 符号  "<< t << "在匹配时发生了一个错误" << endl ;
					cout << "   错误原因：可能输入了多余的符号 " << endl<<endl;
					input.pop_back();
				}
				else if (bdsNum == -2)
				{
					cout << endl;
					cout << "   警告: 符号  " << t << "在匹配时发生了一个错误" << endl;
					cout << "   错误原因：‘"<<t<<"’之前可能缺少一些符号 " << endl << endl;
					analy.pop_back();
				}
				else {
					act.clear();
					analy.pop_back();
					for (int k = 0; k < bds[bdsNum].right.size(); k++)
					{
						act.push_back(bds[bdsNum].right[k]);
						action = action + " " + bds[bdsNum].right[k];
						analy.push_back(bds[bdsNum].right[bds[bdsNum].right.size()-1-k]);        //呈现栈的效果，因此倒置插入
					}																
			
					cout << setw(15) << action << endl;
				}
			}
			else               //终极符，匹配
			{
				if (ch1==t)
				{	
					action = "";
					if (ch1 == "#")
					{
						action = "Accepted!";
						cout << setw(15) << action << endl;
						return;
					}
					else{
						action = ch1 + "匹配";
						analy.pop_back();
						input.pop_back();
						cout << setw(15) << action << endl;
					}
				}
				else{					
					//匹配错误的错误处理！！！！！！！！！！！！！！！！！！！
					analy.pop_back();
					cout << endl;
					cout << "   警告: 符号  " << t << "在匹配时发生了一个错误" << endl;
					cout << "   错误原因：‘" << t << "’之前可能缺少一些符号 !!" << endl << endl;
				}
			}
			
		}
	}
};

string Gram::final[] = { "integer", "char", "program", "array",
"of",
"record",
"end",
"var",
"procedure",
"begin",
"if",
"then",
"else",
"fi",
"while",
"do",
"endwh",
"read",
"write",
"return",
"type",
"repeat", "id", "INTC",
"+", "-", "*", "/", "<", ";", ":=", ",", ".", "..", "=", "[", "]", "(", ")", 
"#"
};

string Gram::unfinal[] = {      //非终结符 
	"Program", "ProgramHead", "ProgramName", "DeclarePart", "TypeDecpart", "TypeDec",
	"TypeDecList", "TypeDecMore", "TypeId", "TypeDef", "BaseType", "StructureType",
	"ArrayType", "Low", "Top", "RecType", "FieldDecList", "FieldDecMore", "IdList",
	"IdMore", "VarDecpart", "VarDec", "VarDecList", "VarDecMore", "VarIdList",
	"VarIdMore", "ProcDecpart", "ProcDec", "ProcDecMore", "ProcName",
	"ParamList", "ProcDeclaration", "ParamDecList", "ParamMore", "Param", "FormList",
	"FidMore", "ProcDecPart", "ProcBody", "ProgramBody", "StmList", "StmMore", "Stm",
	"AssCall", "AssignmentRest",
	"ConditionalStm", "LoopStm", "InputStm", "Invar", "OutputStm", "ReturnStm",
	"CallStmRest", "ActParamList", "ActParamMore", "RelExp", "OtherRelE", "Exp",
	"OtherTerm", "Term", "OtherFactor", "Factor", "Variable", "VariMore", "FieldVar",
	"FieldVarMore", "CmpOp", "AddOp", "MultOp"
};
int main(){
	vector<Token> result;
	Token *temp = NULL;
	string filename = "D:\\pro\\b2.txt";
	FILE *file = fopen(filename.c_str(), "r");

	if (!file){
		printf("打开文件失败\n");
		return -1;
	}
	while ((temp = scanner(file)) != NULL){
		result.push_back(*temp);
		delete temp;
		temp = NULL;
	}
	fclose(file);
	
	Gram g;
	g.buildTable();
	g.analysis(result);
	
	system("pause");
	
}