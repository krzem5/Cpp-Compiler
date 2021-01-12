#include <string>



namespace krzem::platform_api{
	bool file_exists(const char* nm);



	std::string get_cwd();



	std::string get_full_path(const char* fp);



	std::string read_file_content(const char* fp);
}
