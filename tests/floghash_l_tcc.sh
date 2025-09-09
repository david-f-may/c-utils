cnt=1
keys=500000
length=25
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

while [ $cnt -le $endhere ]; do
 echo "$keys keys for #$cnt ..."
 tcc -run words.c $keys $length > input
 tcc -run -I.. rough_sort.c $keys $length input > input.srt
 echo "running #$cnt ..."
 tcc -I.. -I.. -run  floghash_l.c input.srt
 if [ $? != 0 ]; then
  echo " failed!"
  echo "inputshl producing the failure is left in \"input\""
  exit 1
 fi
 echo "Passed!"

 cnt=`expr $cnt + 1`
done
