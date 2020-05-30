#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <windows.h>



const char* REGEX="(include|as)|(<[^>]*>|\"(?:\\[\'\"\\]|.)*\"|\'(?:\\[\'\"\\]|.)')|([a-zA-Z_][a-zA-Z0-9_]*)|(\\/\\/|[\\=\\+\\-\\*\\/%^])|([0-9][0-9_]*(?:\\.[0-9][0-9_]*)?)|(\\.)|(,)|(;)|([\\(\\)])|([\\[\\]])|\\s+";




enum ERROR_E:unsigned int{
	ERROR_FILE_DOESNT_EXIST=1,
	ERROR_UNEXPECTED_CHARACTER=2
};



struct Token{
	enum TOKEN{
		TOKEN_KEYWORD,
		TOKEN_STRING,
		TOKEN_NAME,
		TOKEN_OPERATOR,
		TOKEN_NUMBER,
		TOKEN_PERIOD,
		TOKEN_SEP,
		TOKEN_LINE_SEP,
		TOKEN_BRACKETS,
		TOKEN_SQ_BRACKETS
	} t;
	std::string v;
};



void _raise_error(ERROR_E e){
	std::cout<<"ERROR CODE: "<<e<<"\n";
}



std::vector<Token> read_tokenize(const char* fn){
	HANDLE fh=CreateFileA(fn,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);
	std::vector<Token> o;
	if (fh==INVALID_HANDLE_VALUE){
		_raise_error(ERROR_FILE_DOESNT_EXIST);
		return o;
	}
	unsigned long f_sz=GetFileSize(fh,nullptr);
	std::string f_dt(f_sz,'\0');
	unsigned long br;
	ReadFile(fh,&f_dt[0],f_sz,&br,nullptr);
	unsigned long idx=0;
	try{
		std::regex c_rg(REGEX);
		std::smatch m;
		while (std::regex_search(f_dt,m,c_rg)==true){
			if (m.position()!=0){
				std::cout<<idx;
				_raise_error(ERROR_UNEXPECTED_CHARACTER);
				CloseHandle(fh);
				o.clear();
				return o;
			}
			o.push_back({Token::TOKEN_BRACKETS,m.str(0)});
			idx+=m.length();
			f_dt=f_dt.substr(m.position()+m.length());
		}
	}
	catch (std::regex_error &e){
		std::cout<<e.code()<<"\n";
	}
	CloseHandle(fh);
	return o;
}



int main(int argc,const char** argv){
	std::vector<Token> tl=read_tokenize(argv[1]);
	std::cout<<tl.size();
	return 0;
}
