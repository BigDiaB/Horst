#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>
#include <unistd.h>

#define String std::string
#define Vector std::vector

void replace(String& input, String pattern, String replacement)
{
    std::regex pat(pattern);
    input = std::regex_replace(input,pat, replacement.c_str());
}

#define NUM_ATTRIBUTES 12

#define VERSION_MAJOR 11
#define VERSION_MINOR 3
#define VERSION_PATCH 5

#define version() std::cout << "Horst Version: " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;

String vars[] = {
    "COMPILER_NAME","LINKER_FLAGS","COMPILER_FLAGS","SOURCE","LIB_PATH", "INCLUDES", "LIBRARIES","EXECUTABLE_NAME", "DEBUGGER", "DEPENDENCIES", "DEPENDENCY_TYPE", "DEFINES"
};

String attr_template[] = {
    "gxx: ","gxxflags: ","cxxflags: ", "source:", "lib_path:", "includes:", "libraries: ","out: ", "debugger: ", "dependencies: ", "d_type: ", "defines: "
};

char exe_path[2056];

void file_to_string(String filename, String& doc)
{	
    std::ifstream file;
    file.open(String(exe_path) + "/" + String(filename) + "/build/" + String(filename) + ".horstproj");
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
    for (int i = 0; i < (int)lines.size(); i++)
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
    std::cout << "Keywords:"   << std::endl << "help, new, build, run, link, compile, do" << std::endl << std::endl;
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

void atrr_stuff(String filename, Vector<String>& attributes)
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
    for (int i = 0; i < (int)attributes.size(); i++)
    {
        temp.push_back(attributes[i]);
    }
    for (int i = 0; i < (int)temp.size(); i++)
    {
        replace(temp[i],attr_template[i],"");
    }
    
    bool first = true;
    
    for (int j = 0; j < (int)commands.size(); j++)
    {
        for (int i = 0; i < NUM_ATTRIBUTES; i++)
        {
            if (vars[i] == "EXECUTABLE_NAME")
                replace(temp[i]," ","");
            replace(commands[j], vars[i],temp[i]);
            if (first && argc > 2 && (String(argv[1]) == "build" || String(argv[1]) == "do"))
                std::cout << vars[i] << ": " << attributes[i] << std::endl;
        }
        first = false;
        
    }
}


void add_to_proj_list(char* name)
{
    std::ifstream file;
    file.open("Horst/build/proj_list.horstproj");

    String line,doc;
    Vector<String> lines;
    
    while (!file.eof() and file.is_open() and file)
    {
        std::getline(file, line);
        lines.push_back(line + "\n");
    }

    file.close();

    lines.push_back(String(name));


    std::ofstream file_o;
    file_o.open("Horst/build/proj_list.horstproj");

    for (int i = 0; i < (int)lines.size(); i++)
        file_o << lines[i];   

    file_o.close();

}

void remove_from_proj_list(char* name)
{
    std::ifstream file;
    file.open("Horst/build/proj_list.horstproj");

    String line,doc;
    Vector<String> lines;
    
    while (!file.eof() and file.is_open() and file)
    {
        std::getline(file, line);
        if (!(0 == strcmp(name,line.c_str())))
            lines.push_back(line + "\n");
    }

    file.close();

    replace(lines[lines.size() - 1],"\n","");

    std::ofstream file_o;
    file_o.open("Horst/build/proj_list.horstproj");

    for (int i = 0; i < (int)lines.size(); i++)
        file_o << lines[i];

    file_o.close();
}

bool check_in_proj_list(String name)
{
    return true;
    std::ifstream file;
    String line,doc;
    file.open("Horst/build/proj_list.horstproj");
    while (!file.eof() and file.is_open() and file)
    {
        std::getline(file, line);
        if (strcmp(name.c_str(),line.c_str()) == 0)
            return true;
    }
    return false;
}

bool use_lldb(Vector<String> attr)
{
    return (strstr(attr[8].c_str(),"lldb") != NULL);
}

void copy_dependencies(Vector<String> attributes, char* target)
{
    Vector<String> dependencies;
    Vector<String> types;

    std::stringstream ds(attributes[9]);
    std::stringstream dt(attributes[9]);
    
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

    for (int i = 1; i < (int)dependencies.size(); i++)
    {
        if (!check_in_proj_list(dependencies[i]))
        {
            std::cerr << "Konnte die Dependency in der Projekt-Liste nicht finden!: " << dependencies[i] << std::endl;
            // exit(EXIT_FAILURE);
            continue;
        }

        String T = "mkdir ";
        T += String(target);
        T += "/libs/include/";
        T += dependencies[i];
        system(T.c_str());
        T = "cp ";
        T += dependencies[i];
        T += "/src/*.h ";
        T += String(target);
        T += "/libs/include/";
        T += dependencies[i];
        system(T.c_str());

        if (types[i] == "dynamic")
        {
            String T = "cd && Horst dlib ";
            T += dependencies[i];
            T += " && ";
            T += "cp ";
            T += dependencies[i];
            T += "/build/lib";
            T += dependencies[i];
            T += ".dylib ";
            T += String(target);
            T += "/libs/lib/lib";
            T += dependencies[i];
            T += ".dylib";
            system(T.c_str());
        }
        else
        {
            String T = "cd && Horst slib ";
            T += dependencies[i];
            T += " && ";
            T += "cp ";
            T += dependencies[i];
            T += "/build/lib";
            T += dependencies[i];
            T += ".a ";
            T += String(target);
            T += "/libs/lib/lib";
            T += dependencies[i];
            T += ".a";
            system(T.c_str());
        }
    }

}

