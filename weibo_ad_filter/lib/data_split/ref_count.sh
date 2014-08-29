
if [ $# -lt 2 ]; then
	echo "No Arguments"
	exit 1
else 
	echo start
	declare ceil
	if [ $# -eq 3 ]; then
		ceil=$3
	else 
		ceil=9999
	fi
	out_file="${1%.*}_count_$2-$ceil.res"
	out_meta_file="${1%.*}_count_$2-$ceil.meta"
	awk -F '>' -v floor=$2 -v ceil=$ceil '{ 
		if ($NF >= floor && $NF < ceil) 
			print $0;}' $1 | awk -F '>' '{ print $(NF-1), $0}' | sort -n -k1 > $out_file
	printf "%10s | %10s\n" "Ref Count" "Fingerprint" > $out_meta_file
	cut -d ' ' -f 1 $out_file | uniq -c >> $out_meta_file
fi
