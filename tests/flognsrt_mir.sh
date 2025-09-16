#
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of flognsrt"
echo ""

cnt=1
keys=50000
length=38
while [ 1 == 1 ]; do
 echo "$keys keys for #$cnt ..."
 c2m words.c -eg $keys $length > input
 # Uncomment the following if you want to test dups.
 echo "making dups..."
 c2m mkdups.c -ei input input.1 37
 c2m mkdups.c -ei input.1 input 36
 c2m mkdups.c -ei input input.1 35
 c2m mkdups.c -ei input.1 input 34
 c2m mkdups.c -ei input input.1 33
 c2m mkdups.c -ei input.1 input 32
 c2m mkdups.c -ei input input.1 31
 c2m mkdups.c -ei input.1 input 30
 c2m mkdups.c -ei input input.1 29
 c2m mkdups.c -ei input.1 input 28
 c2m mkdups.c -ei input input.1 27
 c2m mkdups.c -ei input.1 input 26
 c2m mkdups.c -ei input input.1 25
 c2m mkdups.c -ei input.1 input 24
 c2m mkdups.c -ei input input.1 23
 c2m mkdups.c -ei input.1 input 22
 c2m mkdups.c -ei input input.1 21
 c2m mkdups.c -ei input.1 input 20
 c2m mkdups.c -ei input input.1 19
 c2m mkdups.c -ei input.1 input 18
 c2m mkdups.c -ei input input.1 17
 c2m mkdups.c -ei input.1 input 16
 c2m mkdups.c -ei input input.1 15
 c2m mkdups.c -ei input.1 input 14
 c2m mkdups.c -ei input input.1 13
 c2m mkdups.c -ei input.1 input 12
 c2m mkdups.c -ei input input.1 11
 c2m mkdups.c -ei input.1 input 10
 c2m mkdups.c -ei input input.1 9
 c2m mkdups.c -ei input.1 input 8
 c2m mkdups.c -ei input input.1 7
 c2m mkdups.c -ei input.1 input 6
 c2m mkdups.c -ei input input.1 5
 c2m mkdups.c -ei input.1 input 4
 c2m mkdups.c -ei input input.1 3
 c2m mkdups.c -ei input.1 input 2
 rm -f input.1
 numitems=`wc -l input | awk '{print $1}'`
 c2m -I.. -I../hdrlibs rough_sort.c -eg $numitems $length input > inputsrt
 echo "sorting..."
 time sort input > inputsrt.srt
 echo "reverse sorting..."
 time sort -r input > inputsrt.rev.srt
 echo "running #$cnt ..."
 c2m -I.. -I../hdrlibs flognsrt.c -eg  inputsrt inputsrt.srt inputsrt.rev.srt
 if [ $? != 0 ]; then
  echo " failed!"
  echo "inputsrt producing the failure is left in \"inputsrt\""
  exit 1
 fi
 if [ $cnt == $endhere ]; then
   echo "Finished flognsrt"
   echo ""
   exit 0
 fi
 sleep 1
 echo "Passed!"

 cnt=`expr $cnt + 1`
done
