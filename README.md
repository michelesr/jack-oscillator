jack-oscillator
=============

* Single oscillator synthesizer with traditional waveforms for JACK (Jack Audio Connection Kit)

* This is a single-oscillator lightweight synthesizer, with traditional waves (sine, square, sawtooth, triangle). You can select the form, amplitude and envelope of the wave.

Compilation
-----------

* In order to get this software working, JACK Server is required. Often you can find a jack or jack2 (jackdmp) in your distribution packages. Official site: http://jackaudio.org .

* There's no a configure file, just a simple Makefile, with -ljack -ldl -lrt -lm -lpthread flags.. if you can't compile try installing these libraries (dev headers).

Usage
-----

* The program is composed of only a main binary, start it.

* Now you can use a jack frontend (such as qjackctl) to connect the ports (you have a mono audio out and a midi input).

* If your midi controllers works with ALSA sequencer instead of JACK midi driver, try using 'a2j -e' (provided by a2jmidid) to convert ALSA ports in MIDI ports and viceversa.

* The client needs a name, you can provide in command line, otherwise the client will ask you on start, and after that, a simple shell will appear.

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

* MIDI CONTROLS - Volume (Controller #7) , Program Change (MIDI Program/Bank Change)

This is free software... feel free to change and redistribute, under the same license (GPLv3).

Is all for now... enjoy and good luck... remember this is a experimental project, if you have some question, contact me at mikefender@cryptolab.net, if you wanna contribute, feel free to do it.