// #define DEBUG

int main(int argc, char* argv[])
{
	strcpy(exe_path,"/Users/benjaminemde/DEV");
	chdir(exe_path);
	
    version();
    if (argc == 1 || String(argv[1]) == "help")
    {
        print_keywords();
        exit(0);
    }
    else if (String(argv[1]) == "self")
    {
		String T = "cd " + String(exe_path) + "/Horst/build && clang++ ../src/main.cpp -o Horst && cd ../../";
        system(T.c_str());
        return 0;
    }
    
    commands.push_back("COMPILER_NAME COMPILER_FLAGS -c SOURCE INCLUDES DEFINES");
    commands.push_back("COMPILER_NAME LINKER_FLAGS -o EXECUTABLE_NAME *.o INCLUDES LIB_PATH LIBRARIES");
    commands.push_back("COMPILER_NAME -c COMPILER_FLAGS -o EXECUTABLE_NAME.o INCLUDES SOURCE && ar rc libEXECUTABLE_NAME.a EXECUTABLE_NAME.o"); //static library
    commands.push_back("COMPILER_NAME -dynamiclib -o libEXECUTABLE_NAME.dylib SOURCE INCLUDES LIB_PATH LIBRARIES"); //dynamic library
    commands.push_back("lldb -b -o run -f " + String(exe_path) + "/" + String(argv[2]) + "/build/" + "EXECUTABLE_NAME"); //execute with lldb
    commands.push_back("./EXECUTABLE_NAME");    //execute without lldb
    Vector<String> attributes;


    #ifndef DEBUG
    if (String(argv[1]) != "new" && String(argv[1]) != "delete")
    {
        if (!check_in_proj_list(String(argv[2])))
        {
            std::cerr << "Projekt \"" << argv[2] << "\"" << " ist nicht in der Projekt-Liste!" << std::endl;
            exit(EXIT_FAILURE);
        }
        atrr_stuff(argv[2], attributes);
        prepare_var(attributes,commands, argc, argv);
    }
    
    
    if (true)
    {
        if (String(argv[1]) == "new")
        {
            add_to_proj_list(argv[2]);

            String T;
            T = "cp -r ";
            T += exe_path;
            T += "/Horst/";
            T += "prep ";
            T += exe_path;
            T += "/";
            T += argv[2];
            system(T.c_str());

            T = "mv ";
            T += exe_path;
            T += "/";
            T += argv[2];
            T += "/build/";
            T += "prep.horstproj ";
            T += exe_path;
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
        else if (String(argv[1]) == "delete")
        {
            remove_from_proj_list(argv[2]);

            String T;
            T = "rm -rf ";
            T += argv[2];
            system(T.c_str());
        }
        else if (String(argv[1]) == "dlib")
        {
            copy_dependencies(attributes,argv[2]);
            std::cout << commands[3] << std::endl;
            String T = "cd ";
            T += exe_path;
            T += "/";
            T += argv[2];
            T += "/build";
            T += " &&" + commands[3];
            system(T.c_str());
        }
        else if (String(argv[1]) == "slib")
        {
            copy_dependencies(attributes,argv[2]);
            std::cout << commands[2] << std::endl;
            String T = "cd ";
            T += exe_path;
            T += "/";
            T += argv[2];
            T += "/build";
            T += " &&" + commands[2];
            system(T.c_str());
        }
        else if (String(argv[1]) == "build")
        {
            copy_dependencies(attributes,argv[2]);
            std::cout << commands[0] + "\n" + commands[1] << std::endl;
            String T = "cd ";
            T += exe_path;
            T += "/";
            T += argv[2];
            T += "/build";
            T += " &&" + commands[0] + " && " + commands[1];
            if (strstr(attributes[2].c_str(),"-g ") != NULL)
            {
                 T += "&& dsymutil ";
                 T += attributes[7];
            }
            T += " && rm -f ";
            T += exe_path;
            T += "/";
            T += argv[2];
            T += "/build/*.o";
            system(T.c_str());
        }
        else if (String(argv[1]) == "run")
        {
            String T = "cd ";
            T += exe_path;
            T += "/";
            T += argv[2];
            T += "/build";
            if (use_lldb(attributes))
                T += "&& " + commands[4];
            else
                T += "&& " + commands[5];

            system(T.c_str());
        }
        else if (String(argv[1]) == "do")
        {
            copy_dependencies(attributes,argv[2]);
            std::cout << commands[0] + "\n" + commands[1] << std::endl;
            String T = "cd ";
            T += exe_path;
            T += "/";
            T += argv[2];
            T += "/build";
            T += " &&" + commands[0] + " && " + commands[1];
            if (strstr(attributes[2].c_str(),"-g ") != NULL)
            {
                 T += "&& dsymutil ";
                 T += attributes[7];
            }
            T += " && rm -f ";
            T += exe_path;
            T += "/";
            T += argv[2];
            T += "/build/*.o";

            if (use_lldb(attributes))
                T += " && " + commands[4];
            else
                T += " && " + commands[5];

            system(T.c_str());
        }
        else
        {
            print_keywords();
            exit(EXIT_SUCCESS);
        }
    }
    #endif
    return EXIT_SUCCESS;
}
