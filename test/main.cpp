#include "prefix.h"

kletch::VectorAssetPack<double> Assets::doubles;

int main (int argc, char** argv) {
    Assets::doubles.set_root_dir("assets/doubles");
    auto& foo = Assets::doubles["foo"];
    std::cout << foo.name() << " " << foo.source_path() << std::endl;
    for (double x : *foo)
        std::cout << x << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
