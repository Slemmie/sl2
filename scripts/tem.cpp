// copies the sl2 default template C++ file to the desired destination
// has seperate C++17 and C++20 versions

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

struct Arg_info {
	
	// file path
	std::string output_file;
	
	enum class CXX_VERSION {
		
		V_20, // C++ 20
		V_17, // C++ 17
		V_OLD, // < C++ 17 (C++ 17 will be headers used)
		
		V_DEFUALT // C++ 20
		
	};
	
	// C++ version used
	CXX_VERSION cxx_version = CXX_VERSION::V_DEFUALT;
	
	// print usage information
	bool help_flag = false;
	
};

// parse command line arguments
Arg_info parse_clargs(int argc, const char** argv) noexcept;

// print help (script usage information)
void print_help() noexcept;

// copy the correct file to the correct destination
void copy_file(const Arg_info& arg_info) noexcept;

// UTILITY

// get absolute path of sl2 install directory
// a.k.a. where the template.cpp files are ...
// should not include the 'sl2' part
#ifndef INSTALL_DIR
#error "missing install directory macro"
#endif
std::string abs_install_dir() noexcept;

// convert Arg_info::cxx_version to string form
std::string cxx_version_string(const Arg_info& arg_info) noexcept;

int main(int argc, char** argv) {
	Arg_info arg_info = parse_clargs(argc, (const char**) argv);
	
	if (arg_info.help_flag) {
		print_help();
		
		return 0;
	}
	
	std::cerr << "[info]: C++ version: " << cxx_version_string(arg_info) << std::endl;
	std::cerr << "copying to '" << arg_info.output_file << "'..." << std::endl;
	
	copy_file(arg_info);
	
	std::cerr << "done" << std::endl;
}

Arg_info parse_clargs(int argc, const char** argv) noexcept {
	// require at least the output file
	if (argc < 2) {
		std::cerr << "[fatal]: missing output file (get help with '$ tem --help')" << std::endl;
		
		exit(EXIT_FAILURE);
	}
	
	Arg_info result;
	
	bool has_o_flag = false;
	
	// gather arguments and check for '-o' flag occurrence
	std::vector <std::string> args(argc - 1);
	for (int i = 1; i < argc; i++) {
		args[i - 1] = argv[i];
		
		if (args[i - 1] == "-o") {
			if (has_o_flag) {
				std::cerr << "[fatal]: multiple occurrences of '-o' flag (get help with '$ tem --help')" << std::endl;
				
				exit(EXIT_FAILURE);
			}
			
			has_o_flag = true;
		}
	}
	
	for (size_t i = 0; i < args.size(); i++) {
		if (args[i] == "-h" || args[i] == "-help" || args[i] == "--help") {
			result.help_flag = true;
			
			continue;
		}
		
		if (
		args[i] == "20" ||
		args[i] == "2a" ||
		args[i] == "c++20" ||
		args[i] == "C++20" ||
		args[i] == "c++2a" ||
		args[i] == "C++2a" ||
		args[i] == "gnu++20" ||
		args[i] == "gnu++2a") { // C++ version 20
			result.cxx_version = Arg_info::CXX_VERSION::V_20;
			
			continue;
		} else if (
		// only override 'result.cxx_version' if C++ 20 has not yet been specified
		result.cxx_version != Arg_info::CXX_VERSION::V_20 && (
		args[i] == "17" ||
		args[i] == "c++17" ||
		args[i] == "C++17" ||
		args[i] == "gnu++17")) { // C++ version 17
			result.cxx_version = Arg_info::CXX_VERSION::V_17;
			
			continue;
		} else if (
		// only override 'result.cxx_version' if neither C++ 20 nor C++ 17 had need specified yet
		result.cxx_version != Arg_info::CXX_VERSION::V_20 &&
		result.cxx_version != Arg_info::CXX_VERSION::V_17 && (
		args[i] == "-old" ||
		args[i] == "c++old" ||
		args[i] == "C++old" ||
		args[i] == "c++14" ||
		args[i] == "C++14" ||
		args[i] == "c++11" ||
		args[i] == "C++11" ||
		args[i] == "c++03" ||
		args[i] == "C++03" ||
		args[i] == "c++98" ||
		args[i] == "C++98")) { // old (potentially depricated) C++ version - will attempt to use C++ 17 headers
			result.cxx_version = Arg_info::CXX_VERSION::V_OLD;
			
			continue;
		}
		
		// expect output file
		if (args[i] == "-o") {
			if (i + 1 == args.size()) {
				std::cerr << "[fatal]: expected output file after '-o' flag (get help with '$ tem --help')" << std::endl;
				
				exit(EXIT_FAILURE);
			}
			
			result.output_file = args[++i];
			
			continue;
		}
		
		// do not allow multiple output files
		// if '-o' flag is used, this counts as multiple output file error
		if (has_o_flag || !result.output_file.empty()) {
			std::cerr << "[fatal]: multiple output files specified (get help with '$ tem --help')" << std::endl;
			
			exit(EXIT_FAILURE);
		}
		
		
		result.output_file = args[i];
	}
	
	// check for collected information
	if (result.output_file.empty() && !result.help_flag) {
		std::cerr << "[fatal]: missing output file (get help with '$ tem --help')" << std::endl;
		
		exit(EXIT_FAILURE);
	}
	
	return result;
}

