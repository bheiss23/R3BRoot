#!/bin/bash

for DISTANCE in 1400 3500; do
	for NDOUBLEPLANES in $(seq 0 50); do
		NPLANES=$((${NDOUBLEPLANES}*2));
		root -l -q ${VMCWORKDIR}'/macros/r3b/geo/create_neuland_demo_geo.C('${NPLANES}', '${DISTANCE}', "v2_'${DISTANCE}'cm_'${NDOUBLEPLANES}'dp")';
	done
done
