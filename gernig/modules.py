
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
    def __init__(self, arg="") -> None:
        self.arg = arg
        dns_prep('analysis')
        

# does prep work for dns function
def dns_prep(dns_type):
    import requests
    import socket
    import random
    import os

    resolved_domains_file = os.path.join('dns', 'resolved-domains.txt')
    fake_domains_file = os.path.join('dns', 'fake-domains.txt')
    wordlist_file = os.path.join('dns', 'wordlist.txt')
    tld_file = os.path.join('dns', 'tld.txt')

    if os.path.isdir('dns'):
        if dns_type == 'analysis':
            if os.path.isfile(resolved_domains_file) and os.path.isfile(fake_domains_file):
                return None
        elif dns_type == 'noise':
            if os.path.isfile(wordlist_file) and os.path.isfile(tld_file):
                return None
    
    url = "https://www.randomlists.com/data/words.json"
    request = requests.get(url)
    wordlist = list(request.json()['data'])

    url = "https://data.iana.org/TLD/tlds-alpha-by-domain.txt"
    request = requests.get(url)
    tld_txt = request.text
    tld_list = [i.lower() for i in tld_txt.split('\n') if i.isalpha()]

    os.makedirs("dns", exist_ok=True)
    if dns_type == 'noise':
        with open(wordlist_file, "w") as wf:
            wf.write("\n".join(wordlist))
        with open(tld_file, "w") as wf:
            wf.write("\n".join(tld_list))
    elif dns_type == 'analysis':
        rand_5_resolved_domains = []
        rand_5_fake_domains = []
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
                if len(rand_5_resolved_domains) < 5:
                    rand_5_resolved_domains.append(query_domain)
                if len(rand_5_resolved_domains) == 5 and len(rand_5_fake_domains) == 5:
                    break
            except socket.gaierror:
                print(f"Domain {query_domain} does not exist.")
                if len(rand_5_fake_domains) < 5:
                    rand_5_fake_domains.append(query_domain)
                else:
                    continue
        
        print(f"Resolved domains: {rand_5_resolved_domains}")
        print(f"Fake domains: {rand_5_fake_domains}")

        with open(resolved_domains_file, "w") as wf:
            wf.write("\n".join(rand_5_resolved_domains))
        
        with open(fake_domains_file, "w") as wf:
            wf.write("\n".join(rand_5_fake_domains))