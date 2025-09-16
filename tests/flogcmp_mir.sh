#
endhere=250

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of flogcmp"
echo ""

cnt=1
keys=500000
lengths=38
while [ 1 == 1 ]; do
 echo "$keys keys for #$cnt ..."
 c2m words.c -ei $keys $lengths > input
 # Comment the following and uncomment the cp if you want to test
 # nsort on raw data.
 c2m -I.. -I../hdrlibs rough_sort.c -eg 500000 38 input > inputsrt
# cp input inputsrt
 echo "sorting using sort..."
 time sort input > inputsrt.srt
 echo "running #$cnt ..."
 c2m -I.. -I../hdrlibs flogcmp.c -eg inputsrt inputsrt.srt
 if [ $? != 0 ]; then
  echo " failed!"
  echo "inputsrt producing the failure is left in \"inputsrt\""
  exit 1
 fi
 if [ $cnt == $endhere ]; then
   echo "Finished flogcmp"
   echo ""
   exit 0
 fi
 sleep 1
 echo "Passed!"

 cnt=`expr $cnt + 1`
done
