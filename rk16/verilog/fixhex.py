with open("build/rom.inv.hex") as f:
    for l in f.readlines():
        print(l[6:8],l[4:6],l[2:4],l[0:2],sep='')
