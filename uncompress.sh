#file name : uncompress.sh
#!/bin/bash
fn="./temptemptemp_file"
touch $fn
echo $1 > $fn
echo $2 >> $fn
./uncompress.out < $fn
rm $fn
