#!/bin/bash
#PJM -L "rscunit=ito-a"            
#PJM -L "rscgrp=ito-l-dbg"
#PJM -L "vnode=32"
#PJM -L "vnode-core=36"
#PJM -L "elapse=59:00"
#PJM -j

module load intel/2019.4
#export OMP_NUM_THREADS=2
#LD_LIBRARY_PATH=/home/app/intel/intel2019_up4/compilers_and_libraries_2019.4.243/linux/compiler/lib/intel64_lin 

NUM_NODES=${PJM_VNODES}
NUM_CORES=36
NUM_PROCS=1152
#72 144 288 576 1152 
export I_MPI_PERHOST=$NUM_CORES
export I_MPI_FABRICS=shm:ofi

export I_MPI_HYDRA_BOOTSTRAP=rsh
export I_MPI_HYDRA_BOOTSTRAP_EXEC=/bin/pjrsh
export I_MPI_HYDRA_HOST_FILE=${PJM_O_NODEINF}

#mpiexec.hydra -n $NUM_PROCS ./mpiomp.out -s 20 -d 1000
#mpiexec.hydra -n $NUM_PROCS ./mpiomp.out -x ../input/0000000.cdv -v ../input/0000000.vel -s 20 -d 1000
mpiexec.hydra -n $NUM_PROCS ./mpiomp_exp_vesicle4.out -x ../exp_vesicle4/initial_pos_lipid.cdv -v ../exp_vesicle4/initial_vel_lipid.cdv -b ../exp_vesicle4/bond_info.cdv -a ../exp_vesicle4/angle_info.cdv -s 26000 -d 100 -o result_exp_vesicle4
