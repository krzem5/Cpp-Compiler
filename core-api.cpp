#include <core.h>
#include <core-api.h>
#include <platform.h>
#include <iostream>



void krzem::core_api::system::out(krzem::core::Object* cls,krzem::core::Object* a){
	std::cout<<sizeof((*a).value)<<"\n";
}



void krzem::core_api::system::err(krzem::core::Object* cls,krzem::core::Object* a){
	std::cerr<<"E: "<<sizeof((*a).value)<<"\n";
}



void krzem::core_api::error::raise(krzem::core::Object* cls,krzem::core::Object* a){
	if ((*a).type!=krzem::core::OBJECT_TYPE_ERROR){
		krzem::core_api::error::raise_internal({std::string("TypeError"),std::string("Expected Type 'error', found Type '")+*static_cast<std::string*>(krzem::core::get_attr(krzem::core::get_type(*a),"type",&std::string("Undefined")))+"'.",krzem::core::CallStack()});
	}
	else{
		krzem::core_api::error::raise_internal({*static_cast<std::string*>(krzem::core::get_attr((*a),"name",&std::string("Error"))),*static_cast<std::string*>(krzem::core::get_attr((*a),"msg",&std::string(""))),*static_cast<krzem::core::CallStack*>((*a).i_properties["cs"])});
	}
}



void krzem::core_api::error::raise_internal(krzem::core::ErrorObject e){
	size_t j;
	unsigned int k;
	std::string ln;
	for (krzem::core::CallStack::iterator i=e.cs.begin();i!=e.cs.end();i++){
		j=0;
		k=0;
		while (true){
			j+=(*i).o.dt.substr(j).find("\n")+1;
			if (k==(*i).sl){
				ln="";
				while (true){
					ln+="\n  "+(*i).o.dt.substr((*i).o.dt.substr(0,j-1).rfind("\n")+1,j-(*i).o.dt.substr(0,j-1).rfind("\n")-2);
					if (k==(*i).el){
						break;
					}
					j+=(*i).o.dt.substr(j).find("\n")+1;
					k++;
				}
				std::cerr<<"File '"<<krzem::platform_api::get_full_path((*i).o.fp.c_str())<<((*i).sl==(*i).el?std::string("', Line ")+std::to_string((*i).sl+1):std::string("', Lines ")+std::to_string((*i).sl+1)+"-"+std::to_string((*i).el+1))<<((*i).f.length()>0?std::string(" in '")+(*i).f+"':":":")<<ln<<"\n";
				break;
			}
			k++;
		}
	}
	std::cerr<<e.nm<<": "<<e.msg<<"\n";
	std::cerr.flush();
}
