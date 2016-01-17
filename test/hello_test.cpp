#include "prefix.h"

#include "../src/hello.h"
using namespace kletch;

TEST (HelloTest, Hello) {
    Hello hello;
    ASSERT_EQ("Hello World!", hello.hello());
}
