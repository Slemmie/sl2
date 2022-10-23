// prepares a c++ source file using sl2 headers for online judge submission
// substitutes things like '#include <sl2/...>' into the actual code

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <set>

struct Arg_info {
	
	// file path
	std::string input_file;
	
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
	
	// keep non sl2/ - '#include' lines
	bool keep_include = false;
	
	// print usage information
	bool help_flag = false;
	
};

// parse command line arguments
Arg_info parse_clargs(int argc, const char** argv) noexcept;

struct Source_info {
	
	// input file source code
	// line by line
	std::vector <std::string> input_source;
	
	// output source code
	// to be written to output file
	std::string output_source;
	
};

// print help (script usage information)
void print_help() noexcept;

// read input file from 'arg_info' and fill 'input_source' buffer in returned object
Source_info get_input_source(const Arg_info& arg_info) noexcept;

// parse input source code and generate output source code buffer
void parse_source_code(const Arg_info& arg_info, Source_info& source_info) noexcept;

// write output source code to output file
void write_to_output_file(const Arg_info& arg_info, const Source_info& source_info) noexcept;

// UTILITY

// read a file line by line
// may throw exception: throws error message string
// strips each line of trailing newline characters
std::vector <std::string> read_file(const std::string& file_path);

// get absolute path of sl2 install directory
// a.k.a. where all the headers are ...
// should not include the 'sl2' part
#ifndef INSTALL_DIR
#error "missing install directory macro"
#endif
std::string abs_install_dir() noexcept;

// std::string::find_first_of() but match entire string
size_t find_first_of(const std::string& str, const std::string& pattern) noexcept;

// convert Arg_info::cxx_version to string form
std::string cxx_version_string(const Arg_info& arg_info) noexcept;

int main(int argc, char** argv) {
	Arg_info arg_info = parse_clargs(argc, (const char**) argv);
	
	if (arg_info.help_flag) {
		print_help();
		
		return 0;
	}
	
	std::cerr << "[info]: C++ version: " << cxx_version_string(arg_info) << std::endl;
	if (arg_info.keep_include) {
		std::cerr << "[info]: keeping additional includes" << std::endl;
	}
	std::cerr << "\nfetching input source..." << std::endl;
	
	Source_info source_info = get_input_source(arg_info);
	
	std::cerr << "parsing..." << std::endl;
	
	parse_source_code(arg_info, source_info);
	
	std::cerr << "writing to '" << arg_info.output_file << "'..." << std::endl;
	
	write_to_output_file(arg_info, source_info);
	
	std::cerr << "done" << std::endl;
}

Arg_info parse_clargs(int argc, const char** argv) noexcept {
	// require at least an input file
	if (argc < 2) {
		std::cerr << "[fatal]: missing input file (get help with '$ pre --help')" << std::endl;
		
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
				std::cerr << "[fatal]: multiple occurrences of '-o' flag (get help with '$ pre --help')" << std::endl;
				
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
		
		if (args[i] == "-ki" || args[i] == "--keep_include" || args[i] == "--keep_includes") {
			result.keep_include = true;
			
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
				std::cerr << "[fatal]: expected output file after '-o' flag (get help with '$ pre --help')" << std::endl;
				
				exit(EXIT_FAILURE);
			}
			
			result.output_file = args[++i];
			
			continue;
		}
		
		if (!result.input_file.empty()) {
			// do not allow multiple input files
			// if '-o' flag is used, possible output file interpretation is not allowed
			if (has_o_flag || !result.output_file.empty()) {
				std::cerr << "[fatal]: multiple input files specified (get help with '$ pre --help')" << std::endl;
				
				exit(EXIT_FAILURE);
			}
			
			// in case no '-o' flag is specified and an input file has been set already: assume this is the output file
			result.output_file = args[i];
			
			continue;
		}
		
		result.input_file = args[i];
	}
	
	// check for collected information
	if (result.input_file.empty() && !result.help_flag) {
		std::cerr << "[fatal]: missing input file (get help with '$ pre --help')" << std::endl;
		
		exit(EXIT_FAILURE);
	}
	if (result.output_file.empty()) {
		// do not log for now
		//std::cerr << "[info]: output file defualting to 'out.cpp'" << std::endl;
		
		result.output_file = "out.cpp";
	}
	
	return result;
}

