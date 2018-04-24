CC = g++ -std=c++11 -g

hdflibs = -L/hbeta/harold/lhome/external/hdf-4.2.13/lib -lmfhdf -ldf -ljpeg -lz -lsz
hdfinc = -I/hbeta/harold/lhome/external/hdf-4.2.13/include 

OBJS = main.o  modis_hdf.o modis_resamp.o

modis_resamp : ${OBJS}
	${CC} ${OBJS} ${hdflibs} -o modis_resamp

.cpp.o :
	${CC} ${hdfinc} -c $*.cpp
