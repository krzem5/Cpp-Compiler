#include <core.h>
#include <string>



namespace krzem::core_api::system{
	static std::string path="D:\\K\\Coding\\projects\\Cpp-Compiler\\lib\\;D:\\";



	void out(krzem::core::Object* al);



	void err(krzem::core::Object* al);
}



namespace krzem::core_api::error{
	void raise(krzem::core::Object e);
}
