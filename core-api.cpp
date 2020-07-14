#include <core.h>
#include <core-api.h>
#include <platform.h>
#include <iostream>



void krzem::core_api::system::out(krzem::core::Object* al){
	std::cout<<sizeof(al)<<"\n";
}



void krzem::core_api::system::err(krzem::core::Object* al){
	std::cerr<<"E: "<<sizeof(al)<<"\n";
}



void krzem::core_api::error::raise(krzem::core::Object e){
	if (e.type!=krzem::core::OBJECT_TYPE_ERROR){
		e={krzem::core::OBJECT_TYPE_ERROR,krzem::core::OBJECT_MODIFIER_NONE,{},{{"name",krzem::core::constant<std::string>(&std::string("TypeError"))},{"msg",krzem::core::constant<std::string>(&(std::string("Expected Type 'error', found Type '")+*static_cast<std::string*>(krzem::core::get_attr(krzem::core::get_type(e),"type",&std::string("Undefined")))+"'."))},{"cs",krzem::core::constant<krzem::core::CallStack>(&krzem::core::CallStack())}}};
	}
	krzem::core::CallStack cs=*static_cast<krzem::core::CallStack*>(e.i_properties["cs"]);
	size_t j;
	unsigned int k;
	std::string ln;
	for (krzem::core::CallStack::iterator i=cs.begin();i!=cs.end();i++){
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
	std::cerr<<*static_cast<std::string*>(krzem::core::get_attr(e,"name",&std::string("Error")))<<": "<<*static_cast<std::string*>(krzem::core::get_attr(e,"msg",&std::string("")))<<"\n";
	std::cerr.flush();
}
