#!/bin/rc
rfork e
oldawk=trueawk
awk=kwa

for(i in t.*){
	echo -n $i:
	$oldawk -f $i test.data >foo1.$i
	$awk -f $i test.data >foo2.$i
	if(cmp -s foo1.$i foo2.$i)
		echo
	if not
		echo BAD
	diff -b foo1.$i foo2.$i | sed 's/^/	/;10q'
}
