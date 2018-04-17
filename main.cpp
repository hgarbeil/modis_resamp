#include <iostream>
#include <fstream>
#include "mfhdf.h"

int main (int argc, char *argv[]){

	char infile [240], name[240], attrb_name[80] ;
	int i ;
	int32 status, sd_id, sds_id, n_fileattrs, n_datasets ;
	int32 attnum, sd_therm_bands, num_type=0 ;
	int32 data_type, n_values, rank=0, dim_sizes[5], attributes=0 ;
	float *rad_scales_emiss ;


	// ensure input file can be read
	strcpy (infile, "/home/harold/data/hawaii/MOD021KM.A2014359.2035.061.2017317151219.hdf") ;
	sd_id = SDstart (infile, DFACC_RDONLY) ;
	if (sd_id <= 0) {
		printf ("Problem with %s\r\n", infile) ;
		exit(-1) ;
	}
	printf ("Status is %d\r\n", sd_id) ;
	
	// get the number of datasets and file attributes
	status = SDfileinfo (sd_id, &n_datasets, &n_fileattrs) ;
	printf ("Number of datasets is : %d\r\n",n_datasets) ;
	printf ("Number of fileattrs is : %d\r\n",n_fileattrs) ;

	for (i=0; i<n_datasets; i++) {
		sds_id = SDselect(sd_id, i) ;
		status = SDgetinfo (sds_id, name, &rank, dim_sizes, &num_type, 
			&attributes) ;
		if (!strcmp("EV_1KM_Emissive",name)) {
			strcpy (attrb_name, "radiance_scales") ;
			attnum = SDfindattr(sds_id, attrb_name) ;
			SDattrinfo (sds_id, attnum, attrb_name, &data_type, &n_values) ;
			rad_scales_emiss = new float [n_values] ;
			SDreadattr (sds_id, attnum, rad_scales_emiss) ;
			printf ("scale is : %f\r\n",rad_scales_emiss) ;
		}


		printf ("%d : %s\r\n",i, name) ;
	}







}

