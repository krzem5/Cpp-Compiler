#include <platform.h>
#include <string>
#include <iostream>
#if defined(_MSC_VER)
#define WINDOWS
#include <windows.h>
#elif defined(__GNUC__)
#define LINUX
#include <fstream>
#include <unistd.h>
#endif



bool krzem::platform_api::file_exists(const char* fp){
#if defined(WINDOWS)
	WIN32_FIND_DATA f;
	HANDLE h=FindFirstFile(fp,&f);
	if (h!=INVALID_HANDLE_VALUE){
		FindClose(h);
	}
	return (h==INVALID_HANDLE_VALUE?false:true);
#elif defined(LINUX)
	return (access(fp,F_OK)!=-1?true:false);
#endif
}



std::string krzem::platform_api::get_cwd(){
#if defined(WINDOWS)
	std::string o=std::string(MAX_PATH,'\0');
	GetCurrentDirectoryA(MAX_PATH,&o[0]);
	return o.substr(0,o.find('\0'));
#elif defined(LINUX)
	char tmp[MAXPATHLEN];
	return (getcwd(tmp,MAXPATHLEN)?std::string(tmp):std::string(""));
#endif
}



std::string krzem::platform_api::get_full_path(const char* fp){
#if defined(WINDOWS)
	char* t;
	std::string o=std::string(MAX_PATH,'\0');
	GetFullPathNameA(fp,MAX_PATH,&o[0],&t);
	return o.substr(0,o.find('\0'));
#elif defined(LINUX)
#endif
}



std::string krzem::platform_api::read_file_content(const char* fp){
#if defined(WINDOWS)
	HANDLE fh=CreateFileA(fp,GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);
	std::string o=std::string(GetFileSize(fh,nullptr),'\0');
	unsigned long br;
	ReadFile(fh,&o[0],o.length(),&br,nullptr);
	CloseHandle(fh);
	return o;
#elif defined(LINUX)
	std::ifstream ifs(fp);
	return std::string(std::istreambuf_iterator<char>(ifs),std::istreambuf_iterator<char>());
#endif
}



// HMODULE dll=LoadLibraryExA(fp.c_str(),nullptr,DONT_RESOLVE_DLL_REFERENCES);
// _LOADED_IMAGE dll_i;
// MapAndLoad(fp.c_str(),nullptr,&dll_i,true,true);
// unsigned long ds;
// _IMAGE_EXPORT_DIRECTORY* ex_d=(_IMAGE_EXPORT_DIRECTORY*)
// ImageDirectoryEntryToData(dll_i.MappedAddress,
// false,IMAGE_DIRECTORY_ENTRY_EXPORT,&ds);
// unsigned long* nm_rva=(unsigned long*)ImageRvaToVa(dll_i.FileHeader,dll_i.MappedAddress,ex_d->AddressOfNames,nullptr);
// for (unsigned int i=0;i<ex_d->NumberOfNames;i++){
// 	std::string f_nm=(char*)ImageRvaToVa(dll_i.FileHeader,dll_i.MappedAddress,nm_rva[i],nullptr);
// 	// GetProcAddress(dll,f_nm.c_str());
// 	std::cout<<"Function: "<<f_nm<<"\n";
// }
// UnMapAndLoad(&dll_i);
// FreeLibrary(dll);
// return {};
