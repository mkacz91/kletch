def gen(name):
    return importlib.import_module(name).gen()

def dispatch_cpp(name, out_dir):
    values = gen(name)
    m, n = dims(values)
    with open(os.path.join(out_dir, name + '.h')) as header:
        guard = 'KLETCH_TEST_GEN_' + name.upper() + '_H'
        print("#ifndef", guard, file=header)
        print("#define", guard, file=header)
        print("namespace kletch {{ namespace gen {{ namespace test {{", file=header)
        print("    extern double", name, "[]")
        print("}}}}}} // namespace kletch::gen::test", file=header)
        print("#endif //", guard, file=header)
