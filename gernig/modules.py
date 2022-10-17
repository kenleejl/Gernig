from .consts import *

# init classes for all noise functions
class PrintNoise:
    def __init__(self, text="") -> None:
        self.text = text

class DnsNoise:
    def __init__(self, arg="") -> None:
        self.arg = arg
        dns_prep('noise')

class FileNoise:
    def __init__(self, arg="") -> None:
        self.arg = arg


# init classes for analysis check functions

class DnsAnalysis:
    """
    Generates a small list of domains to check that the dns resolver on host machine resolves domain names properly

    If you wish to specify a list of domains to query for DNS checking then just edit 
    the fake-domains.txt and resolved-domains.txt files accordingly
    
    :force: pass the word "force" to this function to force regeneration of domain names; default will not regenerate domain names if
    fake-domains.txt and resolved-domains.txt exist under the dns folder
    """
    def __init__(self, arg="") -> None:
        self.arg = arg
        if self.arg == "force":
            dns_prep('forceanalysis')
        else:
            dns_prep('analysis')
        

# does prep work for dns function
def dns_prep(dns_type):
    import requests
    import socket
    import random
    import os

    NUM_DOMAINS = 10

    if os.path.isdir('dns'):
        if dns_type == "forceanalysis":
            pass
        elif dns_type == 'analysis':
            if os.path.isfile(RESOLVED_DOMAINS_FILEPATH) and os.path.isfile(FAKE_DOMAINS_FILEPATH):
                return None
        elif dns_type == 'noise':
            if os.path.isfile(WORDLIST_FILEPATH) and os.path.isfile(TLD_FILEPATH):
                return None
    
    wordlist = []
    tld_list = []
    os.makedirs("dns", exist_ok=True)
    if  os.path.isfile(WORDLIST_FILEPATH) and os.path.isfile(TLD_FILEPATH):
        with open(WORDLIST_FILEPATH, "r") as rf:
            wordlist = rf.read().split('\n')
        with open(TLD_FILEPATH, "r") as rf:
            tld_list = rf.read().split('\n')
    else:
        url = "https://www.randomlists.com/data/words.json"
        request = requests.get(url)
        if request.ok:
            wordlist = list(request.json()['data'])
        else:
            print("Unable to retrieve wordlist to generate domain names. Quitting...")
            exit(1)

        url = "https://data.iana.org/TLD/tlds-alpha-by-domain.txt"
        request = requests.get(url)
        if request.ok:
            tld_txt = request.text
            tld_list = [i.lower() for i in tld_txt.split('\n') if i.isalpha()]
        else:
            print("Unable to retrieve the list of TLDs to generate domain names. Quitting...")
            exit(1)

    
    if dns_type == 'noise':
        with open(WORDLIST_FILEPATH, "w") as wf:
            wf.write("\n".join(wordlist))
        with open(TLD_FILEPATH, "w") as wf:
            wf.write("\n".join(tld_list))
    elif dns_type == 'analysis' or dns_type == 'forceanalysis':
        rand_resolved_domains = []
        rand_fake_domains = []
        sys_rand = random.SystemRandom()
        while True:
            rand_word1 = wordlist[sys_rand.randint(0, len(wordlist)-1)]
            rand_word2 = wordlist[sys_rand.randint(0, len(wordlist)-1)]
            rand_tld = tld_list[sys_rand.randint(0, len(tld_list)-1)]
            query_domain = f"{rand_word1}{rand_word2}.{rand_tld}"
            try:
                print(f"Querying {query_domain}")
                socket.getaddrinfo(query_domain, 0)
                print(f"Domain {query_domain} exists.")
                if len(rand_resolved_domains) < NUM_DOMAINS:
                    rand_resolved_domains.append(query_domain)
                if len(rand_resolved_domains) == NUM_DOMAINS and len(rand_fake_domains) == NUM_DOMAINS:
                    break
            except socket.gaierror:
                print(f"Domain {query_domain} does not exist.")
                if len(rand_fake_domains) < NUM_DOMAINS:
                    rand_fake_domains.append(query_domain)
                else:
                    continue
        
        print(f"Resolved domains: {rand_resolved_domains}")
        print(f"Fake domains: {rand_fake_domains}")

        with open(RESOLVED_DOMAINS_FILEPATH, "w") as wf:
            wf.write("\n".join(rand_resolved_domains))
        
        with open(FAKE_DOMAINS_FILEPATH, "w") as wf:
            wf.write("\n".join(rand_fake_domains))