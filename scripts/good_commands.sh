#!/usr/bin/env bash


# analyse experiment results
function show_reports {
    len_of_report=
    for i in $(find ./ -name "report_epoch*" ); do 
        if [ -z $len_of_report ]; then
            len_of_report=$(cat $i | wc -l)
        fi
        tmp_d=$(mktemp -d )
        tail -n+2 ./$i > $tmp_d/tail_file
        yes "$i" | head -n $(($len_of_report - 1)) > $tmp_d/yes_file
        paste $tmp_d/tail_file $tmp_d/yes_file 
    done | sort -r -k 4 | head -n 20
}
