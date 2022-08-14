#pragma once

#ifdef _WIN32
string home_dir = "C:\\Users\\Benjamin Emde\\DEV";
#else
string home_dir = "Users/Benjaminemde/DEV";
#endif

vector<string> attributes;
vector<string> commands;

enum arg_indices
{
    arg_call,
    arg_method,
    arg_target
};

typedef void (*Horst_call)(string);

enum proj_method
{
    proj_add,
    proj_remove,
    proj_check
};

enum command_list
{
    cmd_compile,
    cmd_link,
    cmd_static,
    cmd_dynamic,
    cmd_execute
};

#ifdef _WIN32
    #define PROJ_LIST_DIR "Horst\\build\\proj_list.horstproj"
#else
    #define PROJ_LIST_DIR "Horst/build/proj_list.horstproj"
#endif

bool proj_list(string target, enum proj_method method)
{
    switch(method)
    {
        case proj_add:
        {
            std::ifstream file;
            file.open(PROJ_LIST_DIR);
            vector<string> lines;
            while (!file.eof() and file.is_open() and file)
            {
                string line;
                std::getline(file, line);
                lines.push_back(line + "\n");
            }
            file.close();
            lines.push_back(string(target));

<<<<<<< Updated upstream
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

                vector<string> lines;
                
                while (!file.eof() and file.is_open() and file)
                {
                    string line;
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
            string line;
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
=======
    String T;
    T = "cp -r \"";
    T += exe_path;
    T += "/Horst/";
    T += "prep\" \"";
    T += exe_path;
    T += "/";
    T += target;
    T += "\"";
    system(T.c_str());

    T = "mv \"";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build/";
    T += "prep.horstproj\" \"";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build/";
    T += target;
    T += ".horstproj\"";
    system(T.c_str());
>>>>>>> Stashed changes
}

void copy_dependencies(string target)
{
    vector<string> dependencies;
    vector<string> types;

    std::stringstream ds(attributes[9]);
    std::stringstream dt(attributes[10]);
    
    while (ds.good()) {
        string substr;
        std::getline(ds, substr, ' ');
        dependencies.push_back(substr);
    }

    while (dt.good()) {
        string substr;
        std::getline(dt, substr, ' ');
        types.push_back(substr);
    }
    
    for (int i = 1; i < (int)dependencies.size(); i++)
    {
        if (dependencies[i].empty())
            continue;
        
        if (!proj_list(dependencies[i],proj_check))
        {
            std::cerr << "Konnte die Dependency in der Projekt-Liste nicht finden!: " << "\"" << dependencies[i] << "\"" << std::endl;
            continue;
        }

        struct stat info;

        if (stat(string(target + "/libs/include/" + dependencies[i]).c_str(), &info ) == 0 && !(info.st_mode & S_IFDIR))
            create_directory(target + "/libs/include/" + dependencies[i]);

        copy_file(dependencies[i] + "/src/*.h",target + "libs/include/" + dependencies[i]);

        std::cout << types[i] << " library " << "\"" << dependencies[i] << "\"" << " wird kompiliert und nach \"" << target << "\" kopiert!" << std::endl;
                
        if (strstr(types[i].c_str(),"dynamic") != NULL)
        {
            system(string("cd && Horst dlib " + dependencies[i]).c_str());
            #ifdef _WIN32
            copy_file(dependencies[i] + "\\build\\lib" + dependencies[i] + ".dll", target + "\\libs\\lib");
            #elif defined __linux__
            copy_file(dependencies[i] + "/build/lib" + dependencies[i] + ".so", target + "/libs/lib");
            #elif defined __APPLE__
            copy_file(dependencies[i] + "/build/lib" + dependencies[i] + ".dylib", "/usr/local/lib/lib" + dependencies[i] + ".dylib");
            #endif
        }
        else if (strstr(types[i].c_str(),"static") != NULL)
        {
            system(string("cd && Horst slib " + dependencies[i]).c_str());
            #ifdef _WIN32
            copy_file(dependencies[i] + "\\build\\lib" + dependencies[i] + ".lib", target + "\\libs\\lib");
            #else
            copy_file(dependencies[i] + "/build/lib" + dependencies[i] + ".a", target + "/libs/lib");
            #endif
        }
        else
            printf("Weder \"static\" noch \"dynamic\" angegeben, ignoriere dependency: %s!\n",dependencies[i].c_str());
    }

    std::cout << "Dependencies für \"" << target << "\" wurden kopiert!" << std::endl;
}


void get_attributes(string target)
{
<<<<<<< Updated upstream
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

    const unsigned int NUM_ATTRIBUTES = 12;

    string attr_template[NUM_ATTRIBUTES] = {
    "gxx: ","gxxflags: ","cxxflags: ", "source:", "lib_path:", "includes:", "libraries: ","out: ", "debugger: ", "dependencies: ", "d_types: ", "defines: "
    };

    string vars[NUM_ATTRIBUTES] = {
    "COMPILER_NAME","LINKER_FLAGS","COMPILER_FLAGS","SOURCE","LIB_PATH", "INCLUDES", "LIBRARIES","EXECUTABLE_NAME", "DEBUGGER", "DEPENDENCIES", "DEPENDENCY_TYPE", "DEFINES"
    };

    std::ifstream file;
    file.open(home_dir + "\\" + target + "\\build\\" + target + ".horstproj");
    while (!file.eof() and file.is_open() and file)
    {
        string line;
        std::getline(file, line);

        for (int i = 0; i < NUM_ATTRIBUTES; i++)
            if (line.find(attr_template[i]) != string::npos)
                attributes.push_back(line);
    }

    string temp[NUM_ATTRIBUTES];
    for (int j = 0; j < NUM_ATTRIBUTES; j++)
    {
        for (int i = 0; i < attributes.size(); i++)
        {
            if (attributes[i].find(attr_template[j]) != string::npos)
            {
                replace(attributes[i],attr_template[j],"");
                temp[j] = attributes[i];
            }
        }
    }
    attributes.clear();
    for (int i = 0; i < NUM_ATTRIBUTES; i++)
        attributes.push_back(temp[i]);
    for (int i = 0; i < commands.size(); i++)
    {
        for (int j = 0; j < NUM_ATTRIBUTES; j++)
        {
            if (commands[i].find(vars[j]) != string::npos)
            {
                replace(commands[i],vars[j],attributes[j]);
            }
        }
    }
}

void Horst_new(string target)
{
    if (proj_list(target, proj_check))
        return;
    proj_list(target,proj_add);
    create_directory(string("\"" + home_dir + "\"\\\"" + target + "\""));
    copy_file(string("Horst\\prep "), string("\"" + home_dir + "\"\\\"" + target + "\""));
    rename_file(string("\"" + home_dir + "\\" + target + "\\build\\prep.horstproj\""), string("\"" + target + ".horstproj\""));
}

void Horst_delete(string target)
{
    if (!proj_list(target, proj_check))
        return;
    proj_list(target,proj_remove);
    delete_directory(string("\"" + home_dir + "\"\\\"" + target + "\""));

}

void Horst_dlib(string target)
{
    string T = "cd ";
    T += target;
    T += "\\build && ";
    T += commands[cmd_dynamic];
=======
    copy_dependencies(attributes,target);
    std::cout << commands[cmd_compile] + "\n" + commands[cmd_link] << std::endl;
    String T = "cd \"";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build\"";
    T += " &&" + commands[cmd_compile] + " && " + commands[cmd_link];
    #ifndef _WIN32
    if (strstr(attributes[2].c_str(),"-g") != NULL)
    {
         T += " && dsymutil ";
         T += attributes[7];
    }
    #endif
    T += " && rm -f \"";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build/*.o\"";
>>>>>>> Stashed changes
    system(T.c_str());
}

void Horst_slib(string target)
{
<<<<<<< Updated upstream
    string T = "cd ";
    T += target;
    T += "\\build && ";
    T += commands[cmd_static];
=======
    String T = "cd \"";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build\"";
    T += "&& " + commands[cmd_execute];
>>>>>>> Stashed changes
    system(T.c_str());
}

void Horst_build(string target)
{
    copy_dependencies(target);
    string T = "cd ";
    T += target;
    T += "\\build && ";
    T += commands[cmd_compile] + " && " + commands[cmd_link];
    system(T.c_str());
}

void Horst_run(string target)
{
<<<<<<< Updated upstream
    string T = "cd ";
    T += target;
    T += "\\build && ";
    T += commands[cmd_execute];
=======
    copy_dependencies(attributes,target);
    std::cout << commands[cmd_dynamic] << std::endl;
    String T = "cd \"";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build\"";
    T += " &&" + commands[cmd_dynamic];
>>>>>>> Stashed changes
    system(T.c_str());
}

void Horst_do(string target)
{
<<<<<<< Updated upstream
    string T = "Horst build ";
    T += target;
    T += " && Horst run ";
    T += target;
=======
    copy_dependencies(attributes,target);
    std::cout << commands[cmd_static] << std::endl;
    String T = "cd \"";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build\"";
    T += " &&" + commands[cmd_static];
>>>>>>> Stashed changes
    system(T.c_str());
}

Horst_call get_call(string method)
{
    if (method == "new")
    {
        return Horst_new;
    }
    else if (method == "delete")
    {
        return Horst_delete;
    }
    else if (method == "dlib")
    {
        return Horst_dlib;
    }
    else if (method == "slib")
    {
        return Horst_slib;
    }
    else if (method == "build")
    {
        return Horst_build;
    }
    else if (method == "run")
    {
        return Horst_run;
    }
    else if (method == "do")
    {
        return Horst_do;
    }

    return NULL;
}