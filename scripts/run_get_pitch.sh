#!/bin/bash

# Put here the program (maybe with path)
GETF0="get_pitch"

for fwav in /home/albert/Documents/uni/3/3B/PAV/lab/Practica3/P3/pitch_db/train/*.wav; do
    ff0=${fwav/.wav/.f0}
    echo "$GETF0 $fwav ----"
    $GETF0 $fwav $ff0 0.82 0.36 2100 1 1 1 3 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
done

exit 0
