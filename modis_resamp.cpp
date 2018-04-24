#include "modis_resamp.h"
#include <math.h>



modis_resamp::modis_resamp () {

	distarr = 0L ;
	bandsarr = 0L ;
	nl = 0 ;
	ns = 0 ;
	gridspace = 0.1 ;


}

modis_resamp::~modis_resamp () {

	delete [] bandsarr ;
	delete [] distarr ;
}

void modis_resamp::set_modis_hdfs (modis_hdf *g, modis_hdf *th) {
	geom = g ;
	therm = th ;
}

void modis_resamp::set_bounds  (float ulc_lat, float ulc_lon, float lrc_lat, 
        float lrc_lon, float gspace) {

		int i ;
		

		gridspace = gspace ; 

		nl = int (((ulc_lat - lrc_lat) / gridspace )+ 1) ;
		ns = int (((lrc_lon - ulc_lon) / gridspace )+ 1) ;
		startlat = ulc_lat ;
		startlon = ulc_lon ;
		endlat = lrc_lat ;
		endlon = lrc_lon ;

		cout << "Number of lines is : " << nl << endl ;
		cout << "Number of samples is : " << ns << endl ;

		if (distarr) {
			delete [] distarr ;
			delete [] bandsarr ;
		}
		distarr = new float [ns * nl] ;
		bandsarr = new float [8 * ns * nl] ;
		for (i=0; i<ns * nl; i++) {
			distarr [i] = 9999. ;
		}
		for (i=0; i<ns * nl * 8; i++) {
			bandsarr [i] = -1. ;
		}

}

void modis_resamp::process () {

	int i, j, ib, snum, is, js, ixloc, iyloc, grid_x, grid_y, gridloc,npix ;
	float xloc, yloc, xdist, ydist, dist, latval, lonval ;
	npix = nl * ns ;

	for (i=0; i<2030; i++) {
		for (j=0; j<1354; j++) {
			snum = i*1354+j ;
			latval = geom->latarr[snum] ;
			lonval = geom->lonarr[snum] ;
			if (latval > startlat || latval < endlat) 
				continue ;
			if (lonval < startlon || lonval > endlon) 
				continue ;

			xloc = (lonval - startlon) / gridspace ;
			yloc = (startlat - latval) / gridspace ;
			ixloc = int(xloc+0.5) ;
			iyloc = int(yloc+0.5) ;

			for (is=-2; is<=2; is++) {
				
				grid_y = iyloc + is ;
				if (grid_y < 0) continue ;
				if (grid_y >= nl) continue ;

				ydist = yloc - grid_y ;
				for (js=-2; js<=2; js++) {
					grid_x = is + ixloc ;
					if (grid_x < 0) continue ;
					if (grid_x >= ns) continue ;
					gridloc = grid_y * ns + grid_x ;
					xdist = xloc - grid_x ;
					dist = sqrt (xdist * xdist + ydist * ydist) ;
					if (dist < distarr[grid_y * ns + grid_x])
					{
						distarr[iyloc*ns+ixloc]= dist ;
						bandsarr[gridloc] = therm->refdata_cal[i*ns+j] ;
						for (ib=0; ib<3; ib++) 
						bandsarr[(ib+1) * npix + gridloc] = therm->raddata_cal[ib*2030*1354+snum] ;
						bandsarr[4*npix+gridloc] = geom->solsens[snum] ; 
						bandsarr[5*npix+gridloc] = geom->solsens[2030L*1354+snum] ;
						bandsarr[6*npix+gridloc] = geom->solsens[2*2030L*1354+snum] ;
						bandsarr[7*npix+gridloc] = geom->solsens[3*2030L*1354+snum] ;
					}
				}
			}
		}
	}
}


void modis_resamp::write_output (char *outfile) {
	char hdrfile [420] ;
	int i, npix ;
	npix = ns * nl ;
	cout << "Number of samples is : " << ns << endl ;
	cout << "Number of lines is : " << nl << endl ;

	strcpy (hdrfile, outfile) ;
	strcat (hdrfile, ".hdr") ;
	write_header (hdrfile) ;

	FILE *fout = fopen (outfile, "w") ;
	if (fout == NULL) {
		cout << "Could not open " << outfile << endl ;
		return ;
	}
	for (i=0; i<8; i++) {
		fwrite ((char *) &bandsarr[i * npix], 4, npix, fout) ;
	}
	fclose (fout) ;

}


void modis_resamp::write_header (char *outfile)
{

	FILE *hdrout = fopen (outfile, "w") ;
	char bnames [1200] ;
	strcpy (bnames, "band names = {\nModisBand6,ModisBand21,ModisBand22,ModisBand32,\nSolarAzimuth,SolarZenith,SensorAzimuth,SensorZenith}\n") ;

    fprintf (hdrout, "ENVI\ndescription = {\nMOD021KM - MOD03  }\n") ;
    fprintf (hdrout, "samples    = %5d\n",ns) ;
	fprintf (hdrout, "lines      = %5d\n",nl) ;
	fprintf (hdrout, "bands      = 8\n") ;
	fprintf (hdrout, bnames) ;
	fprintf (hdrout, "header offset = 0 \n") ;
	fprintf (hdrout, "file type = ENVI Standard \n") ;
	fprintf (hdrout, "data type = 4 \n") ;
	fprintf (hdrout, "interleave = bsq \n") ;
	fprintf (hdrout, "sensor type = MODIS \n") ;
	fprintf (hdrout, "byte order = 0\n") ;
	fprintf (hdrout, "map info = {Geographic Lat/Lon, 1.0000, 1.0000, %12.8f, %12.8f,",
	         startlon, startlat) ;
	fprintf (hdrout, "%6.4f, %6.4f, WGS-84, units=Degrees}\n", gridspace, gridspace) ;

	fclose (hdrout) ;
}


