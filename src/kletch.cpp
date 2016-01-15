#include "prefix_headers.h"
#include "prefix_usings.h"

#include "hello.h"
using namespace kletch;

int main(int argc, char** argv)
{
    Hello hello;
    cout << hello.hello() << endl;
}
