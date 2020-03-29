#!/bin/bash

# Put here the program (maybe with path)
GETF0="get_pitch"

for th1 in $(seq 0.6 0.02 0.98); do
    for th2 in $(seq 0.34 0.02 0.7); do
        for zeros in 1900 2000 2100 2200;do
            for fwav in /home/albert/Documents/uni/3/3B/PAV/lab/Practica3/P3/pitch_db/train/*.wav; do
                ff0=${fwav/.wav/.f0}
                $GETF0 $fwav $ff0 $th1 $th2 $zeros 0 0 0 0  || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
    
                
            done
            echo "th1 = " $th1 " th2 = " $th2 " zeros = " $zeros
            echo "th1 = " $th1 " th2 = " $th2 " zeros = " $zeros >> results.txt 
            pitch_evaluate /home/albert/Documents/uni/3/3B/PAV/lab/Practica3/P3/pitch_db/train/*.f0ref
        done
    done
done

for cl in $(seq 0.7 0.02 1); do
    for th1 in $(seq 0.6 0.02 0.98); do
        for th2 in $(seq 0.34 0.02 0.7); do
            for zeros in 1900 2000 2100 2200;do
                for fwav in /home/albert/Documents/uni/3/3B/PAV/lab/Practica3/P3/pitch_db/train/*.wav; do
                    ff0=${fwav/.wav/.f0}
                    $GETF0 $fwav $ff0 $th1 $th2 $zeros 1 $cl 0 0  || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
        
                    
                done
                echo "th1 = " $th1 " th2 = " $th2 " zeros = " $zeros "cl = " $cl
                echo "th1 = " $th1 " th2 = " $th2 " zeros = " $zeros "cl = " $cl >> results.txt 
                pitch_evaluate /home/albert/Documents/uni/3/3B/PAV/lab/Practica3/P3/pitch_db/train/*.f0ref
            done
        done
    done
done

for mf in 3 5; do
    for th1 in $(seq 0.6 0.02 0.98); do
        for th2 in $(seq 0.34 0.02 0.7); do
            for zeros in 1900 2000 2100 2200;do
                for fwav in /home/albert/Documents/uni/3/3B/PAV/lab/Practica3/P3/pitch_db/train/*.wav; do
                    ff0=${fwav/.wav/.f0}
                    $GETF0 $fwav $ff0 $th1 $th2 $zeros 0 0 1 $mf  || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
        
                    
                done
                echo "th1 = " $th1 " th2 = " $th2 " zeros = " $zeros "mf = " $mf
                echo "th1 = " $th1 " th2 = " $th2 " zeros = " $zeros "mf = " $mf >> results.txt
                pitch_evaluate /home/albert/Documents/uni/3/3B/PAV/lab/Practica3/P3/pitch_db/train/*.f0ref
            done
        done
    done
done

for mf in 3 5; do
    for cl in $(seq 0.7 0.02 1); do
        for th1 in $(seq 0.6 0.02 0.98); do
            for th2 in $(seq 0.34 0.02 0.7); do
                for zeros in 1900 2000 2100 2200;do
                    for fwav in /home/albert/Documents/uni/3/3B/PAV/lab/Practica3/P3/pitch_db/train/*.wav; do
                        ff0=${fwav/.wav/.f0}
                        $GETF0 $fwav $ff0 $th1 $th2 $zeros 1 $cl 1 $mf  || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
            
                        
                    done
                    echo "th1 = " $th1 " th2 = " $th2 " zeros = " $zeros "cl = " $cl "mf = " $mf
                    echo "th1 = " $th1 " th2 = " $th2 " zeros = " $zeros "cl = " $cl "mf = " $mf >> results.txt
                    pitch_evaluate /home/albert/Documents/uni/3/3B/PAV/lab/Practica3/P3/pitch_db/train/*.f0ref
                done
            done
        done
    done
done



exit 0