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

bool proj_list(string target, enum proj_method method)
{
    return false;
}

void get_attributes(string target, vector<string>& attributes)
{

}

void Horst_new(string target)
{
    if (proj_list(target, proj_check))
        return;
    proj_list(target,proj_add);
    create_directory(string("\"" + home_dir + "\"\\\"" + target + "\""));
    copy_file(string("prep "), string("\"" + home_dir + "\"\\\"" + target + "\""));
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

}

void Horst_slib(string target)
{

}

void Horst_build(string target)
{

}

void Horst_run(string target)
{

}

void Horst_do(string target)
{

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