#pragma once

void Horst_new(char* target)
{
    add_to_proj_list(target);

    String T;
    T = "cp -r ";
    T += exe_path;
    T += "/Horst/";
    T += "prep ";
    T += exe_path;
    T += "/";
    T += target;
    system(T.c_str());

    T = "mv ";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build/";
    T += "prep.horstproj ";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build/";
    T += target;
    T += ".horstproj";
    system(T.c_str());

    T = "Horst build ";
    T += target;
    system(T.c_str());
}

void Horst_delete(char* target)
{
    remove_from_proj_list(target);

    String T;
    T = "rm -rf ";
    T += target;
    system(T.c_str());
}

void Horst_build(char* target)
{
    copy_dependencies(attributes,target);
    std::cout << commands[0] + "\n" + commands[1] << std::endl;

    String T = "cd ";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build";
    T += " &&" + commands[0] + " && " + commands[1];
    if (strstr(attributes[2].c_str(),"-g") != NULL)
    {
         T += "&& dsymutil ";
         T += attributes[7];
    }
    T += " && rm -f ";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build/*.o";
    system(T.c_str());
}
void Horst_run(char* target)
{
    String T = "cd ";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build";
    if (use_lldb(attributes))
        T += "&& " + commands[4];
    else
        T += "&& " + commands[5];

    system(T.c_str());
}
void Horst_do(char* target)
{
    Horst_build(target);
    Horst_run(target);
}

void Horst_dlib(char* target)
{
    copy_dependencies(attributes,target);
    std::cout << commands[3] << std::endl;
    String T = "cd ";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build";
    T += " &&" + commands[3];
    system(T.c_str());
}
void Horst_slib(char* target)
{
    copy_dependencies(attributes,target);
    std::cout << commands[2] << std::endl;
    String T = "cd ";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build";
    T += " &&" + commands[2];
    system(T.c_str());
}

typedef void (*Horst_call)(char*);

enum Horst_call_index {
    HCI_new = 0,
    HCI_delete,
    HCI_build,
    HCI_run,
    HCI_do,
    HCI_dlib,
    HCI_slib,
    HCI_NUM_CALLS
};

Horst_call Horst_calls[HCI_NUM_CALLS] = {
    Horst_new,
    Horst_delete,

    Horst_build,
    Horst_run,
    Horst_do,

    Horst_dlib,
    Horst_slib
};

Horst_call get_call(String call)
{
    DEBUG_MSG("asked for " + call);
    if (call == "new")
    {
        return Horst_calls[HCI_new];
    }
    else if (call == "delete")
    {
        return Horst_calls[HCI_delete];
    }
    else if (call == "dlib")
    {
        return Horst_calls[HCI_dlib];
    }
    else if (call == "slib")
    {
        return Horst_calls[HCI_slib];
    }
    else if (call == "build")
    {
        return Horst_calls[HCI_build];
    }
    else if (call == "run")
    {
        return Horst_calls[HCI_run];
    }
    else if (call == "do")
    {
        return Horst_calls[HCI_do];
    }

    return NULL;
}