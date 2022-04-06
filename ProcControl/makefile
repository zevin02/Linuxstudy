.PHONY:all
all:myexe myload 

myload:myload.c
		gcc myload.c -o myload -g
myexe:myexe.c
		gcc myexe.c -o myexe -g
myproc:myproc.c	
		gcc myproc.c -o myproc -g
.PHONY:clean
clean:
		rm -f myproc myexe myload
