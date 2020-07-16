#include <core.h>//31307+31308
#include <core-api.h>
#include <platform.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <windows.h>
#include <imagehlp.h>
#pragma comment(lib,"imagehlp")



void krzem::core::init(){
	native_modules["native:_sys"]={
		OBJECT_TYPE_MODULE,
		OBJECT_MODIFIER_NONE,
		nullptr,
		{
			{
				"out",
				{
					OBJECT_TYPE_NATIVE_FUNCTION,
					OBJECT_MODIFIER_ONE_ARG,
					krzem::core_api::system::out
				}
			},
			{
				"err",
				{
					OBJECT_TYPE_NATIVE_FUNCTION,
					OBJECT_MODIFIER_ONE_ARG,
					krzem::core_api::system::err
				}
			},
			{
				"path",
				{
					OBJECT_TYPE_STRING,
					OBJECT_MODIFIER_NONE,
					&krzem::core_api::system::path
				}
			}
		}
	};
}



krzem::core::FileObject krzem::core::read_file(const char* fp,bool* e,krzem::core::CallStack cs){
	if (krzem::platform_api::file_exists(fp)==false){
		krzem::core_api::error::raise({.one=krzem::core::create_error("FileNotFoundError",std::string("File '")+fp+"' not Found.",cs)});
		*e=true;
		return {};
	}
	return {std::string(fp),krzem::platform_api::read_file_content(fp)};
}



krzem::core::ASTToken krzem::core::next_token(std::string dt,krzem::core::ASTToken nxt){
	krzem::core::ASTToken o={krzem::core::TOKEN_TYPE_END_OF_DATA};
	size_t i=nxt._off;
	while (true){
		if (i>=dt.length()){
			break;
		}
		else if (dt[i]==' '||dt[i]=='\t'||dt[i]=='\r'||dt[i]=='\n'){
			i++;
			continue;
		}
		else if (dt[i]=='\''){
			o.type=krzem::core::TOKEN_TYPE_CHAR;
			i++;
			if (dt[i]=='\''){
				return {krzem::core::TOKEN_TYPE_ERROR,"Empty Character String.",i-1};
			}
			unsigned int e;
			o.value=krzem::core::get_next_char(dt.substr(i),&i,&e);
			switch (e){
				case 1:
					return {krzem::core::TOKEN_TYPE_ERROR,std::string("Expected a Any of 'b e f n o r t v x', found '")+dt[i]+"'.",i};
				case 2:
					return {krzem::core::TOKEN_TYPE_ERROR,std::string("Expected a Decimal Digit, found '")+dt[i]+"'.",i};
				case 3:
					return {krzem::core::TOKEN_TYPE_ERROR,std::string("Expected a Octal Digit, found '")+dt[i]+"'.",i};
				case 4:
					return {krzem::core::TOKEN_TYPE_ERROR,std::string("Expected a Hexadecimal Digit, found '")+dt[i]+"'.",i};
			}
			if (dt[i]!='\''){
				return {krzem::core::TOKEN_TYPE_ERROR,std::string("Expected String Quote ('\\''), found '")+dt[i]+"'.",i};
			}
			break;
		}
		else if (dt[i]=='\"'){
			o.type=krzem::core::TOKEN_TYPE_STRING;
			i++;
			while (dt[i]!='\"'||dt[i-1]=='\\'){
				o.value+=dt[i];
				i++;
				if (i>=dt.length()){
					return {krzem::core::TOKEN_TYPE_ERROR,"Expected String Quote ('\"'), reached End of Data.",i-1};
				}
			}
			i++;
			break;
		}
		else if (dt[i]=='<'){
			o.type=krzem::core::TOKEN_TYPE_MODULE_INDENTIFIER;
			i++;
			while (dt[i]!='>'){
				o.value+=dt[i];
				if (std::string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-.").find(dt[i])==-1){
					return {krzem::core::TOKEN_TYPE_ERROR,std::string("Expected a Closing Arrow Quote ('>'), found '")+dt[i]+"'.",i};
				}
				i++;
				if (i>=dt.length()){
					return {krzem::core::TOKEN_TYPE_ERROR,"Expected a Closing Arrow Quote ('>'), reached End of Data.",i-1};
				}
			}
			i++;
			break;
		}
		else if (dt.substr(i,4)=="use "){/////////////////////////////////////////////////////////////////////////////////
			i+=4;
			o.type=krzem::core::TOKEN_TYPE_KEYWORD;
			o.value="use";
			break;
		}
		else if (dt[i]==';'){
			o.type=krzem::core::TOKEN_TYPE_SEMICOLON;
			break;
		}
		else{
			return {krzem::core::TOKEN_TYPE_UNKNOWN,dt.substr(i,1),i+1};
		}
	}
	o._off=i;
	return o;
}



