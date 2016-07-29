import os
import importlib
import genutil

root_dir = os.path.dirname(os.path.realpath(__file__))
out_dir = os.path.join(root_dir, 'data')
os.makedirs(out_dir, exist_ok=True)

def gen(name, *args):
    module_name = 'gen_' + name;
    module_path = os.path.join(root_dir, module_name + '.py')
    out_path = os.path.join(out_dir, name + '.txt')
    module_mtime = os.path.getmtime(module_path)
    out_mtime = os.path.getmtime(out_path) if os.path.exists(out_path) else module_mtime
    if module_mtime >= out_mtime:
        print("[u]", out_path)
        module = importlib.import_module(module_name)
        values = module.gen(*args)
        with open(out_path, 'w') as out:
            genutil.print_values(values, file=out)
    else:
        print("[ ]", out_path)

if __name__ == '__main__':
    gen('fresnel_standard', 0, 5, 100)
