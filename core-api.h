#include <core.h>
#include <string>



namespace krzem::core_api::system{
	static std::string path="D:\\K\\Coding\\projects\\Cpp-Compiler\\lib\\;D:\\";



	void out(krzem::core::FunctionArgs a);



	void err(krzem::core::FunctionArgs a);
}



namespace krzem::core_api::error{
	void raise(krzem::core::FunctionArgs a);
}
