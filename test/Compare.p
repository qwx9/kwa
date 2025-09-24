#!/bin/rc
rfork e
oldawk=awk
awk=kwa

for(i in p.*){
	echo -n $i:
	$oldawk -f $i test.countries test.countries >foo1 
	$awk -f $i test.countries test.countries >foo2 
	if(cmp -s foo1 foo2)
		echo
	if not
		echo BAD
	diff -b foo1 foo2 | sed 's/^/	/;10q'
}
