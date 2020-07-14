#include <core.h>



int main(int argc,const char** argv){
	krzem::core::init();
	bool e=false;
	krzem::core::FileObject fo=krzem::core::read_file(argv[1],&e,{});
	if (e==true){
		return 1;
	}
	krzem::core::ASTObject ast=krzem::core::parse_ast(fo,&e,{});
	if (e==true){
		return 1;
	}
	return 0;
}
