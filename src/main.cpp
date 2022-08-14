
<<<<<<< Updated upstream
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
=======
#define VERSION_MAJOR 15
#define VERSION_MINOR 0
#define VERSION_PATCH 0
>>>>>>> Stashed changes

#include "util.hpp"
#include "calls.hpp"

<<<<<<< Updated upstream
int main(int argc, char* argv_char[])
{
    string argv[argc];

    for (int i = 0; i < argc; i++)
        argv[i] = argv_char[i];
=======
int main(int argc, char* argv[])
{
    #ifdef _WIN32
        strcpy_s(exe_path,"C:/Users/benjamin Emde/DEV");
    #else
        strcpy_s(exe_path,"/Users/benjaminemde/DEV");
    #endif
	chdir(exe_path);
	
    version();
>>>>>>> Stashed changes

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

<<<<<<< Updated upstream
=======
    DEBUG_MSG("calling call");
    requested_call(argv[2]);
    DEBUG_MSG("called call");

    #ifdef _WIN32
        String T = "mv \"";
        T += String(exe_path);
        T += "/Horst/build/Horst.exe\" \"";
        T += String(exe_path);
        T += "/Horst\" ";
        system(T.c_str());
    #endif

>>>>>>> Stashed changes
    return EXIT_SUCCESS;
}