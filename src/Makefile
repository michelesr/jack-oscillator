../bin/simple_osc: ../bin/obj/shell_ui.o ../bin/obj/synth.o simple_osc.c Makefile
	mkdir -p ../bin
	gcc simple_osc.c ../bin/obj/shell_ui.o ../bin/obj/synth.o  -o ../bin/simple_osc -ljack -lpthread -ldl -lrt -lm -O2

../bin/obj/shell_ui.o: lib/shell_ui.c Makefile
	mkdir -p ../bin/obj/
	gcc lib/shell_ui.c -o ../bin/obj/shell_ui.o -O2 -c 
	
../bin/obj/synth.o: lib/synth.c Makefile
	mkdir -p ../bin/obj/
	gcc lib/synth.c -o ../bin/obj/synth.o -O2 -c 

debug: simple_osc.c lib/shell_ui.c lib/synth.c Makefile 
	mkdir -p ../bin
	gcc simple_osc.c lib/shell_ui.c lib/synth.c  -o ../bin/simple_osc -ljack -lpthread -ldl -lrt -lm -g

clean:
	rm -fr ../bin
