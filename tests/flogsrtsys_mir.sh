#
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of flogsrtsys"
echo ""

cnt=1
keys=500000
lengths=38
while [ 1 == 1 ]; do
 echo "$keys keys for #$cnt ..."
 c2m words.c -ei $keys $lengths > inputshl
 echo "sorting..."
 sort inputshl > inputshl.srt
 echo "running #$cnt ..."
 c2m -I.. -I../hdrlibs flogsrtsys.c -ei inputshl $keys
 if [ $? != 0 ]; then
  echo " failed!"
  echo "inputshl producing the failure is left in \"inputshl\""
  exit 1
 fi
 if [ $cnt == $endhere ];then
   echo "Finished flogsrtsys"
   echo ""
   exit 0
 fi
 echo "Passed!"

 cnt=`expr $cnt + 1`
done
