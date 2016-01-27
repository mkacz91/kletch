#include <string>
#include <iostream>
#include <fstream>

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

int main(int argc, char** argv)
{
    const char* header_out_filename = argv[1];
    const char* source_out_filename = argv[2];

    ofstream header_out(header_out_filename);
    ofstream source_out(source_out_filename);

    header_out
        << "#ifndef KLETCH_DEMO_RESOURCE_H\n"
        << "#define KLETCH_DEMO_RESOURCE\n"
        << "\n"
        << "namespace kletch {\n"
        << "\n"
        << "struct Resource\n"
        << "{\n";

    source_out
        << "#include \"" << header_out_filename << "\"\n"
        << "\n"
        << "namespace kletch {\n"
        << "\n";

    string prefix;
    for (int i = 3; i < argc; ++i)
    {
        string filename = argv[i];
        if (filename == "-prefix")
        {
            if (prefix != "")
                header_out << "    };\n\n";

            prefix = argv[++i];
            cout << "Encountered prefix: " << prefix << endl;
            header_out << "    struct " << prefix << "\n    {\n";

            filename = argv[++i];
        }

        int name_end = filename.find_last_of(".");
        name_end = name_end == string::npos ? filename.size() : name_end;
        int name_beg = filename.find_last_of("\\/");
        name_beg = name_beg == string::npos ? 0 : name_beg + 1;
        string name = filename.substr(name_beg, name_end - name_beg);
        cout << "Processing file: " << filename << ", name: " << name << endl;

        header_out
            << "    " << (prefix != "" ? "    " : "")
            << "static const char* " << name << ";\n";

        source_out
            << "const char* Resource::" << (prefix != "" ? prefix + "::" : "") << name << " = \"";

        ifstream fin(filename);
        while (fin)
        {
            char c = fin.get();
            if (fin)
            {
                switch (c)
                {
                    case '\\': source_out << "\\\\"; break;
                    case '\"': source_out << "\\\""; break;
                    case '\n': source_out << "\\n"; break;
                    case '\r': source_out << "\\r"; break;
                    case '\t': source_out << "\\t"; break;
                    default: source_out << c; break;
                }
            }
        }

        source_out << "\";\n";
    }

    header_out
        << "};\n"
        << "\n"
        << "} // namespace kletch\n"
        << endl;
    header_out.close();

    source_out
        << "\n"
        << "} // namespace kletch\n"
        << endl;
    source_out.close();
}
