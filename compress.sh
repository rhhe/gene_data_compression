#file name : compress.sh
#!/bin/bash
fn="./temptemp_file"
touch $fn
echo $1 > $fn
echo $2 >> $fn
./compress.out < $fn
rm $fn
