#
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of flogsrt_l"
echo ""

cnt=1
keys=400000
length=38
while [ 1 == 1 ]; do
 echo "$keys keys for #$cnt ..."
 tcc -run -I.. words.c $keys $length > input
 # Comment the following and uncomment the cp if you want to test
 # nsort on raw data.
 tcc -run -I.. rough_sort.c $keys $length input > inputsrt
 echo "sorting..."
 time sort input |uniq >  inputsrt.srt
 echo "reverse sorting..."
 time sort -r input |uniq > inputsrt.rev.srt
 echo "running #$cnt ..."
 tcc -run -I.. flogsrt_l.c inputsrt inputsrt.srt inputsrt.rev.srt
 if [ $? != 0 ]; then
  echo " failed!"
  echo "inputsrt producing the failure is left in \"inputsrt\""
  exit 1
 fi
 if [ $cnt == $endhere ]; then
   echo "Finished flogsrt_l"
   echo ""
   exit 0
 fi
 sleep 1
 echo "Passed!"

 cnt=`expr $cnt + 1`
done
