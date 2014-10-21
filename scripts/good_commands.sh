#!/usr/bin/env bash


# analyse experiment results
function show_reports {
    path_to_show=./
    if [ $# -gt 0 ]; then
        path_to_show=
        for var in "$@"; do
            path_to_show="$path_to_show $var"
        done
    fi        
    len_of_report=
    tmp_d=$(mktemp -d)
    for i in $(find $path_to_show -name "report_epoch*"); do 
        if [ -z $len_of_report ]; then
            len_of_report=$(cat $i | wc -l)
        fi
        bn=$(basename $i)
        tail -n+2 ./$i > $tmp_d/tail_file_$bn
        yes "$i" | head -n $(($len_of_report - 1)) > $tmp_d/yes_file_$bn
        paste $tmp_d/tail_file_$bn $tmp_d/yes_file_$bn
    done | sort -r -k 4 | head -n 20
}
