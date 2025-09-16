#
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of flogsrtsys"
echo ""

cnt=1
keys=400000
lengths=38
while [ 1 == 1 ]; do
 echo "$keys keys for #$cnt ..."
 tcc -run words.c $keys $lengths > inputshl
 echo "sorting..."
 time sort inputshl > inputshl.srt
 echo "running #$cnt ..."
 tcc -I.. -I../hdrlibs -run flogsrtsys.c inputshl $keys
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
