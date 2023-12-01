#pragma once

void Horst_new()
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

void Horst_delete()
{
    proj_list(target,proj_remove);

    String T;
    T = "rm -rf ";
    T += target;
    system(T.c_str());
}

void Horst_build()
{
    std::cout << commands[cmd_compile] + "\n" + commands[cmd_link] << std::endl;
    String T = commands[cmd_directory] + " && cd build";
    T += " && " + commands[cmd_compile] + " && " + commands[cmd_link];
    if (attributes[9] != "dont")
        T += " && " + commands[cmd_cleanup];
    system(T.c_str());
}

void Horst_run()
{
    String T = commands[cmd_directory] + " && cd build";
    T += " && " + commands[cmd_execute];
    system(T.c_str());
}

void Horst_do()
{
    Horst_build();
    puts("Finished building...");
    Horst_run();
}

void Horst_make()
{
    String T = commands[cmd_directory] + " && cd build ";
    T += " && touch makefile";
    system(T.c_str());

    String make_template = "all: build run\n\nbuild:\n\tcd " + String(exe_path) + "/" + String(target) + "/build && ";
    make_template += commands[cmd_compile];
    make_template += "\n\tcd " + String(exe_path) + "/" + String(target) + "/build && ";
    make_template += commands[cmd_link];
    make_template += "\nrun:\n\tcd " + String(exe_path) + "/" + String(target) + "/build && ";
    make_template += commands[cmd_execute];
    make_template += "\n.PHONY: all build run";

    std::cout << make_template << std::endl;

    T = exe_path;
    T += "/";
    T += target;
    T += "/makefile";

    FILE* mf = fopen(T.c_str(),"w");

    replace_keywords(make_template);

    fwrite(make_template.c_str(),make_template.size(),1,mf);

    fclose(mf);

    system(T.c_str());
}

void Horst_slib()
{
    String T = commands[cmd_directory] + " && cd build";
    T += " && " + commands[cmd_compile];
    T += " && " + commands[cmd_staticlib];
    T += " && " + commands[cmd_cleanup];

    system(T.c_str());
}


void Horst_dlib()
{
    String T = commands[cmd_directory] + " && cd build";
    T += " && " + commands[cmd_compile];
    T += " && " + commands[cmd_dynamiclib];
    T += " && " + commands[cmd_cleanup];

    system(T.c_str());
}

void Horst_serve() {
    String T = commands[cmd_directory];
    T += "&& " + commands[cmd_server_setup];

    system(T.c_str());
}

typedef void (*Horst_call)(void);

enum Horst_call_index {
    HCI_new = 0,
    HCI_delete,

    HCI_build,
    HCI_run,
    HCI_do,

    HCI_slib,
    HCI_dlib,

    HCI_make,
    HCI_serve,

    HCI_NUM_CALLS
};

Horst_call Horst_calls[HCI_NUM_CALLS] = {
    Horst_new,
    Horst_delete,

    Horst_build,
    Horst_run,
    Horst_do,


    Horst_slib,
    Horst_dlib,

    Horst_make,
    Horst_serve
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
    else if (call == "make")
    {
        return Horst_calls[HCI_make];
    }
    else if (call == "slib")
    {
        return Horst_calls[HCI_slib];
    }
    else if (call == "dlib")
    {
        return Horst_calls[HCI_dlib];
    }
    else if (call == "serve")
    {
        return Horst_calls[HCI_serve];
    }

    return NULL;
}