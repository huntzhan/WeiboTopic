#!/bin/bash

done_tables=$(grep "ends:" run.log | cut -d " " -f 4)
echo $done_tables > done.log
# declare -a done_tables

# nline=0
# while read line; do
# 	# done_tables=("${done_tables[@]}" "$line")
# 	done_tables[$nline]=$line
# 	nline=$(($nline+1))
# done < $file
# echo "done table: " $nline

sql="show tables;"
declare -a undone_tables
n=0
for table in $(mysql -u root -p123456 "filter_ref_zombie_source_bayes" -e "$sql" | grep "FilteredMicroblog"); do
	flag=0
	for t in ${done_tables[@]}; do
		if [ $table == $t ]; then
			flag=1
			break
		fi
	done
	if [ $flag == "0" ]; then
		# undone_tables=("${undone_tables[@]}" $table)
		undone_tables[$n]=$table
		n=$(($n+1))
	fi
done
echo "undone table: " ${#undone_tables[@]}
for t in ${undone_tables[@]}; do
	echo $t
done

if [ ${#undone_tables[@]} -gt 0 ]; then
	declare sql_drop_table
	for t in ${undone_tables[@]}; do
		sql_drop_table+="DROP TABLE " 
		sql_drop_table+=$t
		sql_drop_table+=";"
	done
	echo $sql_drop_table
	mysql -u root -p123456 "filter_ref_zombie_source_bayes" -e "$sql_drop_table"
fi