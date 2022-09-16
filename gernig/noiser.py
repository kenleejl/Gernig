import os
import subprocess
from .consts import *
from .bin2array import do_conversion


class PrintNoise:
    def __init__(self, text="") -> None:
        self.text = text


class Noiser:
    def __init__(self, bin_path) -> None:
        self.bin_path = bin_path
        self.script_dir = os.path.dirname(os.path.realpath(__file__))
        self.include_path = f"{self.script_dir}/include"
        self.defines_path = f"{self.include_path}/{FILENAME_DEFINES_HEADER}"
        open(self.defines_path, "w").close()

    def addNoise(self, noise):
        noise_type = type(noise)
        if noise_type == PrintNoise:
            self.__add_def(PRINT_NOISE_ENABLED)
            self.__add_def(TEMPLATE_PRINT_NOISE_TEXT.format(noise.text))

    def __add_def(self, content):
        with open(self.defines_path, "a") as f:
            f.write(TEMPLATE_DEFINE.format(content))

    def __bin_to_header(self):
        print(f"Embedding {self.bin_path}")
        with open(self.bin_path, "rb") as f:
            bin_raw = f.read()
        bin_hex = do_conversion(bin_raw)
        binexp_path = f"{self.include_path}/{FILENAME_BINEXP_HEADER}"
        with open(binexp_path, "w") as f:
            f.write(TEMPLATE_CHAR_ARRAY.format(bin_hex))

    def generate(self, fname=FILENAME_NOISY):
        self.__bin_to_header()
        os.chdir(self.script_dir)
        print(f"Compiling {fname}")
        subprocess.run(
            [
                "g++",
                "-o",
                fname,
                "src/DllLoader/MemoryModule.c",
                "src/DllLoader/main.cpp",
                "src/DllLoader/loader.cpp",
                "src/modules/file.cpp",
                "src/modules/network.cpp",
                "src/modules/print.cpp",
                "src/modules/registry.cpp",
                "-Iinclude",
            ]
        )
        print(f"Success")
