 
#include <mach-o/dyld.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#define String std::string
#define NAME 5
#define NUM_ATTRIBUTES 4

String attr_template[NUM_ATTRIBUTES] = {
    "gxx: ","gxxflags: ","cxxflags: ","out: "
};
String attr_insert[NUM_ATTRIBUTES] = {
    "gxx=","gxxflags=","cxxflags?=","out="
};
void file_to_string(String filename, String& doc)
{
    char working_dir[1024];
    getcwd(working_dir,1024);
    String dir = String(working_dir);
    
    std::ifstream file;
    file.open(dir + "/" + String(filename) + "/" + String(filename) + ".horstproj");
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

void string_to_vector(String doc, std::vector<String>& vec)
{
 
    std::stringstream ss(doc);
 
    while (ss.good()) {
        String substr;
        std::getline(ss, substr, '\n');
        vec.push_back(substr);
    }
}

void lines_to_attributes(std::vector<String> lines,std::vector<String>& attributes)
{
    for (int i = 0; i < lines.size(); i++)
    {
        String t = attr_insert[i] + (lines[i].substr(lines[i].find(": ") + 1));
        t.erase(remove(t.begin(),t.end(),' '),t.end());
        attributes.push_back(t); 
    }
}

void sort_attributes_in_lines(std::vector<String>&lines)
{
    String new_lines[NUM_ATTRIBUTES];
    for (int i = 0; i < NUM_ATTRIBUTES; i++)
    {
        if (lines[i].find(attr_template[0]) != String::npos)
            new_lines[0] = attr_insert[0] + lines[i];
        else if (lines[i].find(attr_template[1]) != String::npos)
            new_lines[1] = attr_insert[1] + lines[i];
        else if (lines[i].find(attr_template[2]) != String::npos)
            new_lines[2] = attr_insert[2] + lines[i];
        else if (lines[i].find(attr_template[3]) != String::npos)
            new_lines[3] = attr_insert[3] + lines[i];
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
    std::cout << "Keywords:" << std::endl << "help, new, build, run, clean" << std::endl << std::endl;
    std::cout << "Struktur:" << std::endl << "Argumente und eine kleine Beschreibung zu den Argumenten" << std::endl << "Beschreibung und Hinweise zur Nutzung" << std::endl << std::endl;
    std::cout << "help:" << std::endl << "Keine Argumente, ruft diesen Bildschirm auf" << std::endl << std::endl;
    std::cout << "new:" << std::endl << "Name des zu erstellenden Projektes (Vergiss nicht vorher zum richtigen Ort zu CD'en)" << std::endl << "Erstellt ein neues Projekt mit Hello, World!- Beispiel" << std::endl << std::endl;
    std::cout << "build:" << std::endl << "Name des Projekts (, OPTIONAL: Feld der Makefile (Run, all, clean))" << std::endl << "Compiliert, wenn es kein 2. Argument gab, ansonsten wird das 2. Argument ausgeführt" << std::endl << std::endl;
    std::cout << "run:" << std::endl << "Name des Projektes" << std::endl << "führt das Projekt aus oder called vorher build, falls die SRC-Dateien verändert wurden" << std::endl << std::endl;
    std::cout << "clean:" << std::endl << "Name des Projektes" << std::endl << "Löscht alle Objekt-Dateien und die Executable" << std::endl << std::endl;
}

void atrr_stuff(String filename, std::vector<String>& attributes,char* argv[])
    {
        std::vector<String> proj_data;
        String doc;
        file_to_string(argv[2], doc);
        string_to_vector(doc,proj_data); 
        sort_attributes_in_lines(proj_data);
        lines_to_attributes(proj_data, attributes);
    }


int main(int argc, char* argv[])
{
    char exe_path[1024];
    uint32_t size = sizeof(exe_path);
    _NSGetExecutablePath(exe_path, &size);
    
    std::vector<String> attributes;
    String attributes_make = " ";
    if (String(argv[1]) == "build" || String(argv[1]) == "run" || String(argv[1]) == "clean")
        {
            atrr_stuff("GAME", attributes, argv);
            for (int i = 0; i < NUM_ATTRIBUTES; i++)
            {
                attributes_make += attributes[i] + " ";
            }
            attributes_make += " ";
        }

    

    
    
    char working_dir[1024];
    getcwd(working_dir,1024);
    
    if (argc <= 1 || String(argv[1]) == "help")
        print_keywords();
    else
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
            T += "/";
            T += "prep.horstproj ";
            T += working_dir;
            T += "/";
            T += argv[2];
            T += "/";
            T += argv[2];
            T += ".horstproj";
            system(T.c_str());
        }
        else if (String(argv[1]) == "build")
        {
            String T = "cd ";
            T += working_dir;
            T += "/";
            T += argv[2];
            T += "/build";
            T += " && make";
            T += attributes_make;
            if (argc == 4)
                T += argv[3];
            system(T.c_str());
        }
        else if (String(argv[1]) == "run")
        {
            String T = "cd ";
            T += working_dir;
            T += "/";
            T += argv[2];
            T += "/build";
            T += " && make";
            T += attributes_make;
            T += "run";
            
            system(T.c_str());
        }
        else if (String(argv[1]) == "clean")
        {
            String T = "cd ";
            T += working_dir;
            T += "/";
            T += argv[2];
            T += "/build";
            T += " && make";
            T += attributes_make;
            T += "clean";
            
            system(T.c_str());
        }
    return 0;
}
