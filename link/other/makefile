#动态库与位置无关，库文件可以在任何位置加载，而且不影响和其他程序的关联性，相对地址的偏移量
#形成一个共享库，
libmymath.so:add.o sub.o
	gcc -shared -o $@ $^ 
%.o:%.c
	gcc -fPIC -c $<

.PHONY:clean
clean:
	rm -f libmymath.so

.PHONY:lib 
lib:
	mkdir lib
	cp *.h lib
	cp libmymath.so lib

# libmymath.a:sub.o add.o
# 		ar -rc $@ $^
# %.o:%.c
# 		gcc -c $<

# .PHONY:clean
# clean:
# 		rm -rf *.o libmymath.a output

# .PHONY:output
# output:
# 		mkdir output 
# 		cp -rf *.h output 
# 		cp libmymath.a output
