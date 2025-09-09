#

cnt=1
keys=100
len=25
incr=100
endhere=200
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
	./flogsrtsm $keys inputshl
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
    sleep 5

	cnt=`expr $cnt + 1`
done
