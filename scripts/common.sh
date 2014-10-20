
RUNS_DIR=~/prog/sim/runs

SNN_SIM="../build/bin/snn_sim"
SNN_POSTPROC="../build/bin/snn_postproc"

export LC_NUMERIC="en_US.UTF-8"

function get_const_for_name {
   grep -Eo "^$1.*=[ ]*[ \/_.a-zA-Z0-9]+" ../constants.ini | awk -F'=' '{ print $2}' | sed -e 's|^[ ]*||g' -e 's|[ ]*$||g'  | tr ' ' _
}
