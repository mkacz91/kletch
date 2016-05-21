#include "prefix.h"

#include <lib/display_fresnel.h>
using namespace kletch;

TEST(FresnelTest, eval_c)
{
    // Values taken from WolframAlpha
    real values[][2] = {
        {rl(0.0), rl(0.0000000000000000000000000000)},
        {rl(0.1), rl(0.0999975326270850680504666909)},
        {rl(0.5), rl(0.4923442258714463928788436651)},
        {rl(1.0), rl(0.7798934003768228294742064136)},
        {rl(2.0), rl(0.4882534060753407545002235033)},
        {rl(4.0), rl(0.4984260330381776155307095868)},
        {rl(8.0), rl(0.4998021803771971355569838602)},

        {rl(-1.0), rl(-1.0)}
    };

    real tol = rl(1.7e-3);
    int i = 0;
    while (values[i][0] != rl(-1.0))
    {
        EXPECT_NEAR(values[i][1], DisplayFresnel::eval_c(values[i][0]), tol);
        ++i;
    }
}
