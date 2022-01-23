FLAGS    = -fno-stack-protector -z execstack
FLAGS_32 = -static -m32

all:
	gcc $(FLAGS) $(FLAGS_32) -o vul vul.c
	gcc $(FLAGS) $(FLAGS_32) -o fmtvul fmtvul.c
