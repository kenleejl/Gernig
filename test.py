from gernig.modules import ACGBlind, BlockDLLBlind, DebugAnalysis, DnsAnalysis, EventlogBlind, FileNoise, MACAddrAnalysis, CPUIDAnalysis, NetworkNoise, ProcessAnalysis, SleepAnalysis
from gernig.noiser import Noiser, DnsNoise, PrintNoise

n = Noiser("calc.exe")
# n.addNoise(PrintNoise("testing"))
# n.addAnalysis(SleepAnalysis(360000))
# n.addNoise(DnsNoise())
# n.addNoise(FileNoise())
n.addNoise(NetworkNoise())
# n.addAnalysis(DnsAnalysis())
# n.addAnalysis(MACAddrAnalysis())
n.addAnalysis(CPUIDAnalysis())
n.addAnalysis(DebugAnalysis())
n.addBlind(EventlogBlind())
# n.addAnalysis(ProcessAnalysis())
# n.addBlind(ACGBlind())
# n.addBlind(BlockDLLBlind())

n.generate("output.exe")