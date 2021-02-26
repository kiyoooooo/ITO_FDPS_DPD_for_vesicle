PZC_TARGET_ARCH?=sc2
LLC_OPT=-O3 -disable-pz-mul53
#LLC_OPT+= -enable-pz-mad -enable-fp-mad

MC_OPT+=-verbose-pz-hexobj

TARGET = kernel.pz

PZCSRC=../pzc/kernel.pzc

vpath %.pzc ../pzc

include /opt/pzsdk.ver4.1/make/default_pzcl_kernel.mk
