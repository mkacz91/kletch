#include "prefix.h"

#include <lib/hello.h>
using namespace kletch;

TEST (HelloTest, Hello) {
    Hello hello;
    ASSERT_EQ("Hello World!", hello.hello());
}
