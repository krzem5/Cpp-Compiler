#ifndef CORE
#define CORE
#include <vector>
#include <string>
#include <map>



namespace krzem::core{
	enum TOKEN_TYPE:unsigned int{
		TOKEN_TYPE_UNKNOWN=0x00,
		TOKEN_TYPE_STRING=0x01,
		TOKEN_TYPE_CHAR=0x02,
		TOKEN_TYPE_MODULE_INDENTIFIER=0x03,
		TOKEN_TYPE_MODULE_NAME_INDENTIFIER=0x04,
		TOKEN_TYPE_NUMBER=0x05,
		TOKEN_TYPE_KEYWORD=0x06,
		TOKEN_TYPE_INDENTIFIER=0x07,
		TOKEN_TYPE_SEMICOLON=0x08,
		TOKEN_TYPE_ERROR=0x09,
		TOKEN_TYPE_END_OF_DATA=0x0a
	};



	enum OBJECT_TYPE:unsigned int{
		OBJECT_TYPE_NONE=0x00,
		OBJECT_TYPE_STRING=0x01,
		OBJECT_TYPE_CHAR=0x02,
		OBJECT_TYPE_INT=0x03,
		OBJECT_TYPE_FLOAT=0x04,
		OBJECT_TYPE_FUNCTION=0x05,
		OBJECT_TYPE_NATIVE_FUNCTION=0x06,
		OBJECT_TYPE_MODULE=0x07,
		OBJECT_TYPE_CLASS=0x08,
		OBJECT_TYPE_ERROR=0x09,
		OBJECT_TYPE_TYPE=0x0a
	};



	enum OBJECT_MODIFIER:unsigned int{
		OBJECT_MODIFIER_NONE=0x000,
		OBJECT_MODIFIER_PUBLIC=0x001,
		OBJECT_MODIFIER_PRIVATE=0x002,
		OBJECT_MODIFIER_STATIC=0x004,
		OBJECT_MODIFIER_EXPORT=0x008,
		OBJECT_MODIFIER_FROZEN=0x010,
		OBJECT_MODIFIER_FROZENTYPE=0x020,
		OBJECT_MODIFIER_ZERO_ARG=0x040,
		OBJECT_MODIFIER_ONE_ARG=0x080,
		OBJECT_MODIFIER_LIST_ARG=0x100,
		OBJECT_MODIFIER_NAME_ARG=0x200
	};



	struct Object{
		unsigned int type;
		unsigned int modifiers;
		void* value;
		std::map<std::string,Object> properties;
		std::map<std::string,void*> i_properties;
	};



	union FunctionArgs{
		void* zero;
		Object one;
		Object* list;
		std::map<std::string,Object> dict;
	};



	struct FileObject{
		std::string fp;
		std::string dt;
	};



	struct CallStackElem{
		FileObject o;
		size_t sl;
		size_t el;
		std::string f;
	};



	typedef std::vector<CallStackElem> CallStack;



	struct StackTrace{
		std::string nm;
		std::string msg;
		CallStack cs;
	};



	struct IntermediateProgramStructure{
		std::map<std::string,Object> vl;
	};



	struct ASTToken{
		unsigned int type;
		std::string value;
		size_t _off;
	};



	struct ASTObjectNode{
		enum ASTObjectNodeType:unsigned int{
			NODE_TYPE_UNKNOWN=0x00,
			NODE_TYPE_ROOT=0x01,
			NODE_TYPE_LIST=0x02,
			NODE_TYPE_SET_VAR=0x03,
			NODE_TYPE_COMPARE=0x04
		} t;
		std::vector<ASTObjectNode> ch;
		std::map<std::string,void*> p;
	};



	struct ASTObject{
		FileObject src;
		ASTObjectNode root;
	};



	void init();



	FileObject read_file(const char* fn,bool* e,CallStack cs);



	ASTToken next_token(std::string dt,ASTToken nxt);



	ASTObject parse_ast(FileObject fo,bool* e,CallStack cs);



	std::string find_module(std::string nm);



	std::string get_module_name(std::string nm);



	Object object_from_module(std::string fp,CallStack cs);



	Object compile_module(FileObject fo,CallStack cs);



	Object patch_object(Object o);



	inline Object create_error(std::string nm,std::string msg,CallStack cs);



	CallStack extend_call_stack(CallStack cs,CallStackElem e);



	inline std::string get_last_func_name(CallStack cs);



	size_t token_to_line(std::string dt,ASTToken t);



	Object get_type(Object o);



	void* get_attr(Object o,std::string k,void* d);



	template<typename T> inline void* constant(T* p);



	char get_next_char(std::string s,size_t* i,unsigned int* e);



	static std::map<std::string,Object> native_modules;



	static std::vector<std::string> loaded_modules;
}
#endif
