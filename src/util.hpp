#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>

#include <sys/stat.h>
#include <unistd.h>

#define String std::string
#define Vector std::vector
#define PROJ_LIST_DIR "Horst/build/proj_list.horstproj"
#define NUM_ATTRIBUTES 12
#define version() std::cout << "Horst Version: " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;

Vector<String> attributes;
Vector<String> commands;
char exe_path[2056];

enum command_list
{
    cmd_compile,
    cmd_link,
    cmd_static,
    cmd_dynamic,
    cmd_execute
};

enum proj_method
{
    proj_add,
    proj_remove,
    proj_check
};

void DEBUG_MSG(String msg)
{
    #ifdef EBUG
    puts(msg.c_str());
    #else
    msg = "";
    #endif
}

void replace(String& input, String pattern, String replacement)
{
    std::regex pat(pattern);
    input = std::regex_replace(input,pat, replacement.c_str());
}

void get_attributes(String target)
{
    #ifdef _WIN32
    commands.push_back("COMPILER_NAME COMPILER_FLAGS -c SOURCE INCLUDES DEFINES"); //compile standard executable
    commands.push_back("COMPILER_NAME LINKER_FLAGS -o EXECUTABLE_NAME.exe *.o INCLUDES LIB_PATH LIBRARIES"); //link standard executable
    commands.push_back("COMPILER_NAME -c COMPILER_FLAGS DEFINES -o EXECUTABLE_NAME.o INCLUDES SOURCE && ar rc libEXECUTABLE_NAME.lib *.o"); //static library
    commands.push_back("COMPILER_NAME -dynamiclib -o libEXECUTABLE_NAME.dll SOURCE INCLUDES LIB_PATH LIBRARIES"); //dynamic library
    commands.push_back("EXECUTABLE_NAME.exe");
    #elif defined __linux__
    commands.push_back("COMPILER_NAME COMPILER_FLAGS -c SOURCE INCLUDES DEFINES"); //compile standard executable
    commands.push_back("COMPILER_NAME LINKER_FLAGS -o EXECUTABLE_NAME *.o INCLUDES LIB_PATH LIBRARIES"); //link standard executable
    commands.push_back("COMPILER_NAME -c COMPILER_FLAGS DEFINES -o EXECUTABLE_NAME.o INCLUDES SOURCE && ar rc libEXECUTABLE_NAME.a *.o"); //static library
    commands.push_back("COMPILER_NAME -shared -o libEXECUTABLE_NAME.so SOURCE INCLUDES LIB_PATH LIBRARIES"); //dynamic library
    commands.push_back("./EXECUTABLE_NAME");
    #elif defined __APPLE__
    commands.push_back("COMPILER_NAME COMPILER_FLAGS -c SOURCE INCLUDES DEFINES"); //compile standard executable
    commands.push_back("COMPILER_NAME LINKER_FLAGS -o EXECUTABLE_NAME *.o INCLUDES LIB_PATH LIBRARIES"); //link standard executable
    commands.push_back("COMPILER_NAME -c COMPILER_FLAGS DEFINES -o EXECUTABLE_NAME.o INCLUDES SOURCE && ar rc libEXECUTABLE_NAME.a *.o"); //static library
    commands.push_back("COMPILER_NAME -dynamiclib -o libEXECUTABLE_NAME.dylib SOURCE INCLUDES LIB_PATH LIBRARIES"); //dynamic library
    commands.push_back("./EXECUTABLE_NAME");
    #endif

    String attr_template[NUM_ATTRIBUTES] = {
    "gxx: ","gxxflags: ","cxxflags: ", "source:", "lib_path:", "includes:", "libraries: ","out: ", "debugger: ", "dependencies: ", "d_types: ", "defines: "
    };

    String vars[NUM_ATTRIBUTES] = {
    "COMPILER_NAME","LINKER_FLAGS","COMPILER_FLAGS","SOURCE","LIB_PATH", "INCLUDES", "LIBRARIES","EXECUTABLE_NAME", "DEBUGGER", "DEPENDENCIES", "DEPENDENCY_TYPE", "DEFINES"
    };

    std::ifstream file;
    file.open(String(exe_path) + "/" + target + "/build/" + target + ".horstproj");
    while (!file.eof() and file.is_open() and file)
    {
        String line;
        std::getline(file, line);

        for (int i = 0; i < NUM_ATTRIBUTES; i++)
            if (line.find(attr_template[i]) != String::npos)
                attributes.push_back(line);
    }

    String temp[NUM_ATTRIBUTES];
    for (int j = 0; j < (int)NUM_ATTRIBUTES; j++)
    {
        for (int i = 0; i < (int)attributes.size(); i++)
        {
            if (attributes[i].find(attr_template[j]) != String::npos)
            {
                replace(attributes[i],attr_template[j],"");
                temp[j] = attributes[i];
            }
        }
    }
    attributes.clear();
    for (int i = 0; i < NUM_ATTRIBUTES; i++)
        attributes.push_back(temp[i]);
    for (int i = 0; i < (int)commands.size(); i++)
    {
        for (int j = 0; j < NUM_ATTRIBUTES; j++)
        {
            if (commands[i].find(vars[j]) != String::npos)
            {
                replace(commands[i],vars[j],attributes[j]);
            }
        }
    }
}

