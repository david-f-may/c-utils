#
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of flogsrt"
echo ""

cnt=1
keys=1000000
length=38
while [ 1 == 1 ]; do
 echo "$keys keys for #$cnt ..."
 c2m words.c -eg $keys $length > input
 # Comment the following and uncomment the cp if you want to test
 # nsort on raw data.
 c2m -I.. -I../hdrlibs rough_sort.c -eg $keys $length input > inputsrt
# cp input inputsrt
 echo "sorting..."
 time sort input |uniq >  inputsrt.srt
 echo "reverse sorting..."
 time sort -r input |uniq > inputsrt.rev.srt
 echo "running #$cnt ..."
 c2m -I.. -I../hdrlibs flogsrt.c -eg inputsrt inputsrt.srt inputsrt.rev.srt
 if [ $? != 0 ]; then
  echo " failed!"
  echo "inputsrt producing the failure is left in \"inputsrt\""
  exit 1
 fi
 if [ $cnt == $endhere ]; then
   echo "Finished flogsrt"
   echo ""
   exit 0
 fi
 sleep 1
 echo "Passed!"

 cnt=`expr $cnt + 1`
done
