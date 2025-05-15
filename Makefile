all:
	@nasm -f elf32 functions.asm -o functions.o  
	@gcc -m32 -no-pie main.c functions.o -o main  
	@./main  

test:
	@nasm -f elf32 functions.asm -o functions.o  
	@gcc -m32 -no-pie main.c functions.o -o main  
	@./main -t r 1 2 5 6 0.0001

clean:
	@rm main functions.o