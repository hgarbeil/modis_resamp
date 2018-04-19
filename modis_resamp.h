#include "modis_hdf.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class modis_resamp {

	int nsamps, nlines ;
	float startlat, startlon ;
	vector <string> bnames = {"band6","band21","band22","band32","solar_azim",
		"solar zenith", "sat azim", "sat azim"}


	public :
		modis_resamp () ;
		void set_modis_hdfs (modis_hdf *geom, modis_hdf *therm) ;
		void set_bounds (float ulc_lat, float ulc_lon, float llc_lat, 
			float llc_lon, float gridspace) ;


} ;