void print_help() noexcept {
	std::cout << " -- pre (sl2) help --" << std::endl;
	std::cout << std::endl;
	std::cout << "  the script recursively unfolds sl2 headers in a C++ source file such that it may be submitted to an online judge" << std::endl;
	std::cout << "  this ensures that the judge can compile the source file without 'unrecognized file' errors from include statements" << std::endl;
	std::cout << std::endl;
	std::cout << "  usage:" << std::endl;
	std::cout << "  $ pre {flags}" << std::endl;
	std::cout << std::endl;
	std::cout << "  '-h'/'-help'/'--help'                                           " <<
	"-  show this message" << std::endl;
	std::cout << "  '20'/'2a'/'c++20'/'C++20'/'c++2a'/'C++2a'/'gnu++20'/'gnu++2a'   " <<
	"-  prepare file using C++20 sl2 headers (this C++ version is default if no other specification is given)" << std::endl;
	std::cout << "  '17'/'c++17'/'C++17''gnu++17'                                   " <<
	"-  prepare file using C++17 sl2 headers" << std::endl;
	std::cout << "  '-old'/'c++old'/'C++old'/'c++<14/11/03/98>'/'C++<14/11/03/98>'  " <<
	"-  prepare file using older C++ sl2 headers (defaults to C++17)" << std::endl;
	std::cout << "  '-ki'/'--keep_include'/'--keep_includes'                        " <<
	"-  keep unrecognized (like stl) includes from sl2 headers (default: will be removed)" << std::endl;
	std::cout << "  '-o'                                                            " <<
	"-  should be followed by a filepath to where the unfolded file should be put" << std::endl;
	std::cout << std::endl;
	std::cout << "  any other arguments will be interpreted as filepaths, first filepath should be the input file, second should be the output filepath" << std::endl;
	std::cout << "  if no output filepath is specified, default to ./out.cpp" << std::endl;
	std::cout << "  if output filepath does not exist, it will be created (the directory specified must exist)" << std::endl;
}

Source_info get_input_source(const Arg_info& arg_info) noexcept {
	Source_info result;
	
	try {
		result.input_source = read_file(arg_info.input_file);
	} catch (const std::string& e) {
		std::cerr << "[fatal]: " << e << "\n";
		
		exit(EXIT_FAILURE);
	}
	
	return result;
}

struct sl2_header_info {
	
	// line of include statement within source code snippet local to whereever the struct instance lies
	size_t line;
	
	// file path relative to whereever sl2 directory exists
	std::string file_path;
	
};

