../bin/simple_osc: ../bin/obj/data.o ../bin/obj/shell_ui.o ../bin/obj/synth.o ../bin/obj/controller.o simple_osc.c Makefile
	mkdir -p ../bin
	gcc simple_osc.c ../bin/obj/shell_ui.o ../bin/obj/synth.o ../bin/obj/data.o ../bin/obj/controller.o -o ../bin/simple_osc -ljack -lpthread -ldl -lrt -lm -O2

../bin/obj/data.o: lib/data.c Makefile
	mkdir -p ../bin/obj/
	gcc lib/data.c -o ../bin/obj/data.o -O2 -c -ljack -lpthread -ldl -lrt -lm
	
../bin/obj/shell_ui.o: lib/shell_ui.c Makefile
	mkdir -p ../bin/obj/
	gcc lib/shell_ui.c -o ../bin/obj/shell_ui.o -O2 -c -ljack -lpthread -ldl -lrt -lm
	
../bin/obj/synth.o: lib/synth.c Makefile
	mkdir -p ../bin/obj/
	gcc lib/synth.c -o ../bin/obj/synth.o -O2 -c -ljack -lpthread -ldl -lrt -lm

../bin/obj/controller.o: lib/controller.c Makefile
	mkdir -p ../bin/obj/
	gcc lib/controller.c -o ../bin/obj/controller.o -O2 -c -ljack -lpthread -ldl -lrt -lm

debug: simple_osc.c lib/data.c lib/shell_ui.c lib/synth.c lib/controller.c Makefile 
	mkdir -p ../bin
	gcc simple_osc.c lib/shell_ui.c lib/synth.c lib/data.c lib/controller.c -o ../bin/simple_osc -ljack -lpthread -ldl -lrt -lm -g

clean:
	rm -fr ../bin
