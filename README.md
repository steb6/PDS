Esecuzione su macchina virtuale:

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

