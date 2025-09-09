#
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of flogcmp"
echo ""

cnt=1
keys=500000
length=38
while [ 1 == 1 ]; do
 echo "$keys keys for #$cnt ..."
 tcc -run words.c $keys $length > input
 # Comment the following and uncomment the cp if you want to test
 # nsort on raw data.
 echo "Running rough_sort.c..."
 tcc -run -I.. rough_sort.c $keys $length input > inputsrt
# cp input inputsrt
 echo "sorting..."
 time sort input > inputsrt.srt
 echo "running #$cnt ..."
 tcc -run -I.. flogcmp.c inputsrt inputsrt.srt
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
