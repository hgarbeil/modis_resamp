#include "modis_resamp.h"


modis_resamp::modis_resamp () {

	distarr = 0L ;
	bandsarr = 0L ;
	nl = 0 ;
	ns = 0 ;


}

void modis_resamp::set_bounds  (float ulc_lat, float ulc_lon, float lrc_lat, 
            float lrc_lon, float gridspace) {


		nl = int ((ulc_lat - lrc_lat) / gridspace )+ 1) ;
		ns = int ((lrc_lon - ulc_lon) / gridspace )+ 1) ;
		startlat = ulc_lat ;
		startlon = ulc_lon ;
		endlat = lrc_lat ;
		endlon = lrc_lon ;


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

	int i, j, snum, is, js, ixloc, iyloc, grid_x, grid_y, gridloc, npix ;
	float xloc, yloc, xdist, ydist, dist ;
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

			for (is=-1; is<=1; is++) {
				grid_y = iyloc + is ;
				ydist = yloc - grid_y ;
				for (js=-1; js<=1; js++) {
					grid_x = is + ixloc ;
					gridloc = grid_y * ns + grid_x ;
					xdist = xloc - grid_x ;
					dist = sqrt (xdist * xdist + ydist * ydist) ;
					if (dist < distarr[grid_y * ns + grid_x])
					{
						distarr[iyloc*ns+ixloc]= dist ;
						bandsarr[gridloc] = therm->refdata_cal[i*ns+j] ;
						for (ib=0; ib<3; ib++) 
							bandsarr[(ib+1) * npix + gridloc] = therm->raddata_cal[ib*2030*1354+snum] ;
						bandsarr[4*npix+gridloc] = geom->solaz[snum] ;
						bandsarr[5*npix+gridloc] = geom->solzen[snum] ;
						bandsarr[6*npix+gridloc] = geom->sensaz[snum] ;
						bandsarr[7*npix+gridloc] = geom->senszen[snum] ;
					}
				}
			}
		}
	}
}
