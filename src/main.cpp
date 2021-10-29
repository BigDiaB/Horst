#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <mach-o/dyld.h>
#include <unistd.h>

#define String std::string
#define Vector std::vector

#define exe_path() char exe_path[1024];uint32_t size = sizeof(exe_path);_NSGetExecutablePath(exe_path, &size)
#define working_dir() char working_dir[1024];getcwd(working_dir,1024)

void replace(String& input, String pattern, String replacement)
{
    std::regex pat(pattern);
    input = std::regex_replace(input,pat, replacement.c_str());
}

#define NAME 5
#define NUM_ATTRIBUTES 8

#define VERSION_MAJOR 7
#define VERSION_MINOR 2
#define VERSION_PATCH 1

#define version() std::cout << "Horst Version: " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;

String vars[NUM_ATTRIBUTES] = {
    "COMPILER_NAME","LINKER_FLAGS","COMPILER_FLAGS","SOURCE","LIB_PATH", "INCLUDES", "LIBRARIES","EXECUTABLE_NAME"
};
String vals[NUM_ATTRIBUTES];
String attr_template[] = {
    "gxx: ","gxxflags: ","cxxflags: ", "source:", "lib_path:", "includes:", "libraries: ","out: "
};

void file_to_string(String filename, String& doc)
{
    char working_dir[1024];
    getcwd(working_dir,1024);
    String dir = String(working_dir);
    
    std::ifstream file;
    file.open(dir + "/" + String(filename) + "/build/" + String(filename) + ".horstproj");
    if (!file.is_open() and !file)
    {
        std::cerr << "Konnte die Projekt-Datei nicht finden!" << std::endl;
        exit(EXIT_FAILURE);
    }
    String line;
    
    while (!file.eof() and file.is_open() and file)
    {
        std::getline(file, line);
        doc += line + "\n";
    }
    
}

void string_to_vector(String doc, Vector<String>& vec)
{
    
    std::stringstream ss(doc);
    
    while (ss.good()) {
        bool relevant = false;
        String substr;
        std::getline(ss, substr, '\n');
        for (int i = 0; i < NUM_ATTRIBUTES; i++)
        {
            if (substr.find(attr_template[i]) != String::npos)
                relevant = true;
        }
        if (relevant)
            vec.push_back(substr);
    }
}

void lines_to_attributes(Vector<String> lines,Vector<String>& attributes)
{
    for (int i = 0; i < lines.size(); i++)
    {
        String t = (lines[i].substr(lines[i].find(": ") + 1));
        attributes.push_back(t);
    }
}

void sort_attributes_in_lines(Vector<String>& lines)
{
    String new_lines[NUM_ATTRIBUTES];
    for (int i = 0; i < NUM_ATTRIBUTES; i++)
    {
        for (int j = 0; j < NUM_ATTRIBUTES; j++)
        {
            if (lines[i].find(attr_template[j]) != String::npos)
            {
                new_lines[j] = lines[i];
            }
        }
    }
    lines.clear();
    for (int i = 0; i < NUM_ATTRIBUTES; i++)
    {
        lines.push_back(new_lines[i]);
    }
}



void print_keywords()
{
    system("clear");
    version();
    std::cout << "Keywords:"    << std::endl << "help, new, build, run" << std::endl << std::endl;
    std::cout << "Struktur:"    << std::endl << "Argumente und eine kleine Beschreibung zu den Argumenten" << std::endl << "Beschreibung und Hinweise zur Nutzung" << std::endl << std::endl;
    std::cout << "help:"    	<< std::endl << "Keine Argumente, ruft diesen Bildschirm auf" << std::endl << std::endl;
    std::cout << "new:"     	<< std::endl << "Name des zu erstellenden Projektes (Vergiss nicht vorher zum richtigen Ort zu CD'en)" << std::endl << "Erstellt ein neues Projekt mit Hello, World!- Beispiel" << std::endl << std::endl;
    std::cout << "build:"       << std::endl << "Name des Projektes" << std::endl << "Kompiliert das Projekt" << std::endl << std::endl;
    std::cout << "run:"         << std::endl << "Name des Projektes" << std::endl << "Führt das Projekt aus. Funktioniert nicht, wenn es vorher nicht mit \"Horst build proj\" kompiliert wurde " << std::endl << std::endl;
}

void atrr_stuff(String filename, Vector<String>& attributes,char* argv[])
{
    Vector<String> proj_data;
    String doc;
    file_to_string(filename, doc);
    string_to_vector(doc,proj_data);
    sort_attributes_in_lines(proj_data);
    lines_to_attributes(proj_data, attributes);
}

