#include "prefix.h"

using namespace kletch;

extern int smk1_cond(int argc, char** argv);

int main(int argc, char** argv)
{
    if (argc < 2)
        return 1;
    string bench_name = argv[1];
    --argc;
    ++argv;
    if (bench_name == "smk1_cond")
        return smk1_cond(argc, argv);
    return 1;
}
