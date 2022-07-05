
#define VERSION_MAJOR 14
#define VERSION_MINOR 0
#define VERSION_PATCH 0

#include "util.hpp"
#include "calls.hpp"

int main(int argc, char* argv[])
{
	strcpy(exe_path,"/Users/benjaminemde/DEV");
	chdir(exe_path);
	
    version();

    if (argc == 1 || String(argv[1]) == "help")
    {
        print_keywords();
        exit(EXIT_SUCCESS);
    }

    Horst_call requested_call = get_call(argv[1]);

    if (requested_call == NULL)
    {
        if (String(argv[1]) == "self")
        {
            String T = "cd " + String(exe_path) + "/Horst/build && clang++ ../src/main.cpp -o Horst && cd ../../";
            system(T.c_str());
            exit(EXIT_SUCCESS);
        }
        print_keywords(); 
        std::cout << "\"" << argv[1] << "\" ist kein gültiger Befehl!" << std::endl;
        exit(EXIT_SUCCESS);
    }
    else if (String(argv[1]) != "new")
    {
        if (argc == 2)
        {
            print_keywords();
            std::cout << "\"" << argv[1] << "\" benötigt weitere Argumente!" << std::endl;
            exit(EXIT_SUCCESS);
        }

        if (!check_in_proj_list(String(argv[2])))
        {
            std::cerr << "Projekt \"" << argv[2] << "\"" << " ist nicht in der Projekt-Liste!" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (String(argv[1]) != "delete")
        {
            atrr_stuff(argv[2], attributes);
            prepare_var(attributes,commands, argc, argv);
        }
    }

    requested_call(argv[2]);
    
    return EXIT_SUCCESS;
}
