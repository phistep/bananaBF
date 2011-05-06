banana: main.o
	gcc main.o -o banana
	
main.o: main.c
	gcc -c main.c
	
install:
	@cp ./banana /usr/bin && echo "\"banana\" successfully installed in \"/usr/bin\""

clean:
	@rm -rf banana main.o && echo "\"banana\" and \"main.o\" successfully deleted"