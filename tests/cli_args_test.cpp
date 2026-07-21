#include <iostream>
#include "cli_args.hpp"

using std::cout;
using std::endl;


void usage(char *av[], CLI_Args& ap)
{
    cout << endl << "Usage: " << av[0];
    cout << " " << *ap.exp_args();
    cout << endl << *ap.help();
    cout << endl;
}


int main(int ac, char *av[])
{

    cout << ac << "Parsing CLI Args: ";
    for (int ii = 0; ii < ac; ii++)
    {
        cout << endl << av[ii];
    }

    CLI_Args ap(ac, av);
    auto in_all = ap.add_flag("-a");
    auto in_long = ap.add_flag("-l");
    auto in_format = ap.add_str("-F");
    auto in_inval = ap.add_str("--in-val");

    try
    {
        cout << endl << "<-------- Parsed CLI Args -------->\n";
        ap.parse_args();

        cout << endl << "in_all: " << *in_all;
        cout << endl << "in_long: " << *in_long;
        cout << endl << "in_format: " << *in_format;
        cout << endl << "in_val: " << *in_inval;
        cout << endl << "unnamed args: ";
        cout << endl << *ap.get_args();

        cout << endl << ">-------- Parsed CLI Args --------<\n";
    }
    catch (const std::exception& exc)
    {
        cout << exc.what();
        usage(av, ap);
        cout << endl << "!-------- Parsed CLI Args --------!\n";
    }

    cout << std::endl;

    return 0;
}

/* End of file */
