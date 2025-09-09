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
 tcc -run words.c $keys $length > input
 tcc -I../include -I../lib -run rough_sort.c 400000 38 input > inputsrt.srt
 echo "running #$cnt ..."
 tcc -I../include -I../lib -L../lib "-run -lnsort" flogthrd.c inputsrt.srt
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
