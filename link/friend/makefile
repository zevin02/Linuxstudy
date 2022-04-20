obj=mytest.o add.o sub.o

mytest:$(obj)
		gcc -o $@ $^
%.o:%.c 
		gcc -c $<

%.o:./other/%.c
		gcc -c $<
