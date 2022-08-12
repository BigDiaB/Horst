
#ifdef _WIN32
#define VERSION_OS      "OS_WIN"
#elif defined __LINUX__
#define VERSION_OS      "OS_LIN"
#elif defined __APPLE__
#define VERSION_OS      "OS_MAC"
#endif

#define VERSION_MAJOR   14
#define VERSION_MINOR   0
#define VERSION_PATCH   0

#include "util.hpp"
#include "calls.hpp"

int main(int argc, char* argv_char[])
{
    string argv[argc];

    for (int i = 0; i < argc; i++)
        argv[i] = argv_char[i];

    if (argc < 3 && false)
    {
        print_keywords();
        return EXIT_FAILURE;
    }

    chdir(home_dir.c_str());
    
    Horst_call call = get_call(argv[arg_method]);
    if (call == NULL)
        return EXIT_FAILURE;

    if (argv[arg_method] != "new")
    {
        if (!proj_list(argv[arg_target],proj_check))
            return EXIT_FAILURE;

        if (argv[arg_method] != "delete")
            get_attributes(argv[arg_target]);
    }

    call(argv[arg_target]);

    #ifdef _WIN32
    if (argv[arg_target] == "Horst")
        move_file(home_dir + "\\Horst\\build\\Horst.exe", home_dir + "\\Horst\\Horst.exe");
    #endif

    return EXIT_SUCCESS;
}