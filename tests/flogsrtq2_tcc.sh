#
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of flogsrtq2"
echo ""

cnt=1
keys=400000
length=38
while [ 1 == 1 ]; do
 tcc -run words.c $keys $length > inputsrt
 echo "$keys keys for #$cnt ..."
 echo "sorting..."
 time sort inputsrt > inputsrt.srt
 echo "reverse sorting..."
 time sort -r inputsrt > inputsrt.rev.srt
 echo "running #$cnt ..."
 tcc -run -I.. flogsrtq2.c $keys inputsrt inputsrt.srt inputsrt.rev.srt
 if [ $? != 0 ]; then
  echo " failed!"
  echo "input producing the failure is left in \"inputshl\""
  exit 1
 fi
 if [ $cnt == $endhere ]; then
   echo "Finished flogsrtq2"
   echo ""
   exit 0
 fi
 echo "Passed!"

 cnt=`expr $cnt + 1`
done
