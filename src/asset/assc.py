import struct

class AssetWriter:
    def __init__(self, stream, **kwargs):
        self.stream = stream
        self.size = 0
        src_path = kwargs.get('src_path', "")
        stream.write(b'----')
        stream.write(src_path.encode())
        stream.write(b'\0')

    def write(self, data):
        self.stream.write(data)
        self.size += len(data)

    def write_str(self, data):
        self.write(data.encode())

    def close(self):
        self.stream.seek(0)
        self.stream.write(struct.pack('i', self.size))
        self.stream.close()
        self.stream = None

if __name__ == '__main__':
    import argparse
    import os
    parser = argparse.ArgumentParser(description="Compile file into an asset")
    parser.add_argument('src', help="source file path", type=str)
    parser.add_argument('-o', '--out', help="output file path", type=str)
    parser.add_argument('--src-dir', help="source directory", type=str)
    parser.add_argument('--out-dir', help="output directory", type=str)
    parser.add_argument('-v', '--verbose', help="enable verbose output", action='store_true')
    args = parser.parse_args();

    verbose = args.verbose

    src_path = args.src
    out_path = args.out
    if out_path is None:
        out_path = os.path.splitext(src_path)[0] + '.ass'
    if args.src_dir is not None:
        src_path = os.path.join(args.src_dir, src_path)
    if args.out_dir is not None:
        out_path = os.path.join(args.out_dir, out_path)

    if verbose:
        print("Source:", src_path)
        print("Output:", out_path)

    if src_path == out_path:
        print("Output file must be different from the source file")
        exit(1)

    try:
        src_file = open(src_path, 'rb')
    except Exception as e:
        print("Unable to open source file.", e)
        exit(1)
    try:
        dst_file = open(out_path, 'wb')
    except Exception as e:
        print("Unable to open output file.", e)
        exit(1)

    writer = AssetWriter(dst_file, src_path=src_path)
    writer.write(src_file.read())
    if verbose:
        print("Size:", writer.size, "bytes")

    writer.close()
    src_file.close()
