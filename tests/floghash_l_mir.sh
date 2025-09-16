cnt=1
keys=500000
length=10
endhere=250

if [ "$1" != "" ]; then
  endhere="$1"
fi

while [ $cnt -le $endhere ]; do
 echo "$keys keys for #$cnt ..."
 c2m words.c -eg $keys $length > input
 c2m -I.. -I../hdrlibs rough_sort.c -ei $keys $length input > input.srt
 echo "running #$cnt ..."
 c2m -I.. -I../hdrlibs floghash_l.c -eg input.srt
 if [ $? != 0 ]; then
  echo " failed!"
  echo "inputshl producing the failure is left in \"input\""
  exit 1
 fi
 echo "Passed!"
 echo ""

 cnt=`expr $cnt + 1`
done
