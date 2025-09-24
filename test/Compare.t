#!/bin/rc
rfork e
oldawk=awk
awk=kwa

for(i in t.*){
	echo -n $i:
	$oldawk -f $i test.data >foo1 
	$awk -f $i test.data >foo2 
	if(cmp -s foo1 foo2)
		echo
	if not
		echo BAD
	diff -b foo1 foo2 | sed 's/^/	/;10q'
}
