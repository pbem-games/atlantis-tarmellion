#!/usr/bin/perl
# script to check if a faction has more than one hero

$flag = $ARGV[0];

#specify the absolute path to the players.in file
$pin = "/home/tarmellion/code/tarmellion/players.in";

#specify the absolute path to the report files
$rep = "/home/tarmellion/code/tarmellion/";

#specify the path to the village database
$list = $rep."script/village-database";
if ($flag eq "test"){
   $listnew = $list.new
} else {
   $listnew = $list;
}

#parsing the players.in file
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    #find the factions
    if (/^Faction:\s(\d*)/){
	$Faction = $1;
	push(@factions, $Faction);
	next;
    }
    #find turn number 
    if (/^TurnNumber:\s(\d*)/){
	$turn = $1;
	next;
    }
}
close(PIN);

print "Checking ";
foreach $fact (@factions){
    next if ($fact < 15);
    print "$fact ";
    $report=$rep."report.".$fact;
    open(REP, "<$report");
    while (<REP>) {
	chomp;
	next unless (/^Damaged units:/);
   	chop($_);
	($dummy, $casus) = split(/: /,$_);
	(@casuls) = split(/, /,$casus);
	foreach $casi (@casuls){
	    push(@casualties,$casi);
	}
    }
    close(REP);
}
print "\n";
open(LIST, "<$list");
while(<LIST>){
    ($coord, $name, $race, $leader, $guards, $fallen) = split(/\|/,$_);
    $cas = "no";
    foreach $j (sort @casualties){
      if ($leader eq $j){
	$cas = "yes";
      }
    }
    if ($cas eq "yes"){
	print "$name in $coord has fallen!\n";
	$entry = "$coord|$name|$race|keiner|0|$turn\n";
    } else {
	$entry = $_;
    }
    push(@newlist,$entry);
  
}
close(LIST);

open(NEWLIST, ">$listnew");
foreach $i (@newlist){
    print NEWLIST $i;
}
close(NEWLIST);
