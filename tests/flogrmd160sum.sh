#
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of flogmd5sum"
echo ""

cnt=0
runs=100
keys=25000
lengths=120
endhere=200
rm -f input
while [ $runs -gt $cnt ]; do
 #echo "$keys keys for #$cnt ..."
 c2m words.c -eg $keys $lengths > input
 run1=$(../rmd160sum input|awk '{print $1}')
 run2=$(../myopenssl rmd160 input|awk '{print $2}')
 if [ "$run1" == "$run2" ]
 then
     echo "run1=$run1; run2=$run2; passed"
 else
     echo "run1=$run1; run2=$run2; run1 != run2: failed"
     exit -1
 fi
 if [ $cnt == $endhere ]; then
   echo "Everything finished successfully with flogmd5sum"
   echo ""
   exit 0
 fi

 cnt=`expr $cnt + 1`
done
