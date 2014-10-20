#!/usr/bin/env bash
set -e

CWD_SCR=$(readlink -f $0)
CWD=$(dirname $CWD_SCR)
pushd $CWD &> /dev/null

. common.sh

function usage {
    echo "$0 constant_section:constant_name:min_value-max_value"
}



declare -A const_name_to_values
EXP_LEN=5

for v in $@; do 
    sect=$(echo $v | awk -F':' '{print $1}')
    name=$(echo $v | awk -F':' '{print $2}')
    min_max=$(echo $v | awk -F':' '{print $3}')
    min=$(echo $min_max | awk -F'-' '{print $1}')
    max=$(echo $min_max | awk -F'-' '{print $2}')
    [ -z $sect ] && ( echo "Can't parse constant section"; usage; exit 1 )
    [ -z $name ] && ( echo "Can't parse constant name"; usage; exit 1 )
    [ -z $min_max ] && ( echo "Can't parse constant min-max values"; usage; exit 1 )
    [ -z $min ] && ( echo "Can't parse constant min value"; usage; exit 1 )
    [ -z $max ] && ( echo "Can't parse constant max value"; usage; exit 1 )
    [ $(echo "$min>=$max" | bc) -eq 1 ] && ( echo "Min value greater or equal than max"; usage; exit 1 )
    inc=$(echo "scale=3; ($max-$min)/$EXP_LEN" | bc)
    values=$(seq $min $inc $max)
    const_name_to_values+=([$name]="$values")
done

EXP_EPOCH=20
EXP_INPUT="$HOME/prog/sim/spikes/ucr_2_classes_train.bin"
EXP_EVAL="$HOME/prog/sim/spikes/ucr_2_classes_test.bin"
WORK_DIR_BASE=$RUNS_DIR/$(get_const_for_name neuron_type)_$(get_const_for_name learning_rule)_$(get_const_for_name prob_fun)
YES_RM_ALL="no"

function ask_for_dir {
    wd=$1
}

for const_name_i in "${!const_name_to_values[@]}"; do
    for i in ${const_name_to_values[$const_name_i]}; do 
        for const_name_j in "${!const_name_to_values[@]}"; do
            [ $const_name_i == $const_name_j ] && continue
            for j in ${const_name_to_values[$const_name_j]}; do 
                echo $const_name_i:$i $const_name_j:$j
                wd=${WORK_DIR_BASE}_${const_name_i}_${i}_${const_name_j}_${j}
                RESP="@"
                if [ -d $wd ] && [ "$YES_RM_ALL" == "no" ]; then
                    while true; do
                        if [ "$RESP" == "y" ]; then
                            rm -rf $wd
                            break
                        elif [ "$RESP" == "n" ]; then
                            exit 0
                            break
                        elif [ "$RESP" == "a" ]; then
                            YES_RM_ALL="yes"
                            break
                        else
                            read -p "$(basename $wd) already exists. Delete it and run in that directory? (y=yes,n=no,a=yes to all): " RESP
                        fi                
                    done            
                fi        
                [ $YES_RM_ALL == "yes" ] && rm -rf $wd

                mkdir -p $wd
                cat ../constants.ini  | sed -e "s|\($const_name_i *= *\)[.0-9]*|\1$i|g"  \
                                            -e "s|\($const_name_j *= *\)[.0-9]*|\1$j|g"   > $wd/constants.ini
                set -x
                bash -x run_sim.sh -w $wd -a -e $EXP_EPOCH -v $EXP_EVAL $EXP_INPUT &> $wd/run_sim.${i}.${j}.output
                set +x
            done
        done                        
    done        
    break # only one 
done    


popd &> /dev/null
