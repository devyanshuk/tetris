#include "../include/arghelper.hpp"

using namespace std;

void print_help(char * program_name) {
	cout << "usage: "  << program_name <<  " [options] { value }" << endl <<
	"Options:" << endl <<
	"-h | --help                            print this help message" << endl <<
	"-l | --length      { dimension }       specify length of game window (default: " << LENGTH << " )" << endl <<
	"-w | --width       { dimension }       specify width of game windwo (default: " << WIDTH << " )" << endl <<
	"-s | --show-hint   {   bool    }       shows the final position of a tetris block during gameplay" << endl;
}

/* returns false if help message was printed out in console */
bool read_args(ArgInput & input, int argc, char ** argv) {

	int ch;
	int index;
	const struct option long_options[] = {
		{ "width"     ,     required_argument ,  0, 'w' },
		{ "length"    ,     required_argument ,  0, 'l' },
		{ "help"      ,     no_argument       ,  0, 'h' },
		{ "show-hint" , 	required_argument ,  0, 's' },
		{ 0           ,     0                 ,  0,  0  }
	};

	while ((ch = getopt_long(argc, argv, "s:w:l:h", long_options, &index)) != -1)
	{
		switch (ch)
		{
			case '?':
			case 'h':
				print_help(argv[0]);
				return false;
			case 'w':
				input._width = atoi(optarg);
				break;
			case 'l':
				input._length = atoi(optarg);
				break;
			case 's':
				std::stringstream(optarg) >> std::boolalpha >> input._show_hint;
		}
	}
	return true;
}
