#

cnt=1
keys=50
len=25
incr=50
endhere=100
if [ "$1" != "" ]; then
	endhere="$1"
fi

while [ 1  = 1 ]; do
	echo "Running #$cnt"
	echo "$keys keys"
	#tcc -I.. -I../hdrlibs -run words.c $keys $len > input
	./words $keys $len > input
	echo ""
	echo "--------------------------------------------------"
	tcc -I.. -I../hdrlibs -run flogsrtsm2.c $keys input
	if [ $? != 0 ]; then
		echo " failed!"
		exit 1
	fi
	if [ $cnt == $endhere ]; then
		echo "Finished flogadd1."
		exit 0
	fi
	keys=`expr $keys + $incr`
	echo "--------------------------------------------------"
	echo ""
	echo "Passed"
	echo ""
    sleep 1

	cnt=`expr $cnt + 1`
done
