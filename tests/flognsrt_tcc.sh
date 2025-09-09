#
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of flognsrt"
echo ""

cnt=1
keys=25000
length=35
while [ 1 == 1 ]; do
 echo "$keys keys for #$cnt ..."
 tcc -run words.c $keys $length > input
 # Uncomment the following if you want to test dups.
 echo "making dups..."
 tcc -run mkdups.c input input.1 37
 tcc -run mkdups.c input.1 input 36
 tcc -run mkdups.c input input.1 35
 tcc -run mkdups.c input.1 input 34
 tcc -run mkdups.c input input.1 33
 tcc -run mkdups.c input.1 input 32
 tcc -run mkdups.c input input.1 31
 tcc -run mkdups.c input.1 input 30
 tcc -run mkdups.c input input.1 29
 tcc -run mkdups.c input.1 input 28
 tcc -run mkdups.c input input.1 27
 tcc -run mkdups.c input.1 input 26
 tcc -run mkdups.c input input.1 25
 tcc -run mkdups.c input.1 input 24
 tcc -run mkdups.c input input.1 23
 tcc -run mkdups.c input.1 input 22
 tcc -run mkdups.c input input.1 21
 tcc -run mkdups.c input.1 input 20
 tcc -run mkdups.c input input.1 19
 tcc -run mkdups.c input.1 input 18
 tcc -run mkdups.c input input.1 17
 tcc -run mkdups.c input.1 input 16
 tcc -run mkdups.c input input.1 15
 tcc -run mkdups.c input.1 input 14
 tcc -run mkdups.c input input.1 13
 tcc -run mkdups.c input.1 input 12
 tcc -run mkdups.c input input.1 11
 tcc -run mkdups.c input.1 input 10
 tcc -run mkdups.c input input.1 9
 tcc -run mkdups.c input.1 input 8
 tcc -run mkdups.c input input.1 7
 tcc -run mkdups.c input.1 input 6
 tcc -run mkdups.c input input.1 5
 tcc -run mkdups.c input.1 input 4
 tcc -run mkdups.c input input.1 3
 tcc -run mkdups.c input.1 input 2
 rm -f input.1
 numitems=`wc -l input | awk '{print $1}'`
 tcc -run -I.. rough_sort.c $numitems $length input > inputsrt
 echo "sorting..."
 time sort input > inputsrt.srt
 echo "reverse sorting..."
 time sort -r input > inputsrt.rev.srt
 echo "running #$cnt ..."
 tcc -run -I.. flognsrt.c inputsrt inputsrt.srt inputsrt.rev.srt
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
