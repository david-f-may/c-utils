cnt=1
keys=500000
length=38
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

while [ $cnt -le $endhere ]; do
 echo "$keys keys for #$cnt ..."
 c2m words.c -eg $keys $length > input
 echo "sorting..."
 time sort input > input.srt
 echo "reverse sorting..."
 time sort -r input > input.rev.srt
 echo "running #$cnt ..."
 c2m -I.. fsort2.c -eg  $keys $length input input.srt input.rev.srt
 if [ $? != 0 ]; then
  echo " failed!"
  echo "inputshl producing the failure is left in \"input\""
  exit 1
 fi
 echo "Passed!"

 cnt=`expr $cnt + 1`
done
