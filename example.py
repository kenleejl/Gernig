from gernig.modules import DebugAnalysis, DnsAnalysis, EventlogBlind, FileNoise, MACAddrAnalysis, CPUIDAnalysis, NetworkNoise, ProcessAnalysis, SleepAnalysis, RegistryNoise, UPXBlind
from gernig.noiser import Noiser, DnsNoise, PrintNoise

resolved_domains = [
    "carcelery.ws",
    "transporthandy.ws",
    "chickensalt.co",
    "peacefulprotective.music",
    "woolsystem.ph",
    "marvelousdelicate.music",
    "unarmedvisit.kids",
    "streamoil.ph",
    "fangbottle.kids",
    "suggestdecide.ws"
]

fake_domains = [
    "ugliestform.tz",
    "competitionparsimonious.med",
    "affordiron.voting",
    "coastvisit.jewelry",
    "multiplyflesh.safe",
    "tiresomeefficient.docs",
    "holidaydisgusting.shangrila",
    "wavesconsist.ferrari",
    "dropeasy.fr",
    "crayonsincere.lexus"
]

n = Noiser("target_program.exe")

n.addNoise(PrintNoise("testing"))
# n.addAnalysis(SleepAnalysis(360000))
# n.addNoise(DnsNoise())
# n.addNoise(FileNoise())
# n.addNoise(RegistryNoise())
# n.addNoise(NetworkNoise())


n.addAnalysis(DnsAnalysis("force", num_domain=8))
# n.addAnalysis(MACAddrAnalysis())
# n.addAnalysis(CPUIDAnalysis())
# n.addAnalysis(DebugAnalysis())
# n.addBlind(EventlogBlind())
# n.addAnalysis(ProcessAnalysis())

# Perform UPX obfuscation on the target binary
n.addBlind(UPXBlind())

n.generate("output.exe")
