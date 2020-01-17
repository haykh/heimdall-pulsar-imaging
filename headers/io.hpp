#ifndef INPUTOUTPUT
#define INPUTOUTPUT

void throw_error(const std::string msg);
double read_from_file (std::string input_name, const std::string param);
std::string read_from_file_str (std::string input_name, const std::string param, const std::string def_val);

void read_in_out(std::string &in, std::string &out, int argc, char* argv[]);

#endif
