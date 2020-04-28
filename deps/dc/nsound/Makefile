# KallistiOS ##version##
#
# libmp3/Makefile
# (c)2001 Dan Potter
#

NSND_INC_DIR = ./include
override CFLAGS := -I$(NSND_INC_DIR) $(CFLAGS)

TARGET = libNSound.a
OBJS = nsnd_iface.o nsnd_sfxmgr.o nsnd_stream.o nsnd_mem.o nsnd_stream_drv.o

# Only compile this if we have an ARM compiler handy
ifdef DC_ARM_CC
	SUBDIRS = arm
endif

include $(KOS_BASE)/Makefile.prefab

print-%  : ; @echo $* = $($*)

#printallvars: $(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))

nsnd_stream_drv.o: subdirs arm/nstream.drv
	$(KOS_BASE)/utils/bin2o/bin2o arm/nstream.drv nsnd_stream_drv nsnd_stream_drv.o

link:
	$(KOS_AR) rcs $(TARGET) $(OBJS)

build: $(OBJS) link

