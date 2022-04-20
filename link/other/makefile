libmymath.a:sub.o add.o
		ar -rc $@ $^
%.o:%.c
		gcc -c $<

.PHONY:clean
clean:
		rm -rf *.o libmymath.a output

.PHONY:output
output:
		mkdir output 
		cp -rf *.h output 
		cp libmymath.a output
