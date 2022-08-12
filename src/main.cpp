
#define VERSION_OS      "OS_WIN"
#define VERSION_MAJOR   14
#define VERSION_MINOR   0
#define VERSION_PATCH   0

#include "util.hpp"
#include "calls.hpp"

int main(int argc, char* argv_char[])
{
    #ifdef _WIN32
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif

    string argv[argc];
    for (int i = 0; i < argc; i++)
        argv[i] = argv_char[i];

    if (argc < 3 && false)
    {
        print_keywords();
        return EXIT_FAILURE;
    }

    string working_dir = home_dir + "\\Horst";
    chdir(working_dir.c_str());
    
    Horst_call call = get_call(argv[arg_method]);

    if (call == NULL)
        return EXIT_FAILURE;

    call(argv[arg_target]);

    return EXIT_SUCCESS;
}