bool proj_list(String target, enum proj_method method)
{
    switch(method)
    {
        case proj_add:
        {
            std::ifstream file;
            file.open(PROJ_LIST_DIR);
            Vector<String> lines;
            while (!file.eof() and file.is_open() and file)
            {
                String line;
                std::getline(file, line);
                lines.push_back(line + "\n");
            }
            file.close();
            lines.push_back(String(target));

            std::ofstream file_o;
            file_o.open(PROJ_LIST_DIR);

            for (int i = 0; i < (int)lines.size(); i++)
                file_o << lines[i];   

            file_o.close();
        }
        break;

        case proj_remove:
        {
                std::ifstream file;
                file.open(PROJ_LIST_DIR);

                Vector<String> lines;
                
                while (!file.eof() and file.is_open() and file)
                {
                    String line;
                    std::getline(file, line);
                    if (target != line)
                        lines.push_back(line + "\n");
                }

                file.close();

                replace(lines[lines.size() - 1],"\n","");

                std::ofstream file_o;
                file_o.open(PROJ_LIST_DIR);

                for (int i = 0; i < (int)lines.size(); i++)
                    file_o << lines[i];

                file_o.close();
        }
        break;

        case proj_check:
        {
            if (target.empty())
                return false;
            std::ifstream file;
            String line;
            file.open(PROJ_LIST_DIR);
            while (!file.eof() and file.is_open() and file)
            {
                std::getline(file, line);
                if (target == line)
                    return true;
            }
            return false;
        }
        break;
    }

    return true;
}

