#pragma once

void Horst_new(char* target)
{
    if (proj_list(target,proj_check))
    {
        std::cout << "\"" << target << "\" ist bereits in der Projektliste!" << std::endl;
        return;
    }
    proj_list(target,proj_add);

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
}

void Horst_delete(char* target)
{
    proj_list(target,proj_remove);

    String T;
    T = "rm -rf ";
    T += target;
    system(T.c_str());
}

void Horst_build(char* target)
{
    std::cout << commands[cmd_compile] + "\n" + commands[cmd_link] << std::endl;
    String T = "cd \"";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build\"";
    T += " &&" + commands[cmd_compile] + " && " + commands[cmd_link];
    T += " && rm -f \"";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build/*.o\"";
    system(T.c_str());
}
void Horst_run(char* target)
{
    String T = "cd \"";
    T += exe_path;
    T += "/";
    T += target;
    T += "/build\"";
    T += "&& " + commands[cmd_execute];
    system(T.c_str());
}
void Horst_do(char* target)
{
    Horst_build(target);
    Horst_run(target);
}

typedef void (*Horst_call)(char*);

enum Horst_call_index {
    HCI_new = 0,
    HCI_delete,

    HCI_build,
    HCI_run,
    HCI_do,

    HCI_NUM_CALLS
};

Horst_call Horst_calls[HCI_NUM_CALLS] = {
    Horst_new,
    Horst_delete,

    Horst_build,
    Horst_run,
    Horst_do,
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