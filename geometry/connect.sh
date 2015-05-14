#!/bin/bash

#usage ./connect.sh file1 file2 file3
# file1 = file with partial design
# file2 = file with full design (we want to add the part(s) in file1)
# file3 = target file for full geometry (file1 + file2)

# at now it is adding part in solids and structure (add (or remove ) sections in the for loop for more) 
i="0"
file2=`echo $2`
for mat in "</solids>" "</structure>"
    do
    mat2=`echo $mat | tr -d "/"` 
    name=`echo $mat2 | tr -d "<" | tr -d ">"`
 
    if [ $i -gt 0 ] ; then file2=`echo $3` ; fi 
    START_SOLID=`grep -n -h $mat2 $1 | tr -d ":$mat2"`
    START_SOLID2=`grep -n -h $mat2 $file2 | tr -d ":$mat2"`
    TAIL_SOLID2=`wc -l $file2 | tr -d " $file2" `
    TAIL_SOLID2=`expr $TAIL_SOLID2 - $START_SOLID2 ` 
    END_SOLID=`grep -n -h $mat $1 | tr -d ":$mat"`
    END_SOLID=`expr $END_SOLID - 1 `
    DIFF_SOLID=`expr $END_SOLID - $START_SOLID`
    if [ $DIFF_SOLID -gt 0 ]
    then
 	head -n${END_SOLID} $1 | tail -n${DIFF_SOLID} > tmp_2_${name}.gdml
	head -n${START_SOLID2} $file2 > tmp_1_${name}.gdml
	tail -n${TAIL_SOLID2} $file2 > tmp_3_${name}.gdml
	if [ -f $3 ] ; then rm  $3 ; fi
	cat  tmp_1_${name}.gdml tmp_2_${name}.gdml tmp_3_${name}.gdml > $3
	rm tmp_1_${name}.gdml tmp_2_${name}.gdml tmp_3_${name}.gdml
	i=`expr $i + 1`
    fi
done


 
