#!/usr/bin/perl
#script to copy the current reports to a download area

#initializing variables
$dir = "/home/tarmellion/code/tarmellion/";
$pin = $dir."/players.in";

#parsing the players.in file to get the current stats
open(PIN, "<$pin");
while (<PIN>) {
  chomp;
  #find the factions
  if (/^Faction:\s(\d*)/){
    $Faction = $1;
    push(@factions, $Faction);
  }
  #saving number of submitted orders
  if (/^Password:\s(.*)/){
    $password{$Faction} = $1;
  }
}
close(PIN);

foreach $faction (@factions) {
  next if ($faction eq "1");
  next if ($faction eq "2");
  next if ($password{$faction} eq "");
	`cp $dir/report.$faction /home/tarmellion/html/report/report.$faction.$password{$faction}`;
}

