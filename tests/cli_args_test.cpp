#include <iostream>
#include "cli_args.hpp"

using std::cout;
using std::endl;
using ArgParser::CLI_Args;


void usage(char* av[], CLI_Args& ap)
{
    cout << endl << "Usage: " << av[0];
    cout << " " << *ap.exp_args();
    cout << endl << *ap.help();
    cout << endl;
}


int main(int ac, char* av[])
{

    cout << ac << "Parsing CLI Args: ";
    for (int ii = 0; ii < ac; ii++) {
        cout << endl << av[ii];
    }

    CLI_Args ap(ac, av);
    auto in_help = ap.add_flag("-h", "--help", "Show help");
    auto in_all = ap.add_flag("-a", "", "Process all");
    auto in_long = ap.add_flag("-l", "", "Long output");
    auto in_format = ap.add_str("-F", "", "Test flag");
    auto in_inval = ap.add_str("", "--in-val", "Test input value");

    try {
        cout << endl << "<-------- Parsed CLI Args -------->\n";
        ap.parse_args();

        if (*in_help == true) {
            usage(av, ap);
            return 0;
        }

        cout << endl << "in_all: " << *in_all;
        cout << endl << "in_long: " << *in_long;
        cout << endl << "in_format: " << *in_format;
        cout << endl << "in_val: " << *in_inval;
        cout << endl << "in_help: " << *in_help;
        cout << endl << "unnamed args: ";
        cout << endl << *ap.get_args();

        cout << endl << ">-------- Parsed CLI Args --------<\n";
    } catch (const std::exception& exc) {
        cout << exc.what();
        usage(av, ap);
        cout << endl << "!-------- Parsed CLI Args --------!\n";
    }

    cout << std::endl;

    return 0;
}

/* End of file */
