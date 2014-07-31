MikeSimpleOsc
=============

* Single oscillator synth with simple waveforms for JACK (Jack Audio Connection Kit) 

* This is a single-oscillator synthesizer with simple waveforms (sine, square, sawtooth, triangle), you can select the waveform, amplitude, e precision of the fourier expansion with the shell interface.

Compilation
-----------

* In order to get this software working, you need a JACK (Jack Audio Connection Kit) Server, you can install by repository. Official site: http://jackaudio.org .

* There's no a configure file, just a simple Makefile, with -ljack -ldl -lrt -lm -lpthread flags.. if you can't compile try installing these libraries (dev headers).

Usage
-----

* You only have a main binary. Start it. Now you can use a jack frontend to connect the ports (you have a mono audio out and a midi input).

* If your midi controller work with ALSA sequencer instead of JACK midi driver, no trouble... you can use 'a2j -e' to convert ALSA ports in MIDI ports and viceversa.

* About the shell... the client needs a name, you can provide in command line or the client will ask you on start... after that, a simple shell will appear.

        $ simple_osc osc1

        Sample Rate = 44100/sec
        Hi! if you need help, type h
        To close the synth, type q or ^D
        osc1: h

        h -> help
        c -> change channel 
        W -> change waveform
        A -> set amplitude
        a -> set attack time and attack amplitude peak
        d -> set decay time
        s -> set sustain
        r -> set release time 
        i -> set number of harmonics

        The number of harmonics is related to the precision of the waveform... 
        more is higher, and more cpu power is needed... you can set at around 20 
        or higher if you want (will your ear notice the difference?).

This is free software... feel free to change and redistribute, under the same license (GPLv3). 

Is all for now... enjoy and good luck... remember this is a experimental project, if you have some question, contact me at mikefender@cryptolab.net, if you wanna contribute, feel free to do it.
