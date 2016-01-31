#include <string>
#include <cstring>
using std::string;
using std::strcmp;

#include <iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include <fstream>

string path_to_varname(const string& path);

int main(int argc, char** argv)
{
    int argi = 1;
    string prefix;
    if (argi < argc && argv[argi][0] == '-')
    {
        if (strcmp(argv[argi], "--prefix") != 0)
        {
            cerr << "Invalid argument: " << argv[1] << endl;
            return 1;
        }
        ++argi;
        if (argc < argi)
        {
            cerr << "Expected path after --prefix" << endl;
            return 1;
        }
        prefix = argv[argi];
        ++argi;
    }

    if (argc < argi)
    {
        cerr << "Expected declaration output file" << endl;
        return 1;
    }
    std::ofstream decl_out(argv[argi]);
    if (!decl_out)
    {
        cerr << "Can't open file for writing: " << argv[argi] << endl;
        return 1;
    }
    ++argi;

    if (argc < argi)
    {
        cerr << "Expected definition output file" << endl;
        return 1;
    }
    std::ofstream def_out(argv[argi]);
    if (!def_out)
    {
        cerr << "Can't open file for writing: " << argv[argi] << endl;
        return 1;
    }
    ++argi;

    std::ifstream in;
    while (argi < argc)
    {
        in.close();
        in.open(argv[argi]);
        if (!in)
        {
            cerr << "Can't open file for reading: " << argv[argi] << endl;
            return 1;
        }
        string varname = path_to_varname(argv[argi]);
        decl_out << "extern Resource " << varname << ";\n";
        def_out << "Resource " << varname << "(\"";
        const char* arg = argv[argi];
        int i = 0;
        while (i < prefix.size() && arg[i] == prefix[i])
            ++i;
        while (arg[i] != '\0')
        {
            if (arg[i] == '\\')
                def_out << "\\\\";
            else
                def_out << arg[i];
            ++i;
        }
        def_out << "\", \"";
        while (in)
        {
            char c = in.get();
            if (in)
            {
                switch (c)
                {
                    case '\"': def_out << "\\\""; break;
                    case '\n': def_out << "\\n"; break;
                    case '\r': def_out << "\\r"; break;
                    case '\t': def_out << "\\t"; break;
                    default: def_out << c; break;
                }
            }
        }
        def_out << "\\n\");\n";
        ++argi;
    }

    return 0;
}

string path_to_varname(const string& path)
{
    int k = path.find_last_of("/\\");
    string filename = k != string::npos ? path.substr(k + 1) : path;
    for (int i = 0; i < filename.size(); ++i)
    {
        char c = filename[i];
        if (c == ' ' || c == '.')
            filename[i] = '_';
    }
    return filename;
}
