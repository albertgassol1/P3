#!/bin/bash

# Put here the program (maybe with path)
GETF0="get_pitch"

for th1 in $(seq 0.6 0.02 0.98); do
    for th2 in $(seq 0.3 0.02 0.84); do
        for fwav in /home/albert/Documents/uni/3/3B/PAV/lab/Practica3/P3/pitch_db/train/*.wav; do
            ff0=${fwav/.wav/.f0}
            echo "$GETF0 $fwav ----"
            echo "$th1 $th2"
            $GETF0 $fwav $ff0 $th1 $th2 1700 1 0.7 0 0 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
        done
        pitch_evaluate pitch_db/train/*f0ref
    done
done
exit 0