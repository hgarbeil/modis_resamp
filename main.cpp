#include <iostream>
#include <fstream>
#include "modis_hdf.h"
#include "modis_resamp.h"

int main (int argc, char *argv[]){

	char *tmp ;
	float startx, starty, endx, endy, gspace ;
	
	modis_hdf *geom, *therm ;
	modis_resamp *mresamp ;
	// input file is the list of files to process
	char outpref[380], outfile[420], tmpfile[420], infile [420], file0[240], file1[240], sub1[24], sub2[24] ;

	if (argc < 6) {
		cout << "Useage :modis_resamp outpref infilelist startx starty endx endy gridspace" << endl ;
		exit (-1) ;
	}
	strcpy (infile, *++argv) ;
	strcpy (outpref, *++argv) ;
	startx = atof (*++argv) ;
	starty = atof (*++argv) ;
	endx = atof (*++argv) ;
	endy = atof (*++argv) ;
	gspace = atof (*++argv) ;




	mresamp = new modis_resamp() ;
	FILE *fin = fopen (infile, "r") ;
	if (fin == NULL) {
		cout << "Could not open input file" << endl ;
		exit(-1) ;
	}
	while (!feof(fin)) {
		fscanf (fin, "%s %s", file0, file1) ;
		strcpy (tmpfile, file0) ;
		tmp = strtok (tmpfile, ".") ;
		tmp = strtok (NULL, ".") ;
		strcpy (sub1, tmp) ;
		tmp = strtok (NULL, ".") ;
		strcpy (sub2, tmp) ;
		sprintf (outfile, "%s_%s_%s", outpref, sub1, sub2) ;
		printf ("mod03 file is : %s", file1) ;
		therm = new modis_hdf (file0) ;
		geom = new modis_hdf (file1) ;

		mresamp->set_modis_hdfs (geom, therm) ;
		mresamp->set_bounds (starty, startx, endy, endx, gspace) ;
		mresamp->process() ;
		mresamp->write_output (outfile) ;
		delete therm ;
		delete geom ;
	}
	fclose (fin) ;
	delete mresamp ;
	//modis_hdf *mfile = new modis_hdf (infile) ;

	//delete mfile ;

	cout << "program done" << endl ;







}

