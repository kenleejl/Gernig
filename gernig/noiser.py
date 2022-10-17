import os
import subprocess
from .consts import *
from .bin2array import do_conversion
from .modules import *

class Noiser:
    def __init__(self, bin_path) -> None:
        self.bin_path = bin_path
        self.script_dir = os.path.dirname(os.path.realpath(__file__))
        self.include_path = os.path.join(self.script_dir, "include") 
        self.defines_path = os.path.join(self.include_path, FILENAME_DEFINES_HEADER) 
        open(self.defines_path, "w").close()

    def addNoise(self, noise):
        noise_type = type(noise)
        if noise_type == PrintNoise:
            self.__add_def(PRINT_NOISE_ENABLED)
            self.__add_def(TEMPLATE_PRINT_NOISE_TEXT.format(noise.text))
        elif noise_type == DnsNoise:
            self.__add_def(DNS_NOISE_ENABLED)
            
            wordlist = []
            with open(WORDLIST_FILEPATH, "r") as rf:
                wordlist = ', '.join(["\"" + i.strip('\n') + "\""  for i in rf.readlines()])
            tld = []
            with open(TLD_FILEPATH, "r") as rf:
                tld = ', '.join(["\"" + i.strip('\n') + "\""  for i in rf.readlines()]) 
            dns_path = os.path.join(self.include_path, FILENAME_DNS_NOISE_HEADER) 
            with open(dns_path, "w") as f:
                f.write("#include <vector>\n#include <string>\n\n")
                f.write(TEMPLATE_DNS_NOISE_WORDLIST_ARG.format(wordlist))
                f.write(TEMPLATE_DNS_NOISE_TLD_ARG.format(tld))

        elif noise_type == FileNoise:
            self.__add_def(FILE_NOISE_ENABLED)

    def addAnalysis(self, analysis):
        analysis_type = type(analysis)
        if analysis_type == DnsAnalysis:
            self.__add_def(DNS_ANALYSIS_ENABLED)

            fake_dns = []
            with open(FAKE_DOMAINS_FILEPATH, "r") as rf:
                fake_dns = ', '.join(["\"" + i.strip('\n') + "\""  for i in rf.readlines()])
            real_dns = []
            with open(RESOLVED_DOMAINS_FILEPATH, "r") as rf:
                real_dns = ', '.join(["\"" + i.strip('\n') + "\""  for i in rf.readlines()])
            dns_path = os.path.join(self.include_path, FILENAME_DNS_ANALYSIS_HEADER) 
            with open(dns_path, "w") as f:
                f.write("#include <vector>\n#include <string>\n\n")
                f.write(TEMPLATE_DNS_ANALYSIS_FAKE_ARG.format(fake_dns))
                f.write(TEMPLATE_DNS_ANALYSIS_REAL_ARG.format(real_dns))


    def __add_def(self, content):
        with open(self.defines_path, "a") as f:
            f.write(TEMPLATE_DEFINE.format(content))

    def __bin_to_header(self):
        print(f"Embedding {self.bin_path}")
        with open(self.bin_path, "rb") as f:
            bin_raw = f.read()
        bin_hex = do_conversion(bin_raw)
        binexp_path = os.path.join(self.include_path, FILENAME_BINEXP_HEADER) 
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
                "src/modules/dns.cpp",
                "src/modules/file.cpp",
                "src/modules/network.cpp",
                "src/modules/print.cpp",
                "src/modules/registry.cpp",
                "-Iinclude",
                "-lws2_32",
            ]
        )
        print(f"Success")
