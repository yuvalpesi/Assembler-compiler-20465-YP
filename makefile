CC = gcc
EXEC = assembler
objC = compiler.o
objS = commandLine.o
objP = preProccesor.o
objU = utils.o
objT = tables.o
objF = firstPass.o
objFC = firstPassCodeHandle.o
objFD = firstPassDirectiveHandle.o
objN = Node.o
objSe = secPass.o
objE = ExportFile.o
COMP_FLAG = -ansi -Wall -pedantic
DEBUG_FLAG = -g

$(EXEC): $(objC) $(objS) $(objP) $(objU) $(objT) $(objF) $(objFD) $(objFC) $(objN) $(objSe) $(objE)
	$(CC) $(COMP_FLAG) $(DEBUG_FLAG) $(objC) $(objS) $(objP) $(objU) $(objT) $(objF) $(objFD) $(objFC) $(objN) $(objSe) $(objE) -o $@

compiler.o: compiler.c data.h
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c -o $@

commandLine.o: commandLine.c
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c -o $@

preProccesor.o: preProccesor.c
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c -o $@

utils.o: utils.c
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c -o $@

tables.o: tables.c
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c -o $@

firstPass.o: firstPass.c
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c -o $@

firstPassCodeHandle.o: firstPassCodeHandle.c
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c -o $@

firstPassDirectiveHandle.o: firstPassDirectiveHandle.c
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c -o $@

Node.o: Node.c
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c -o $@

secPass.o: secPass.c
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c -o $@

ExportFile.o:ExportFile.c
	$(CC) -c $(COMP_FLAG) $(DEBUG_FLAG) $*.c -o $@

clean:
	-rm $(objC) $(objS) $(objP) $(objU) $(objT) $(objF) $(objN) $(objSe) $(objE) $(objFD) $(objFC) $(EXEC)