void print_help() noexcept {
	std::cout << " -- tem (sl2) help --" << std::endl;
	std::cout << std::endl;
	std::cout << "  the script copies the sl2 standard template.cpp file to a desired destination" << std::endl;
	std::cout << std::endl;
	std::cout << "  usage:" << std::endl;
	std::cout << "  $ tem {flags}" << std::endl;
	std::cout << std::endl;
	std::cout << "  '-h'/'-help'/'--help'                                           " <<
	"-  show this message" << std::endl;
	std::cout << "  '20'/'2a'/'c++20'/'C++20'/'c++2a'/'C++2a'/'gnu++20'/'gnu++2a'   " <<
	"-  copy C++20 version of sl2 standard template.cpp (this C++ version is default if no other specification is given)" << std::endl;
	std::cout << "  '17'/'c++17'/'C++17''gnu++17'                                   " <<
	"-  copy C++17 version of sl2 standard template.cpp" << std::endl;
	std::cout << "  '-old'/'c++old'/'C++old'/'c++<14/11/03/98>'/'C++<14/11/03/98>'  " <<
	"-  copy older C++ version of sl2 standard template.cpp (defaults to C++17)" << std::endl;
	std::cout << "  '-o'                                                            " <<
	"-  should be followed by a filepath to where the sl2 standard template.cpp file should be put" << std::endl;
	std::cout << std::endl;
	std::cout << "  any other arguments will be interpreted as filepaths" << std::endl;
	std::cout << "  exactly one filepath should be specified (the output filepath)" << std::endl;
	std::cout << "  if output filepath does not exist, it will be created (the directory specified must exist)" << std::endl;
}

void copy_file(const Arg_info& arg_info) noexcept {
	const std::string cxx_version_path =
	arg_info.cxx_version == Arg_info::CXX_VERSION::V_20 ||
	arg_info.cxx_version == Arg_info::CXX_VERSION::V_DEFUALT ?
	"c++20" : "c++17";
	
	const std::string template_filepath = abs_install_dir() + "/sl2/template/" + cxx_version_path + "/template.cpp";
	
	try {
		std::filesystem::copy(template_filepath, arg_info.output_file);
	} catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "[fatal]: " << e.what() << std::endl;
		
		exit(EXIT_FAILURE);
	}
}

// UTILITY

std::string abs_install_dir() noexcept {
#define STRING_QUOTE(x) (#x)
#define STRING_VERSION(x) STRING_QUOTE(x)
	return STRING_VERSION(INSTALL_DIR);
#undef STRING_QUOTE
#undef STRING_VERSION
}

std::string cxx_version_string(const Arg_info& arg_info) noexcept {
	switch (arg_info.cxx_version) {
		case Arg_info::CXX_VERSION::V_DEFUALT:
		case Arg_info::CXX_VERSION::V_20: return "c++20";
		case Arg_info::CXX_VERSION::V_17: return "c++17";
		case Arg_info::CXX_VERSION::V_OLD: return "older (defaulting to c++17)";
		default: break;
	}
	
	return "[unknown]";
}
