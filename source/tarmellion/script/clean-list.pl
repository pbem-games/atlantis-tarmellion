#!/usr/local/bin/perl
#script to change format of settlement files

$datei = $ARGV[0];

print "Reading $datei\n";

open(TABEL, "<$datei");
while(<TABEL>){
	chop;
	next if (/^-/);
	next if (/^\s/);
	if (/^\d/){
	    ($x, $y, $z) = split(/,/,$_);
	    $z = 0 if ($z eq "nexus");
	    $z = 1 if ($z eq "");
	    $z = 2 if ($z eq "underworld");
	    $z = 3 if ($z eq "deep underworld");
	    $z = 4 if ($z eq "very deep underworld");
	    $z = 5 if ($z eq "underdeep");
	    $z = 6 if ($z eq "deep underdeep");
	    $z = 7 if ($z eq "very deep underdeep");
	    $newline = $x." ".$y." ".$z;
	} elsif (/^\w/){
	    $newline = $newline." |".$_."\n";
	    push(@lines,$newline);
	}
}
close(TABEL);

print "Writing $datei\n";

open(NEU, ">$datei");
foreach $new (@lines){
  print NEU $new;
}

close(NEU);
print "Fertisch\n";



