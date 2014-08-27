
if [ $# -lt 2 ]; then
	echo "No Arguments"
	exit 1
else 
	echo start
	out_file="${1%.*}_count_gt_$2.res"
	out_meta_file="${1%.*}_count_gt_$2.meta"
	awk -F '>' -v count=$2 '{ 
		if ($NF > count) 
			print $0;}' $1 | awk -F '>' '{ print $(NF-1), $0}' | sort -n -k1 > $out_file
	cut -d ' ' -f 1 $out_file | uniq -c > $out_meta_file
fi