krzem::core::ASTObject krzem::core::parse_ast(krzem::core::FileObject fo,bool* e,krzem::core::CallStack cs){
	krzem::core::ASTObject o={fo,{ASTObjectNode::NODE_TYPE_ROOT}};
	krzem::core::ASTToken t=krzem::core::next_token(fo.dt,{});
	krzem::core::IntermediateProgramStructure ips;
	while (true){
		std::cout<<t.type<<": "<<t.value<<"\n";
		switch (t.type){
			case krzem::core::TOKEN_TYPE_UNKNOWN:
			default:
				krzem::core_api::error::raise(krzem::core::create_error("UnexpectedCharacterError","Unexpected character '"+t.value+"'.",krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,t),krzem::core::token_to_line(fo.dt,t),krzem::core::get_last_func_name(cs)})));
				*e=true;
				return {};
			case krzem::core::TOKEN_TYPE_KEYWORD:
				if (t.value=="use"){
					krzem::core::ASTToken m_t=krzem::core::next_token(fo.dt,t);
					if (m_t.type==krzem::core::TOKEN_TYPE_ERROR){
						krzem::core_api::error::raise(krzem::core::create_error("SyntaxError",m_t.value,krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,m_t),krzem::core::token_to_line(fo.dt,m_t),krzem::core::get_last_func_name(cs)})));
						return o;
					}
					else if (m_t.type!=krzem::core::TOKEN_TYPE_MODULE_INDENTIFIER){
						krzem::core_api::error::raise(krzem::core::create_error("UnexpectedCharacterError","Expected `a Module Indenifier, found '"+m_t.value+"'.",krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,t),krzem::core::token_to_line(fo.dt,m_t),krzem::core::get_last_func_name(cs)})));
						*e=true;
						return o;
					}
					krzem::core::ASTToken e_t=krzem::core::next_token(fo.dt,m_t);
					if (e_t.type==krzem::core::TOKEN_TYPE_ERROR){
						krzem::core_api::error::raise(krzem::core::create_error("SyntaxError",e_t.value,krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,e_t),krzem::core::token_to_line(fo.dt,e_t),krzem::core::get_last_func_name(cs)})));
						return o;
					}
					else if (e_t.type==krzem::core::TOKEN_TYPE_SEMICOLON){
						std::string m_fp=krzem::core::find_module(m_t.value);
						if (m_fp.length()==0){
							krzem::core_api::error::raise(krzem::core::create_error("ModuleNotFoundError","Module '"+m_t.value+"' not Found.",krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,t),krzem::core::token_to_line(fo.dt,e_t),krzem::core::get_last_func_name(cs)})));
							*e=true;
							return o;
						}
						ips.vl[krzem::core::get_module_name(m_t.value)]=krzem::core::object_from_module(m_fp,krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,t),krzem::core::token_to_line(fo.dt,e_t),krzem::core::get_last_func_name(cs)}));
					}
					else if (e_t.type!=krzem::core::TOKEN_TYPE_KEYWORD||e_t.value!="as"){
						krzem::core_api::error::raise(krzem::core::create_error("UnexpectedCharacterError","Expected a Keyword ('as') or a Semicolon (';'), found '"+e_t.value+"'.",krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,e_t),krzem::core::token_to_line(fo.dt,e_t),krzem::core::get_last_func_name(cs)})));
						*e=true;
						return o;
					}
					else{
						krzem::core::ASTToken o_nm_t=krzem::core::next_token(fo.dt,e_t);
						if (o_nm_t.type==krzem::core::TOKEN_TYPE_ERROR){
							krzem::core_api::error::raise(krzem::core::create_error("SyntaxError",o_nm_t.value,krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,o_nm_t),krzem::core::token_to_line(fo.dt,o_nm_t),krzem::core::get_last_func_name(cs)})));
							return o;
						}
						else if (o_nm_t.type!=krzem::core::TOKEN_TYPE_MODULE_NAME_INDENTIFIER){
							krzem::core_api::error::raise(krzem::core::create_error("UnexpectedCharacterError","Expected Module Name Indenifier, found '"+o_nm_t.value+"'.",krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,o_nm_t),krzem::core::token_to_line(fo.dt,o_nm_t),krzem::core::get_last_func_name(cs)})));
							*e=true;
							return o;
						}
						e_t=krzem::core::next_token(fo.dt,o_nm_t);
						if (e_t.type==krzem::core::TOKEN_TYPE_ERROR){
							krzem::core_api::error::raise(krzem::core::create_error("SyntaxError",e_t.value,krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,e_t),krzem::core::token_to_line(fo.dt,e_t),krzem::core::get_last_func_name(cs)})));
							return o;
						}
						else if (e_t.type!=krzem::core::TOKEN_TYPE_SEMICOLON){
							krzem::core_api::error::raise(krzem::core::create_error("UnexpectedCharacterError","Expected a Semicolon (';'), found '"+e_t.value+"'.",krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,e_t),krzem::core::token_to_line(fo.dt,e_t),krzem::core::get_last_func_name(cs)})));
							*e=true;
							return o;
						}
						std::string m_fp=krzem::core::find_module(m_t.value);
						if (m_fp.length()==0){
							krzem::core_api::error::raise(krzem::core::create_error("ModuleNotFoundError","Module '"+m_t.value+"' not Found.",krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,t),krzem::core::token_to_line(fo.dt,e_t),krzem::core::get_last_func_name(cs)})));
							*e=true;
							return o;
						}
						krzem::core::Object mo=krzem::core::object_from_module(m_fp,krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,t),krzem::core::token_to_line(fo.dt,e_t),krzem::core::get_last_func_name(cs)}));
						if (o_nm_t.value!="*"){
							ips.vl[o_nm_t.value]=mo;
						}
						else{
							for (std::map<std::string,krzem::core::Object>::iterator mso=mo.properties.begin();mso!=mo.properties.end();mso++){
								ips.vl[mso->first]=mso->second;
							}
						}
					}
					t=e_t;
				}
			case krzem::core::TOKEN_TYPE_ERROR:
				krzem::core_api::error::raise(krzem::core::create_error("SyntaxError",t.value,krzem::core::extend_call_stack(cs,{fo,krzem::core::token_to_line(fo.dt,t),krzem::core::token_to_line(fo.dt,t),krzem::core::get_last_func_name(cs)})));
				return o;
			case krzem::core::TOKEN_TYPE_END_OF_DATA:
				return o;
		}
		t=next_token(fo.dt,t);
	}
}



