#ifndef m_rs 
#define m_rs

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "modis_hdf.h"

using namespace std;

class modis_resamp {

	int nsamps, nlines, ns, nl ;
	float startlat, startlon, endlat, endlon, gspace ;
	float *distarr, *bandsarr ;
	modis_hdf *geom, *therm ;


	public :
		float gridspace ;
		modis_resamp () ;
		~modis_resamp () ;
		void write_output(char *) ;
		void set_modis_hdfs (modis_hdf *geom, modis_hdf *therm) ;
		void set_bounds (float ulc_lat, float ulc_lon, float llc_lat, 
			float llc_lon, float gridspace) ;
		void process () ;
		void write_header (char *) ;


} ;

#endif
