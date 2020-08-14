#include <core.h>



namespace krzem::core_api::system{
	static const char* path="D:\\K\\Coding\\projects\\Cpp-Compiler\\lib\\;";



	void out(krzem::core::Object* cls,krzem::core::Object* a);



	void err(krzem::core::Object* cls,krzem::core::Object* a);
}



namespace krzem::core_api::error{
	void raise(krzem::core::Object* cls,krzem::core::Object* a);



	void raise_internal(krzem::core::ErrorObject e);
}
