#!/usr/bin/perl  -w

$indir = $ARGV[0] ;
$m21 = $indir.'/MOD021*' ;
$m03 = $indir.'/MOD03*' ;

@filelist = `ls -1 $m21` ;
@filelist_mod3 = `ls -1 $m03` ;
foreach $file (@filelist)
{
	$startind = index ($file, '.A20') ;
	$mystr = substr ($file,  $startind+1, 17) ;
	$nlind = index ($file, "\n") ;
	if ($nlind >0) {
		$file=substr ($file,0,$nlind) ;
	}

	foreach $file_03 (@filelist_mod3) {
		$indval = index ($file_03, $mystr) ;
		if ($indval > 0) {
			$nlind = index ($file_03, "\n") ;
			if ($nlind >0) {
				$file_03=substr ($file_03,0,$nlind) ;
			}

			printf("%s\n", ($file.' '.$file_03)) ;
		}
	}
}
	

