run: asm
	rkemu out.rk.bin -e main.el -d main.dl

dbg: asm
	rkemu -a out.rk.bin -e main.el -d main.dl

asm: ../lib/start.rk ../lib/intr.rk ../lib/serial.rk main.rk
	rkasm $^
