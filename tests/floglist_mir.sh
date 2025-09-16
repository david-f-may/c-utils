#
endhere=500
if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of floglist"
echo ""

cnt=1
keys=500000
length=37
while [ 1 == 1 ]; do
	echo "$keys keys for #$cnt ..."
    c2m words.c -eg $keys $length > inputlst
	echo "running #$cnt ..."
	c2m -I.. -I../hdrlibs floglist.c -eg inputlst
	if [ $? != 0 ]; then
		echo " failed!"
		echo "inputlst producing the failure is left in \"inputlst\""
		exit 1
	fi
  if [ $cnt == $endhere ]; then
    echo ""
    echo "Finished floglist"
    exit 0
  fi
	echo "Passed!"

	cnt=`expr $cnt + 1`
done
