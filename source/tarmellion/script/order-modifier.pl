#!/usr/bin/perl
# script to modify orders of all NPC factions

#initializing variables
$dir = "/home/tarmellion/code/tarmellion/web/";
@factions =(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);
$option = $ARGV[0];

foreach $faction (@factions){

#specify the relative path to the orders files
    $pin = $dir."orders.".$faction;
	if ($option eq "test"){
		$npin = $pin.".new";
	} else {
		$npin = $pin;
	}
    $entry = "";
    
#parsing the players.in file to get the current stats
    open(PIN, "<$pin");
    while (<PIN>) {
	chomp;
	#find the IDLE order
	if (/^\@IDLE/){
	    $entry .= "AUTOTAX 1\n";
	}
	$entry .= $_."\n";
    }
    close(PIN);


#writing new orders
    open(NEW, ">$npin")|| warn "trouble opening $npin file\n";
    print NEW $entry;
    close(NEW);
}



