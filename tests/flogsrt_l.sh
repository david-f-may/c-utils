#
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of flogsrt_l"
echo ""

cnt=1
keys=400000
lengths=38
while [ 1 == 1 ]; do
 echo "$keys keys for #$cnt ..."
 ./words $keys $lengths > input
 # Comment the following and uncomment the cp if you want to test
 # nsort on raw data.
 ./rough_sort 400000 38 input > inputsrt
# cp input inputsrt
 echo "sorting..."
 time sort input |uniq >  inputsrt.srt
 echo "reverse sorting..."
 time sort -r input |uniq > inputsrt.rev.srt
 echo "running #$cnt ..."
 ./flogsrt_l inputsrt inputsrt.srt inputsrt.rev.srt
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
