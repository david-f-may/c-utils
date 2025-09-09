cnt=1
keys=500000
length=40
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

while [ $cnt -le $endhere ]; do
 echo "$keys keys for #$cnt ..."
 ./words $keys $length > input
 ./rough_sort 500000 40 input > input.srt
 echo "running #$cnt ..."
 ./floghash input.srt
 if [ $? != 0 ]; then
  echo " failed!"
  echo "inputshl producing the failure is left in \"input\""
  exit 1
 fi
 echo "Passed!"

 cnt=`expr $cnt + 1`
done
