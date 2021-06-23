from os import listdir
from os import system
from os.path import isfile, join
tb = [f for f in listdir("bin") if isfile(join("bin", f))]
for t in tb:
    print("====== Testing %s ======" % t) 
    cmd = "./emu ./bin/" + t
    system(cmd)
