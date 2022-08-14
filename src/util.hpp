#pragma once

#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
<<<<<<< Updated upstream
=======

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #define chdir _chdir
#else
    #include <unistd.h>
#endif

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
>>>>>>> Stashed changes

using namespace std;

#define version()   wcout << VERSION_OS << ":" << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << endl;

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #include <fcntl.h>
    #include <io.h>
    #define chdir _chdir
#else
    #include <unistd.h>
#endif

<<<<<<< Updated upstream
#ifdef _WIN32
    void copy_file(string src, string dest)
    {
        string T = "xcopy /s ";
        T += string(src);
        T += " ";
        T += string(dest);
        system(T.c_str());
    }
=======
void get_attributes(String target)
{
    #ifdef _WIN32
    commands.push_back("COMPILER_NAME COMPILER_FLAGS -c SOURCE INCLUDES DEFINES"); //compile standard executable
    commands.push_back("COMPILER_NAME LINKER_FLAGS -o EXECUTABLE_NAME.exe *.o INCLUDES LIB_PATH LIBRARIES"); //link standard executable
    commands.push_back("COMPILER_NAME -c COMPILER_FLAGS DEFINES -o EXECUTABLE_NAME.o INCLUDES SOURCE && ar rc libEXECUTABLE_NAME.lib *.o"); //static library
    commands.push_back("COMPILER_NAME -shared -o libEXECUTABLE_NAME.dll SOURCE INCLUDES LIB_PATH LIBRARIES"); //dynamic library
    commands.push_back("EXECUTABLE_NAME.exe");
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
>>>>>>> Stashed changes

    void move_file(string src, string dest)
    {
        string T = "move ";
        T += string(src);
        T += " ";
        T += string(dest);
        system(T.c_str());
    }

    /* name only needs to be the new filename ("file.txt") */
    void rename_file(string src, string name)
    {
        string T = "rename ";
        T += string(src);
        T += " ";
        T += string(name);
        system(T.c_str());
    }

    void create_directory(string path)
    {
        string T = "mkdir ";
        T += string(path);
        system(T.c_str());
    }

    void delete_directory(string path)
    {
        string T = "rmdir /s /q ";
        T += path;
        system(T.c_str());
    }
#else
    void copy_file(string src, string dest)
    {
        string T = "cp -r ";
        T += string(src);
        T += " ";
        T += string(dest);
        system(T.c_str());
    }

    void move_file(string src, string dest)
    {
        string T = "mv ";
        T += string(src);
        T += " ";
        T += string(dest);
        system(T.c_str());
    }

    /* name needs to be a full path on UNIX compiant OSes */
    void rename_file(string src, string name)
    {
        string T = "mv ";
        T += string(src);
        T += " ";
        T += string(name);
        system(T.c_str());
    }

    void create_directory(string path)
    {
        string T = "mkdir ";
        T += string(path);
        system(T.c_str());
    }

<<<<<<< Updated upstream
    void delete_directory(string path)
    {
        string T = "rm -rf ";
        T += path;
        system(T.c_str());
=======

        std::cout << types[i] << " library " << "\"" << dependencies[i] << "\"" << " wird kompiliert und nach \"" << target << "\" kopiert!" << std::endl;
		
        #ifdef _WIN32
        if (strstr(types[i].c_str(),"dynamic") != NULL)
        {
            String T = "cd && Horst dlib ";
            T += dependencies[i];
            T += " && ";
            T += "cp \"";
            T += exe_path;
            T += "/";
            T += dependencies[i];
            T += "/build/lib";
            T += dependencies[i];
            T += ".dll\" \"";
            T += String(exe_path);
            T += "../SHARED_LIBS/lib";
            T += dependencies[i];
            T += ".dll\"";
            system(T.c_str());

            puts(T.c_str());
        }
        else if (strstr(types[i].c_str(),"static") != NULL)
        {
            String T = "cd && Horst slib ";
            T += dependencies[i];
            T += " && ";
            T += "cp \"";
            T += exe_path;
            T += "/";
            T += dependencies[i];
            T += "/build/lib";
            T += dependencies[i];
            T += ".lib\" \"";
            T += String(exe_path);
            T += "/";
            T += String(target);
            T += "/libs/lib/\"";
            system(T.c_str());

            puts(T.c_str());
        }
        #else
        if (strstr(types[i].c_str(),"dynamic") != NULL)
        {
            String T = "cd && Horst dlib ";
            T += dependencies[i];
            T += " && ";
            T += "cp \"";
            T += exe_path;
            T += "/";
            T += dependencies[i];
            T += "/build/lib";
            T += dependencies[i];
            T += ".dylib\" ";
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
            T += "cp \"";
            T += exe_path;
            T += "/";
            T += dependencies[i];
            T += "/build/lib";
            T += dependencies[i];
            T += ".a\" \"";
            T += String(exe_path);
            T += "/";
            T += String(target);
            T += "/libs/lib/\"";
            system(T.c_str());

            puts(T.c_str());
        }
        #endif
        else
            printf("Weder \"static\" noch \"dynamic\" angegeben, ignoriere dependency: %s!\n",dependencies[i].c_str());
>>>>>>> Stashed changes
    }
#endif

void replace(string& input, string pattern, string replacement)
{
    regex pat(pattern);
    input = regex_replace(input,pat, replacement.c_str());
}

void print_keywords()
{
    #ifdef _WIN32
        system("CLS");
    #else
        system("clear");
    #endif
    
    version();
    wcout << "Keywords:"   << endl << "help, new, delete, build, run, do, dlib, slib"  << endl << endl;
    wcout << "Struktur:"   << endl << "Argumente und eine kleine Beschreibung zu den Argumenten" << endl << "Beschreibung und Hinweise zur Nutzung" << endl << endl;
    wcout << "help:"       << endl << "Keine Argumente, ruft diesen Bildschirm auf" << endl << endl;
    wcout << "new:"        << endl << "Name des zu erstellenden Projektes (Vergiss nicht vorher zum richtigen Ort zu CD'en)" << endl << "Erstellt ein neues Projekt mit Hello, World!- Beispiel" << endl << endl;
    wcout << "delete:"     << endl << L"Name des zu löschenden Projektes (Vergiss nicht vorher zum richtigen Ort zu CD'en)" << endl << L"Löscht ein vorhandenes Projekt" << endl << endl;
    wcout << "build:"      << endl << "Name des Projektes" << endl << "Kompiliert das Projekt" << endl << endl;
    wcout << "run:"        << endl << "Name des Projektes" << endl << L"Führt das Projekt aus. Funktioniert nicht, wenn es vorher nicht mit \"Horst build proj\" kompiliert wurde " << endl << endl;
    wcout << "do:"         << endl << "Name des Projektes" << endl << L"Kompiliert das Projekt genau wie mit \"Horst build proj\" und führt es danach genau wie \"Horst run proj\" aus" << endl << endl;
    wcout << "dlib:"       << endl << "Name des Projektes" << endl << "Kompiliert das Projekt in eine dynamische Library (Vergiss nicht vorher \"int main()\" aus deinem Code zu entfernen, sonst gibt es nachher \"duplicate symbols\"-Error!)" << endl << endl;
    wcout << "slib:"       << endl << "Name des Projektes" << endl << "Kompiliert das Projekt in eine statische Library (Vergiss nicht vorher \"int main()\" aus deinem Code zu entfernen, sonst gibt es nachher \"duplicate symbols\"-Error!)" << endl << endl;
}