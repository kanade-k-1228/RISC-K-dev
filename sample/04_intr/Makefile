all: ../lib/start.rk ../lib/intr.rk ../lib/serial.rk main.rk
	rkasm $^
	rkemu -a out.rk.bin -i main.intr -d main.dp -t 20
