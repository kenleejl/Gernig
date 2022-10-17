import os

FILENAME_BINEXP_HEADER = "binexp.h"
FILENAME_DNS_NOISE_HEADER = "dnsnoise.h"
FILENAME_DNS_ANALYSIS_HEADER = "dnsanalysis.h"
FILENAME_DEFINES_HEADER = "defines.h"
FILENAME_NOISY = "noisy.exe"

RESOLVED_DOMAINS_FILEPATH = os.path.join('dns', 'resolved-domains.txt')
FAKE_DOMAINS_FILEPATH = os.path.join('dns', 'fake-domains.txt')
WORDLIST_FILEPATH = os.path.join('dns', 'wordlist.txt')
TLD_FILEPATH = os.path.join('dns', 'tld.txt')

TEMPLATE_CHAR_ARRAY = "unsigned char BINARY_ARRAY[] = {{ {} }};"
TEMPLATE_DEFINE = "#define {}\n"

TEMPLATE_PRINT_NOISE_TEXT = '_PRINT_NOISE_TEXT "{}"'
TEMPLATE_DNS_NOISE_WORDLIST_ARG = 'std::vector<std::string> WORDLIST = {{ {} }};\n'
TEMPLATE_DNS_NOISE_TLD_ARG = 'std::vector<std::string> TLD = {{ {} }};\n'

PRINT_NOISE_ENABLED = "_PRINT_NOISE_ENABLED"
DNS_NOISE_ENABLED = "_DNS_NOISE_ENABLED"
FILE_NOISE_ENABLED = "_FILE_NOISE_ENABLED"


DNS_ANALYSIS_ENABLED = "_DNS_ANALYSIS_ENABLED"
TEMPLATE_DNS_ANALYSIS_FAKE_ARG = 'std::vector<std::string> FAKE_DNS = {{ {} }};\n'
TEMPLATE_DNS_ANALYSIS_REAL_ARG = 'std::vector<std::string> REAL_DNS = {{ {} }};\n'
