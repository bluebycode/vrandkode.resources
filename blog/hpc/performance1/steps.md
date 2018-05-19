

# TLP

cd 3tlp
make clean all
cd ..
likwid-perfctr -f -C S0:0-3 -g FLOPS_DP ./3tlp test45M_200chr.fa
