#
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of flognsrt"
echo ""

cnt=1
keys=25000
lengths=38
while [ 1 == 1 ]; do
 echo "$keys keys for #$cnt ..."
 ./words $keys $lengths > input
 # Uncomment the following if you want to test dups.
 echo "making dups..."
 ./mkdups input input.1 37
 ./mkdups input.1 input 36
 ./mkdups input input.1 35
 ./mkdups input.1 input 34
 ./mkdups input input.1 33
 ./mkdups input.1 input 32
 ./mkdups input input.1 31
 ./mkdups input.1 input 30
 ./mkdups input input.1 29
 ./mkdups input.1 input 28
 ./mkdups input input.1 27
 ./mkdups input.1 input 26
 ./mkdups input input.1 25
 ./mkdups input.1 input 24
 ./mkdups input input.1 23
 ./mkdups input.1 input 22
 ./mkdups input input.1 21
 ./mkdups input.1 input 20
 ./mkdups input input.1 19
 ./mkdups input.1 input 18
 ./mkdups input input.1 17
 ./mkdups input.1 input 16
 ./mkdups input input.1 15
 ./mkdups input.1 input 14
 ./mkdups input input.1 13
 ./mkdups input.1 input 12
 ./mkdups input input.1 11
 ./mkdups input.1 input 10
 ./mkdups input input.1 9
 ./mkdups input.1 input 8
 ./mkdups input input.1 7
 ./mkdups input.1 input 6
 ./mkdups input input.1 5
 ./mkdups input.1 input 4
 ./mkdups input input.1 3
 ./mkdups input.1 input 2
 rm -f input.1
 numitems=`wc -l input | awk '{print $1}'`
 ./rough_sort $numitems 38 input > inputsrt
 echo "sorting..."
 time sort input > inputsrt.srt
 echo "reverse sorting..."
 time sort -r input > inputsrt.rev.srt
 echo "running #$cnt ..."
 ./flognsrt inputsrt inputsrt.srt inputsrt.rev.srt
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
