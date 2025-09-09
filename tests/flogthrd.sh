cnt=1
keys=400000
length=38
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Testing threaded sort..."
echo ""

while [ $cnt -le $endhere ]; do
 echo "$keys keys for #$cnt ..."
 ./words $keys $length > input
 ./rough_sort 400000 38 input > inputsrt.srt
 echo "running #$cnt ..."
 ./flogthrd inputsrt.srt
 sort input > inputsrt.rsrt
 echo "diff between sorted output via qsort and flogthrd1...(nothing is good here)"
 echo ""
 diff inputsrt.rsrt inputsrt.out
 if [ $? != 0 ]; then
  echo " failed!"
  echo "inputsrt producing the failure is left in \"inputsrt\""
  exit 1
 fi
 rm -f inputsrt inputsrt.srt
 echo "Passed!"

 cnt=`expr $cnt + 1`
done