void copy_dependencies(Vector<String> attributes, char* target)
{
    Vector<String> dependencies;
    Vector<String> types;

    std::stringstream ds(attributes[9]);
    std::stringstream dt(attributes[10]);
    
    while (ds.good()) {
        String substr;
        std::getline(ds, substr, ' ');
        dependencies.push_back(substr);
    }

    while (dt.good()) {
        String substr;
        std::getline(dt, substr, ' ');
        types.push_back(substr);
    }
	
	bool first = true;
    for (int i = 1; i < (int)dependencies.size(); i++)
    {
		if (dependencies[i].empty())
			continue;

        if (first)
        {
            std::cout << "Dependencies für \"" << target << "\" werden kopiert!" << std::endl;
            first = false;
        }
		
        if (!proj_list(dependencies[i],proj_check))
        {
            std::cerr << "Konnte die Dependency in der Projekt-Liste nicht finden!: " << "\"" << dependencies[i] << "\"" << std::endl;
            continue;
        }

        String T;
        T = String(target);
        T += "/libs/include/";
        T += dependencies[i];

        DEBUG_MSG("opening directory: " + T);

        
        struct stat info;

        if(stat( T.c_str(), &info ) != 0)
        {
            DEBUG_MSG(T + " doesn't exist");
            T = "mkdir ";
            T += String(target);
            T += "/libs/include/";
            T += dependencies[i];
            system(T.c_str());
        }
        else if(info.st_mode & S_IFDIR)
            DEBUG_MSG(T + " exists");
        else
        {
            DEBUG_MSG(T + " doesn't exist");
            T = "mkdir ";
            T += String(target);
            T += "/libs/include/";
            T += dependencies[i];
            system(T.c_str());
        }
        
        T = "cp ";
        T += dependencies[i];
        T += "/src/*.h ";
        T += String(target);
        T += "/libs/include/";
        T += dependencies[i];
        system(T.c_str());


        std::cout << types[i] << " library " << "\"" << dependencies[i] << "\"" << " wird kompiliert und nach \"" << target << "\" kopiert!" << std::endl;
				
        if (strstr(types[i].c_str(),"dynamic") != NULL)
        {
            String T = "cd && Horst dlib ";
            T += dependencies[i];
            T += " && ";
            T += "cp ";
			T += exe_path;
			T += "/";
            T += dependencies[i];
            T += "/build/lib";
            T += dependencies[i];
            T += ".dylib ";
			T += "/usr/local/lib/lib";
            T += dependencies[i];
            T += ".dylib";
            system(T.c_str());

            puts(T.c_str());
        }
        else if (strstr(types[i].c_str(),"static") != NULL)
        {
            String T = "cd && Horst slib ";
            T += dependencies[i];
            T += " && ";
            T += "cp ";
			T += exe_path;
			T += "/";
            T += dependencies[i];
            T += "/build/lib";
            T += dependencies[i];
            T += ".a ";
			T += String(exe_path);
			T += "/";
            T += String(target);
            T += "/libs/lib/";
            system(T.c_str());

            puts(T.c_str());
        }
        else
            printf("Weder \"static\" noch \"dynamic\" angegeben, ignoriere dependency: %s!\n",dependencies[i].c_str());
    }

    std::cout << "Dependencies für \"" << target << "\" wurden kopiert!" << std::endl;
}

void print_keywords()
{
    system("clear");
    version();
    std::cout << "Keywords:"   << std::endl << "help, new, delete, build, run, do, dlib, slib"  << std::endl << std::endl;
    std::cout << "Struktur:"   << std::endl << "Argumente und eine kleine Beschreibung zu den Argumenten" << std::endl << "Beschreibung und Hinweise zur Nutzung" << std::endl << std::endl;
    std::cout << "help:"       << std::endl << "Keine Argumente, ruft diesen Bildschirm auf" << std::endl << std::endl;
    std::cout << "new:"        << std::endl << "Name des zu erstellenden Projektes (Vergiss nicht vorher zum richtigen Ort zu CD'en)" << std::endl << "Erstellt ein neues Projekt mit Hello, World!- Beispiel" << std::endl << std::endl;
    std::cout << "delete:"     << std::endl << "Name des zu löschenden Projektes (Vergiss nicht vorher zum richtigen Ort zu CD'en)" << std::endl << "Löscht ein vorhandenes Projekt" << std::endl << std::endl;
    std::cout << "build:"      << std::endl << "Name des Projektes" << std::endl << "Kompiliert das Projekt" << std::endl << std::endl;
    std::cout << "run:"        << std::endl << "Name des Projektes" << std::endl << "Führt das Projekt aus. Funktioniert nicht, wenn es vorher nicht mit \"Horst build proj\" kompiliert wurde " << std::endl << std::endl;
    std::cout << "do:"         << std::endl << "Name des Projektes" << std::endl << "Kompiliert das Projekt genau wie mit \"Horst build proj\" und führt es danach genau wie \"Horst run proj\" aus" << std::endl << std::endl;
    std::cout << "dlib:"       << std::endl << "Name des Projektes" << std::endl << "Kompiliert das Projekt in eine dynamische Library (Vergiss nicht vorher \"int main()\" aus deinem Code zu entfernen, sonst gibt es nachher \"duplicate symbols\"-Error!)" << std::endl << std::endl;
    std::cout << "slib:"       << std::endl << "Name des Projektes" << std::endl << "Kompiliert das Projekt in eine statische Library (Vergiss nicht vorher \"int main()\" aus deinem Code zu entfernen, sonst gibt es nachher \"duplicate symbols\"-Error!)" << std::endl << std::endl;
}