Vector<String> commands;
void prepare_var(Vector<String> attributes, Vector<String>& commands, int argc, char* argv[])
{
    Vector<String> temp;
    for (int i = 0; i < attributes.size(); i++)
    {
        temp.push_back(attributes[i]);
    }
    for (int i = 0; i < temp.size(); i++)
    {
        replace(temp[i],attr_template[i],"");
    }
    
    bool first = true;
    
    for (int j = 0; j < commands.size(); j++)
    {
        for (int i = 0; i < NUM_ATTRIBUTES; i++)
        {
            if (vars[i] == "EXECUTABLE_NAME")
                replace(temp[i]," ","");
            replace(commands[j], vars[i],temp[i]);
            if (first && argc > 2 && String(argv[1]) == "build")
                std::cout << vars[i] << ": " << attributes[i] << std::endl;
        }
        first = false;
        
    }
}
int main(int argc, char* argv[])
{
    version();
    if (argc == 1)
    {
        print_keywords();
        exit(0);
    }
    
    if (String(argv[1]) == "self")
    {
        system("cd Horst/build && clang++ ../src/main.cpp -o Horst && cd ../../");
        return 0;
    }
    
    exe_path();
    working_dir();
    commands.push_back("COMPILER_NAME COMPILER_FLAGS -c SOURCE INCLUDES && COMPILER_NAME LINKER_FLAGS -o EXECUTABLE_NAME *.o INCLUDES LIB_PATH LIBRARIES");
    commands.push_back("lldb -b -o run -f " + String(working_dir) + "/" + String(argv[2]) + "/build/" + "EXECUTABLE_NAME");
    Vector<String> attributes;
    String attributes_make = " ";
    if (String(argv[1]) == "build" || String(argv[1]) == "run" || String(argv[1]) == "clean")
    {
        
        atrr_stuff(argv[2], attributes, argv);
        bool has_libs = true;
        for (int i = 0; i < attributes.size(); i++)
            if (attributes[i].find("lib_path:") != String::npos)
            {
                String temp = attributes[i];
                replace(temp,"lib_path:","");
                if (temp.length() > 3)
                    has_libs = true;
            }
        
        if (!has_libs)
            commands[0] = "COMPILER_NAME -g -Wall INCLUDES SOURCE -o EXECUTABLE_NAME";
        
        prepare_var(attributes,commands, argc, argv);
    }
    
    
    if (true)
    {
        if (String(argv[1]) == "new")
        {
            String T;
            T = "cp -r ";
            T += exe_path;
            for (int i = 0; i < NAME + 6; i++)
                T.pop_back();
            T += "prep ";
            T += working_dir;
            T += "/";
            T += argv[2];
            system(T.c_str());
            T = "mv ";
            T += working_dir;
            T += "/";
            T += argv[2];
            T += "/build/";
            T += "prep.horstproj ";
            T += working_dir;
            T += "/";
            T += argv[2];
            T += "/build/";
            T += argv[2];
            T += ".horstproj";
            system(T.c_str());
            T = "Horst build ";
            T += argv[2];
            system(T.c_str());
        }
        else if (String(argv[1]) == "do")
        {
            system("clear");
            String T = "Horst build ";
            T += String(argv[2]);
            system(T.c_str());
            T = "Horst run ";
            T += String(argv[2]);
            system(T.c_str());
        }
        else if (String(argv[1]) == "build")
        {
            std::cout << commands[0] << std::endl;
            String T = "cd ";
            T += working_dir;
            T += "/";
            T += argv[2];
            T += "/build";
            T += " &&" + commands[0];
            system(T.c_str());
        }
        else if (String(argv[1]) == "run")
        {
            String T = "cd ";
            T += working_dir;
            T += "/";
            T += argv[2];
            T += "/build";
            T += "&& " + commands[1];
            system(T.c_str());
        }
        else if (String(argv[1]) == "environment")
        {
            bool done = false;
            String input;
            String command;
            while(!done)
            {
                system("clear");
                version();
                std::cout << "Keywords:"    << std::endl << "new, build, run, exit" << std::endl << std::endl;
                std::cout << "Struktur:"    << std::endl << "Argumente und eine kleine Beschreibung zu den Argumenten" << std::endl << "Beschreibung und Hinweise zur Nutzung" << std::endl << std::endl;
                std::cout << "new:"         << std::endl << "Name des zu erstellenden Projektes (Vergiss nicht vorher zum richtigen Ort zu CD'en)" << std::endl << "Erstellt ein neues Projekt mit Hello, World!- Beispiel" << std::endl << std::endl;
                std::cout << "build:"       << std::endl << "Name des Projektes" << std::endl << "Kompiliert das Projekt" << std::endl << std::endl;
                std::cout << "run:"         << std::endl << "Name des Projektes" << std::endl << "Führt das Projekt aus. Funktioniert nicht, wenn es vorher nicht mit \"Horst build proj\" kompiliert wurde " << std::endl << std::endl;
                std::cout << "exit:"        << std::endl << "Braucht keine Argumente" << std::endl << "Beendet das Horst-Build-Environment" << std::endl << std::endl;
                std::cout << "> ";
                std::getline(std::cin,input);
                if (input == "exit")
                {
                    system("clear");
                    return 0;
                }
                std::cout << input << std::endl;
                command = "Horst " + input;
                system(command.c_str());
                std::getline(std::cin,input);
                
            }
        }
        else
        {
            print_keywords();
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}