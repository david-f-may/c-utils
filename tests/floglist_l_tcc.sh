#
#
endhere=100

if [ "$1" != "" ]; then
  endhere="$1"
fi

echo "Doing $endhere iterations of floglist_l"
echo ""

cnt=1
keys=500000
length=38
while [ 1 == 1 ]; do
	echo "$keys keys for #$cnt ..."
	tcc -run words.c $keys $length > inputlst
	echo "running #$cnt ..."
	tcc -I.. -I.. -run floglist_l.c inputlst
	if [ $? != 0 ]; then
		echo " failed!"
		echo "inputlst producing the failure is left in \"inputlst\""
		exit 1
	fi
  if [ $cnt == $endhere ]; then
    echo ""
    echo "Finished floglist_l"
    exit 0
  fi
	echo "Passed!"

	cnt=`expr $cnt + 1`
done
