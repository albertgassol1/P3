#!/bin/bash

# Put here the program (maybe with path)
GETF0="get_pitch"

for fwav in /home/albert/Documents/uni/3/3B/PAV/lab/Practica3/P3/pitch_db/train/*.wav; do
    ff0=${fwav/.wav/.f0}
    echo "$GETF0 $fwav ----"
    $GETF0 $fwav $ff0 0.95 0.65 1700 1 0.75 0 3 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
done

exit 0
