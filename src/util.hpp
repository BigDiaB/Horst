#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>
#include <vector>

#include <sys/stat.h>

#include <windows.h>
#include <direct.h>
#define chdir _chdir

#define String std::string
#define Vector std::vector
#define PROJ_LIST_DIR "Horst/build/proj_list.horstproj"
#define NUM_ATTRIBUTES 9
#define version() std::cout << "Horst Version: " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << std::endl;

Vector<String> attributes;
Vector<String> commands;
char exe_path[2056];

enum command_list
{
    cmd_compile,
    cmd_link,
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
    commands.push_back("COMPILER_NAME COMPILER_FLAGS -c SOURCE INCLUDES DEFINES"); //compile standard executable
    commands.push_back("COMPILER_NAME LINKER_FLAGS -o EXECUTABLE_NAME.exe *.o INCLUDES LIB_PATH LIBRARIES"); //link standard executable
    commands.push_back("EXECUTABLE_NAME.exe");

    String attr_template[NUM_ATTRIBUTES] = {
    "gxx: ","gxxflags: ","cxxflags: ", "source:", "lib_path:", "includes:", "libraries: ","out: ", "defines: "
    };

    String vars[NUM_ATTRIBUTES] = {
    "COMPILER_NAME","LINKER_FLAGS","COMPILER_FLAGS","SOURCE","LIB_PATH", "INCLUDES", "LIBRARIES","EXECUTABLE_NAME", "DEFINES"
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