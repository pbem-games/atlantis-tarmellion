#!/usr/bin/perl
# script to add correct unit numbers to players.in file at gamestart

#initializing variables
$dir = "/home/tarmellion/code/tarmellion/";
$Faction = 0;
push(@factions, $Faction);

#specify the relative path to the players.in and players.in.new files
$pin = $dir."players.in";
$npin = $pin.".new";


#parsing the players.in file to get the current stats
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    #find the factions
    if (/^Faction:\s(\d*)/){
	$Faction = $1;
	push(@factions, $Faction);
	next;
    }
    #find the highest added unit number
    if (/^NewUnit:\s(\d*)/){
	($dummy,$unitnumber) = split(/\s/,$_);
	if ($unitnumber > $newnumber){
	    $newnumber = $unitnumber;
	}
    }
}
close(PIN);

#specify the relative path to the newplayers.in file
$pinnew = "/home/tarmellion/atlantis/tarmellion/newplayers.in";
open(PINNEW, "<$pinnew");
while (<PINNEW>) {
    chomp;
    #find the last added unit
    if (/^NewUnit:\s(\d*)/){
	($dummy,$unitnumber) = split(/\s/,$_);
	if ($unitnumber > $newnumber){
	    $newnumber = $unitnumber;
	}
    }
}
close(PINNEW);

$newnumber++;

#changing the players.in file to put new unit numbers in
$Faction = 0;
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    #find the factions
    if (/^Faction:\s(\d*)/){
	$Faction = $1;
	$entry{$Faction} = "Faction: $Faction\n";
	next;
    }
    #find the next unnumbered new unit
    if ($_ eq "NewUnit: "){
	$entry{$Faction} .= "NewUnit: ".$newnumber."\n";
	$oldnumber = $newnumber;
	$newnumber++;
	next;
    }
    #set correct number to each unnumbered entry
    s/ gm / gm$oldnumber /g;
	
    #saving all listed data
    $entry{$Faction} .= $_."\n";
}
close(PIN);

#writing players.in.new
open(NEW, ">$npin")|| trouble opening players.in.new file;
foreach $Faction (@factions){
  print NEW $entry{$Faction};
}
close(NEW);