void parse_source_code(const Arg_info& arg_info, Source_info& source_info) noexcept {
	std::set <std::string> already_unfolded_sl2_headers;
	
	// look for any include statements that include an sl2 header and unfold them
	// ... recursively
	auto unfold_sl2_headers = [&already_unfolded_sl2_headers, &arg_info]
	(auto&& self, const std::vector <std::string>& source_code, bool is_root = false) -> std::vector <std::string> {
		std::vector <sl2_header_info> header_info;
		
		// find all include statements that include an sl2 header
		
		// used to tell whether the current code is inside a comment section
		// -> can be multi line ...
		bool comment_section = false;
		
		for (size_t i = 0; i < source_code.size(); i++) {
			// create a version of the line that exclude anything commented out
			std::string line;
			bool full_comment = false;
			for (size_t j = 0; j < source_code[i].size() && !full_comment; j++) {
				if (!comment_section && source_code[i][j] == '/' && i + 1 < source_code[i].size() && source_code[i][j + 1] == '/') {
					full_comment = true;
					
					continue;
				}
				
				if (!comment_section && source_code[i][j] == '/' && i + 1 < source_code[i].size() && source_code[i][j + 1] == '*') {
					comment_section = true;
					
					continue;
				}
				
				if (comment_section && source_code[i][j] == '*' && i + 1 < source_code[i].size() && source_code[i][j + 1] == '/') {
					comment_section = false;
					
					j++;
					
					continue;
				}
				
				if (!comment_section) {
					line += source_code[i][j];
				}
			}
			
			size_t begin_angular_bracket = std::string::npos;
			size_t end_angular_bracket = std::string::npos;
			size_t first_quote = line.find_first_of("\"");
			
			// in case this line is an include statement and it includes something with prefix "<sl2/"
			if (find_first_of(line, "#include") != std::string::npos &&
			(begin_angular_bracket = find_first_of(line, "<")) != std::string::npos &&
			(end_angular_bracket = find_first_of(line, ">")) != std::string::npos &&
			begin_angular_bracket + std::string("sl2/").size() < end_angular_bracket &&
			line.substr(begin_angular_bracket + 1, std::string("sl2/").size()) == "sl2/" &&
			first_quote == std::string::npos) {
				std::string header_path =
				std::string(line.begin() + begin_angular_bracket + std::string("<sl2/").size(), line.begin() + end_angular_bracket);
				
				bool is_h_file = header_path.find_last_of('.') != std::string::npos;
				
				if (is_h_file) {
					if (!header_path.starts_with("c++17/") && !header_path.starts_with("c++20/")) {
						if (arg_info.cxx_version == Arg_info::CXX_VERSION::V_20 || arg_info.cxx_version == Arg_info::CXX_VERSION::V_DEFUALT) {
							header_path = "c++20/" + header_path;
						} else {
							header_path = "c++17/" + header_path;
						}
					}
				} else {
					if (!std::filesystem::exists(abs_install_dir() + "/sl2/" + header_path)) {
						std::cerr << "[fatal]: header file '" << abs_install_dir() << "/sl2/" << header_path << "' was not found" << std::endl;
						
						exit(EXIT_FAILURE);
					}
					
					if (arg_info.cxx_version == Arg_info::CXX_VERSION::V_20 || arg_info.cxx_version == Arg_info::CXX_VERSION::V_DEFUALT) {
						header_path = "c++20/" + header_path + ".h";
					} else {
						header_path = "c++17/" + header_path + ".h";
					}
				}
				
				header_path = "sl2/" + header_path;
				
				// make sure file exists
				if (!std::filesystem::exists(abs_install_dir() + "/" + header_path)) {
					std::cerr << "[fatal]: header file '" << abs_install_dir() << "/" << header_path << "' was not found" << std::endl;
					
					exit(EXIT_FAILURE);
				}
				
				// add entry
				header_info.emplace_back(i, abs_install_dir() + "/" + header_path);
			}
		}
		
		std::vector <std::string> result;
		
		comment_section = false;
		size_t hi_ptr = 0;
		
		for (size_t i = 0; i < source_code.size(); i++) {
			// create a version of the line that exclude anything commented out
			std::string line;
			// line with only comments
			std::string comments_before_pound;
			std::string comments_after_pound;
			bool pound_found = false;
			bool full_comment = false;
			for (size_t j = 0; j < source_code[i].size() && !full_comment; j++) {
				if (!comment_section && source_code[i][j] == '/' && i + 1 < source_code[i].size() && source_code[i][j + 1] == '/') {
					full_comment = true;
					
					(pound_found ? comments_after_pound : comments_before_pound) += source_code[i][j];
					
					continue;
				}
				
				if (!comment_section && source_code[i][j] == '/' && i + 1 < source_code[i].size() && source_code[i][j + 1] == '*') {
					comment_section = true;
					
					(pound_found ? comments_after_pound : comments_before_pound) += source_code[i][j];
					
					continue;
				}
				
				if (comment_section && source_code[i][j] == '*' && i + 1 < source_code[i].size() && source_code[i][j + 1] == '/') {
					comment_section = false;
					
					(pound_found ? comments_after_pound : comments_before_pound) += source_code[i][j];
					
					(pound_found ? comments_after_pound : comments_before_pound) += source_code[i][++j];
					
					continue;
				}
				
				if (!comment_section) {
					line += source_code[i][j];
					
					pound_found |= source_code[i][j] == '#';
				}
			}
			
			// always exclude '#pragma once' lines
			if (find_first_of(line, "#pragma") != std::string::npos &&
			find_first_of(line, "once") != std::string::npos &&
			find_first_of(line, "#pragma") < find_first_of(line, "once")) {
				result.emplace_back(comments_before_pound + comments_after_pound);
				
				continue;
			}
			
			if (hi_ptr < header_info.size() && header_info[hi_ptr].line == i) {
				result.emplace_back(comments_before_pound);
				result.emplace_back("");
				try {
					std::string rel_path = header_info[hi_ptr].file_path.substr(abs_install_dir().size() + std::string("/sl2/c++xx/").size());
					if (!already_unfolded_sl2_headers.count(rel_path)) {
						already_unfolded_sl2_headers.insert(rel_path);
						const std::vector <std::string> source = self(self, read_file(header_info[hi_ptr].file_path));
						result.insert(result.end(), source.begin(), source.end());
					}
				} catch (const std::string& e) {
					std::cerr << "[fatal]: " << e << std::endl;
					
					exit(EXIT_FAILURE);
				}
				result.emplace_back("");
				result.emplace_back(comments_after_pound);
				
				hi_ptr++;
			} else {
				// potentially exclude non sl2/ - '#include' lines
				if (find_first_of(line, "#include") != std::string::npos && !arg_info.keep_include && !is_root) {
					continue;
				}
				
				result.emplace_back(source_code[i]);
			}
		}
		
		return result;
	};
	
	std::vector <std::string> result = unfold_sl2_headers(unfold_sl2_headers, source_info.input_source, true);
	
	std::vector <std::string> stripped_result;
	
	// remove duplicate newline entries
	for (const std::string& line : result) {
		if (line == "" && (stripped_result.empty() || stripped_result.back() == "\n")) {
			continue;
		}
		
		stripped_result.emplace_back(line + "\n");
	}
	
	// craete single string from stripped result
	std::string final_result;
	
	for (const std::string& line : stripped_result) {
		final_result += line;
	}
	
	source_info.output_source = final_result;
}