std::string krzem::core::find_module(std::string nm){
	std::replace(nm.begin(),nm.end(),'/','\\');
	for (std::map<std::string,krzem::core::Object>::iterator i=native_modules.begin();i!=native_modules.end();i++){
		if (i->first=="native:"+nm){
			return "native:"+nm;
		}
	}
	size_t i=0;
	size_t j;
	std::string t;
	while (true){
		j=krzem::core_api::system::path.substr(i).find(";");
		t=krzem::core_api::system::path.substr(i,j)+(krzem::core_api::system::path[(j==-1?krzem::core_api::system::path.length():i+j)-1]!='\\'?"\\":"")+nm+".";
		if (krzem::platform_api::file_exists((t+"FILE").c_str())==true){
			return t+"FILE";
		}
		else if (krzem::platform_api::file_exists((t+"h").c_str())==true){
			// HEADER(.h) + DLL(.dll) //
		}
		if (j==-1){
			break;
		}
		i+=j+1;
	}
	return "";
}



std::string krzem::core::get_module_name(std::string nm){
	return nm;////////////////////////////////////////////
}



krzem::core::Object krzem::core::object_from_module(std::string fp,krzem::core::CallStack cs){
	if (krzem::core::native_modules.count(fp)==1){
		if (std::count(krzem::core::loaded_modules.begin(),krzem::core::loaded_modules.end(),fp)==0){
			krzem::core::loaded_modules.push_back(fp);
		}
		return krzem::core::native_modules[fp];
	}
	bool _e;
	krzem::core::loaded_modules.push_back(fp);
	return krzem::core::native_modules[fp]=krzem::core::compile_module(krzem::core::read_file(fp.c_str(),&_e,cs),cs);
}



krzem::core::Object krzem::core::compile_module(krzem::core::FileObject fo,krzem::core::CallStack cs){
	return {};
}



krzem::core::Object krzem::core::patch_object(krzem::core::Object o){
	return o;/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}



inline krzem::core::Object krzem::core::create_error(std::string nm,std::string msg,krzem::core::CallStack cs){
	return krzem::core::patch_object({krzem::core::OBJECT_TYPE_ERROR,krzem::core::OBJECT_MODIFIER_NONE,nullptr,{},{{"name",krzem::core::constant<std::string>(&std::string(nm))},{"msg",krzem::core::constant<std::string>(&std::string(msg))},{"cs",krzem::core::constant<krzem::core::CallStack>(&cs)}}});
}



