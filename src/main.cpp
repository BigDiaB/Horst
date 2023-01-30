#ifdef _WIN32
	#define VERSION_OS "WIN"
#else
	#define VERSION_OS "UNIX"
#endif

#define VERSION_MAJOR 16
#define VERSION_MINOR 1
#define VERSION_PATCH 1

#include "util.hpp"
#include "calls.hpp"

int main(int argc, char* argv[])
{
    /*
    TODO:
        exe_path von einer Datei einlesen oder so kp, lmfao    
    */
    #ifdef _WIN32
        strcpy_s(exe_path,"C:/Users/benjamin Emde/DEV");
    #else
        strcpy(exe_path,"/home/benjaminemde/DEV");
    #endif
	chdir(exe_path);
	
    version();

    if (argc == 1 || (argc >= 2 && String(argv[1]) == "help"))
    {
        exit(EXIT_SUCCESS);
    }

    DEBUG_MSG("call requested");
    Horst_call requested_call = get_call(argv[1]);
    DEBUG_MSG("call received");

    if (requested_call == NULL)
    {
        DEBUG_MSG("call is NULL");
        if (String(argv[1]) == "self")
        {
            String T = "cd " + String(exe_path) + "/Horst/build && clang++ ../src/main.cpp -o Horst && cd ../../";
            system(T.c_str());
            exit(EXIT_SUCCESS);
        }
        std::cout << "\"" << argv[1] << "\" ist kein gültiger Befehl!" << std::endl;
        exit(EXIT_SUCCESS);
    }
    else if (String(argv[1]) != "new")
    {
        DEBUG_MSG("call is not NULL or \"new\"");
        if (argc == 2)
        {
            std::cout << "\"" << argv[1] << "\" benötigt weitere Argumente!" << std::endl;
            exit(EXIT_SUCCESS);
        }

        DEBUG_MSG("checking proj-list");
        if (!proj_list(String(argv[2]),proj_check))
        {
            std::cerr << "Projekt \"" << argv[2] << "\"" << " ist nicht in der Projekt-Liste!" << std::endl;
            exit(EXIT_FAILURE);
        }
        DEBUG_MSG("found in proj-list");

        if (String(argv[1]) != "delete")
        {
            DEBUG_MSG("call is not \"delete\"");
            get_attributes(String(argv[2]));
        }
    }
    DEBUG_MSG("setting target");
    target = String(argv[2]);
    DEBUG_MSG("set target");

    DEBUG_MSG("calling call");
    requested_call();
    DEBUG_MSG("called call");

    #ifdef _WIN32
    if (String(argv[2]) == "Horst")
    {
        String T = "mv \"";
        T += String(exe_path);
        T += "/Horst/build/Horst.exe\" \"";
        T += String(exe_path);
        T += "/Horst\" ";
        system(T.c_str());
    }
    #endif

    return EXIT_SUCCESS;
}
