import os
from pathlib import Path
import subprocess
import sys


SOURCES_EXT = {".c", ".cc", ".cpp", ".cxx"}
# HEADERS_EXT = {'.h', '.hh', '.hpp', '.hxx'}


class CompileExtern:

    DEFAULT_CFLAGS = [
        # '-D__STDC_FORMAT_MACROS',
        "-fPIC",
        "-shared",
        "-Wl,--no-undefined",
    ]

    DEFAULT_LIBS = ["stdc++", "m"]

    def __init__(self, name):
        self.name = name
        self.sources = []
        self.include_dirs = []
        self.libs = []
        self.cflags = []

    @staticmethod
    def create_clib(solver):
        compilation = CompileExtern(solver)
        extern_path = f"./Extern/sat/{solver}"
        module_path = f"./Module/sat/{solver}"
        interface_path = str(Path("../FrameworkInterface").resolve())
        compilation.find_c_sources(extern_path)
        compilation.include_dirs.append(extern_path)
        compilation.find_c_sources(module_path)
        compilation.include_dirs.append(module_path)
        compilation.find_c_sources(interface_path)
        compilation.include_dirs.append(interface_path)

        compilation.cflags += CompileExtern.DEFAULT_CFLAGS
        compilation.cflags += CompileExtern.get_compilation_flags(
            f"./Module/sat/{solver}"
        )
        compilation.libs += CompileExtern.DEFAULT_LIBS
        compilation.check_flags()
        return compilation

    @staticmethod
    def get_compilation_flags(path):
        flags_path = Path(path) / "flags.txt"
        if not flags_path.is_file():
            return []
        with open(str(flags_path)) as f:
            flags = []
            for line in f:
                flags += line.split()
            return flags

    def check_flags(self):
        flags = set(self.cflags)
        if not "-O2" in flags and not "-O3" in flags:
            flags.add("-O2")
        self.cflags = list(flags)

    def find_c_sources(self, root_dir):
        for root, _, files in os.walk(root_dir):
            for name in files:
                _, ext = os.path.splitext(name)
                if ext in SOURCES_EXT:
                    self.sources.append(os.path.join(root, name))

    def get_compile_command(self, output_dir):
        command = ["gcc"]
        command += self.cflags
        include = []
        for path in self.include_dirs:
            include.append("-I")
            include.append(path)
        command += include
        command += self.sources
        command += ["-o", f"{output_dir}/lib{self.name}.so"]
        command += ["-l" + x for x in self.libs]
        return command

    def __str__(self):
        return str(vars(self))


if __name__ == "__main__":
    if len(sys.argv) < 2:
        name = None
    else:
        name = sys.argv[1]
    # Change directory to current directory
    mv_to_dir = os.path.dirname(__file__)
    if mv_to_dir != "":
        os.chdir(mv_to_dir)

    OUTPUT_PATH = "./build"
    if not os.path.isdir(OUTPUT_PATH):
        os.mkdir(OUTPUT_PATH)
    for solver in os.listdir("./Module/sat"):
        if name is not None and name != solver:
            continue
        compilation = CompileExtern.create_clib(solver)
        print(compilation.get_compile_command(OUTPUT_PATH))
        subprocess.run(compilation.get_compile_command(OUTPUT_PATH))
