#

cnt=1
keys=50
len=25
incr=50
endhere=500
if [ "$1" != "" ]; then
	endhere="$1"
fi

while [ 1  = 1 ]; do
	echo "Running #$cnt"
	echo "$keys keys"
    date
	./words $keys $len > inputshl
	echo ""
	echo "--------------------------------------------------"
	./flogsrtsm2 $keys inputshl
	if [ $? != 0 ]; then
		echo " failed!"
		exit 1
	fi
	if [ $cnt == $endhere ]; then
		echo "Finished flogsrtsm."
		exit 0
	fi
	keys=`expr $keys + $incr`
	echo "--------------------------------------------------"
	echo ""
	echo "Passed"
	echo ""

	cnt=`expr $cnt + 1`
done
