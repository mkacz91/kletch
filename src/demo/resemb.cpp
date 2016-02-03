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

void ensure_slashend(string& path);

int main(int argc, char** argv)
{
    if (argc != 5)
    {
        cout << "Usage: " << argv[0] << " in_prefix out_prefix suffix varname" << endl;
        return 1;
    }

    string in_prefix = argv[1]; ensure_slashend(in_prefix);
    string out_prefix = argv[2]; ensure_slashend(out_prefix);
    string suffix = argv[3];
    string varname = argv[4];

    string in_path = in_prefix + suffix;
    std::ifstream in(in_path);
    if (!in)
    {
        cerr << "Can't open file for reading: " << in_path << endl;
        return 1;
    }

    string out_path = out_prefix + suffix + ".cpp";
    std::ofstream out(out_path);
    if (!out)
    {
        cerr << "Can't open file for writing: " << out_path << endl;
        return 1;
    }

    out << "namespace kletch { namespace gen { ";
    out << "const char* " << varname << " = \"";

    while (in)
    {
        char c = in.get();
        if (in)
        {
            switch (c)
            {
                case '\"': out << "\\\""; break;
                case '\n': out << "\\n"; break;
                case '\r': out << "\\r"; break;
                case '\t': out << "\\t"; break;
                default: out << c; break;
            }
        }
    }

    out << "\\n\"; } }";
    out.close();

    return 0;
}

void ensure_slashend(string& path)
{
    int k = path.find_last_of("\\/");
    if (k == string::npos)
        path.push_back('/');
    else if (k < path.size() - 1)
        path.push_back(path[k]);
}
