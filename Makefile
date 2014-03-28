CFLAGS=-ljack -lpthread -ldl -lrt -lm -O2
DFLAGS=-ljack -lpthread -ldl -lrt -lm -g
PREFIX=bin
OBJ=$(PREFIX)/obj
SRC=src
LIB=$(SRC)/lib

$(PREFIX)/simple_osc: $(OBJ)/data.o $(OBJ)/shell_ui.o $(OBJ)/synth.o $(OBJ)/controller.o \
                      $(OBJ)/notes.o $(SRC)/simple_osc.c Makefile
	mkdir -p $(PREFIX)
	gcc $(SRC)/simple_osc.c $(OBJ)/shell_ui.o $(OBJ)/synth.o $(OBJ)/data.o $(OBJ)/controller.o  \
	$(OBJ)/notes.o -o $(PREFIX)/simple_osc $(CFLAGS)

$(OBJ)/data.o: $(LIB)/data.c Makefile
	mkdir -p $(OBJ)/
	gcc $(LIB)/data.c -o $(OBJ)/data.o -c $(CFLAGS)
	
$(OBJ)/shell_ui.o: $(LIB)/shell_ui.c Makefile
	mkdir -p $(OBJ)/
	gcc $(LIB)/shell_ui.c -o $(OBJ)/shell_ui.o -c $(CFLAGS)
	
$(OBJ)/synth.o: $(LIB)/synth.c Makefile
	mkdir -p $(OBJ)/
	gcc $(LIB)/synth.c -o $(OBJ)/synth.o -c $(CFLAGS)

$(OBJ)/controller.o: $(LIB)/controller.c Makefile
	mkdir -p $(OBJ)/
	gcc $(LIB)/controller.c -o $(OBJ)/controller.o -c $(CFLAGS)

$(OBJ)/notes.o: $(LIB)/notes.c Makefile
	mkdir -p $(OBJ)/
	gcc $(LIB)/notes.c -o $(OBJ)/notes.o -O2 -c $(CFLAGS)

debug: $(SRC)/simple_osc.c $(LIB)/data.c $(LIB)/shell_ui.c $(LIB)/synth.c $(LIB)/controller.c $(LIB)/notes.c Makefile
	mkdir -p $(PREFIX)
	gcc $(SRC)/simple_osc.c $(LIB)/shell_ui.c $(LIB)/synth.c $(LIB)/data.c $(LIB)/controller.c $(LIB)/notes.c \
	-o $(PREFIX)/simple_osc_db $(DFLAGS) 

clean:
	rm -fr $(OBJ)

clean-all:
	rm -fr $(PREFIX)
