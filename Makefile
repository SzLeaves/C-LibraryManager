object = main.o login.o operation.o showmenu.o database.o files.o

LibraryManager.exe: $(object)
	cc -o LibraryManager.exe $(object)

main.o: database.h operation.h
	cc -c main.c

login.o: database.h operation.h
	cc -c login.c

operation.o: operation.h
	cc -c operation.c

showmenu.o: database.h operation.h
	cc -c showmenu.c

database.o: database.h
	cc -c database.c

files.o: files.h
	cc -c files.c

.PHONY: clean
clean:
	-rm $(object)