void write_to_output_file(const Arg_info& arg_info, const Source_info& source_info) noexcept {
	try {
		std::ofstream outfile;
		
		outfile.open(arg_info.output_file);
		
		if (!outfile.is_open()) {
			throw "failed to open file '" + arg_info.output_file + "'";
		}
		
		outfile << source_info.output_source;
		
		outfile.close();
	} catch (const std::string& e) {
		std::cerr << "[fatal]: " << e << std::endl;
		
		exit(EXIT_FAILURE);
	}
}

// UTILITY

std::vector <std::string> read_file(const std::string& file_path) {
	std::ifstream infile;
	
	infile.open(file_path);
	
	if (!infile.is_open()) {
		throw (std::string) ("failed to open file '" + file_path + "'");
	}
	
	std::vector <std::string> result;
	
	std::string line;
	while (std::getline(infile, line)) {
		// strip line of newline characters at the end
		while (!line.empty() && line.back() == '\n') {
			line.pop_back();
		}
		
		result.emplace_back(line);
	}
	
	infile.close();
	
	return result;
}

std::string abs_install_dir() noexcept {
#define STRING_QUOTE(x) (#x)
#define STRING_VERSION(x) STRING_QUOTE(x)
	return STRING_VERSION(INSTALL_DIR);
#undef STRING_QUOTE
#undef STRING_VERSION
}

size_t find_first_of(const std::string& str, const std::string& pattern) noexcept {
	if (pattern.empty()) {
		return std::string::npos;
	}
	
	for (size_t i = 0; i + pattern.size() <= str.size(); i++) {
		if (pattern == str.substr(i, pattern.size())) {
			return i;
		}
	}
	
	return std::string::npos;
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