krzem::core::CallStack krzem::core::extend_call_stack(krzem::core::CallStack cs,krzem::core::CallStackElem e){
	krzem::core::CallStack o(cs);
	o.push_back(e);
	return o;
}



inline std::string krzem::core::get_last_func_name(krzem::core::CallStack cs){
	return (cs.size()==0?"":cs[cs.size()-1].f);
}



size_t krzem::core::token_to_line(std::string dt,krzem::core::ASTToken t){
	size_t i=0;
	size_t j=0;
	while (true){
		if ((i==0?0:i-1)<t._off&&t._off<i+dt.substr(i).find("\n")){
			return j;
		}
		i+=dt.substr(i).find("\n")+1;
		j++;
	}
}



krzem::core::Object krzem::core::get_type(krzem::core::Object o){
	std::string t;
	switch (o.type){
		case krzem::core::OBJECT_TYPE_NONE:
			t="None";
			break;
		case krzem::core::OBJECT_TYPE_STRING:
			t="String";
			break;
		case krzem::core::OBJECT_TYPE_CHAR:
			t="Char";
			break;
		case krzem::core::OBJECT_TYPE_INT:
			t="Int";
			break;
		case krzem::core::OBJECT_TYPE_FLOAT:
			t="Float";
			break;
		case krzem::core::OBJECT_TYPE_FUNCTION:
			t="Function";
			break;
		case krzem::core::OBJECT_TYPE_NATIVE_FUNCTION:
			t="Native Function";
			break;
		case krzem::core::OBJECT_TYPE_MODULE:
			t="Module";
			break;
		case krzem::core::OBJECT_TYPE_CLASS:
			t="Class";
			break;
		case krzem::core::OBJECT_TYPE_ERROR:
			t="Error";
			break;
		case krzem::core::OBJECT_TYPE_TYPE:
			t="Type";
			break;
		default:
			t="Unknown";
			break;
	}
	return {krzem::core::OBJECT_TYPE_TYPE,krzem::core::OBJECT_MODIFIER_NONE,nullptr,{},{{"type",&t}}};
}



void* krzem::core::get_attr(krzem::core::Object o,std::string k,void* d){
	if (o.properties.count(k)!=0){
		return &o.properties[k];
	}
	if (o.i_properties.count(k)!=0){
		return o.i_properties[k];
	}
	return d;
}



template<typename T> inline void* krzem::core::constant(T* p){
	return new T(*p);
}



char krzem::core::get_next_char(std::string s,size_t* i,unsigned int* e){
	if (s[0]=='\\'){
		(*i)+=2;
		switch (s[1]){
			case 'b':
				return 0x08;
			case 'd':
				{
					char o=0;
					bool f=false;
					while (s.substr((*i),1).find_first_of("0123456789")==0){
						if (o<255){
							f=true;
							o=o*10+std::stoi(s.substr((*i),1));
							if ((int)o*10+std::stoi(s.substr((*i),1))>255){
								o=255;
							}
						}
						(*i)++;
					}
					if (f==false){
						(*e)=2;
						return 0;
					}
					return o;
				}
			case 'e':
				return 0x1b;
			case 'f':
				return 0x0c;
			case 'n':
				return 0x0a;
			case 'o':
				{
					char o=0;
					bool f=false;
					while (s.substr((*i),1).find_first_of("01234567")==0){
						if (o<255){
							f=true;
							o=o*8+std::stoi(s.substr((*i),1));
							if ((int)o*8+std::stoi(s.substr((*i),1))>255){
								o=255;
							}
						}
						(*i)++;
					}
					if (f==false){
						(*e)=3;
						return 0;
					}
					return o;
				}
			case 'r':
				return 0x0d;
			case 't':
				return 0x09;
			case 'v':
				return 0x0b;
			case 'x':
				{
					char o=0;
					bool f=false;
					while (s.substr((*i),1).find_first_of("0123456789abcdef")==0){
						if (o<255){
							f=true;
							o=o*16+std::string("0123456789abcdef").find(s[(*i)]);
							if ((int)o*16+std::string("0123456789abcdef").find(s[(*i)])>255){
								o=255;
							}
						}
						(*i)++;
					}
					if (f==false){
						(*e)=4;
						return 0;
					}
					return o;
				}
			case '\'':
				return 0x27;
			case '\"':
				return 0x22;
			default:
				(*i)--;
				(*e)=1;
				return 0;
		}
	}
	(*i)++;
	return s[0];
}
