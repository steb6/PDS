// Esecuzione su macchina virtuale:

131.114.136.132 c6320p-2.itc.unipi.it phls
i19
salvato in /etc/hosts

ssh s.berti9@phi19
inserito in .baschrc la linea
export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH
rm Draw*
g++ -Wall -g *.cpp -o demo -phtread

per fare upload dei file da locale, da dentro PDS:
rsync -azur . s.berti9@phi19:

// come calcolare media esecuzione programma
script log.txt
for((i=0; i<10; i++)); do time ./demo 1 10 1000 1; done
exit
cat log.txt | grep real | awk -Fm '{print $2}' | sed -e "s/s//g" | sed -e "s/,/./g" | awk '{sum += $1} END { cat sum/NR >> results.txt }'

sed: con -e passo il comando, "sostituisci/carattere da sostituire/con cosa sostituire/globale"
awk: -Fm dice di separare usando lo spazio

// Per attivare interfaccia grafica
define GRAPH in main.cpp
in test.sh, commentare la linea di compilazione sbagliata
