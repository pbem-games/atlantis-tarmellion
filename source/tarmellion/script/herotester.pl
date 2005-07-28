#!/usr/bin/perl
# script to check if a faction has more than one hero

#specify the absolute path to the players.in file
$pin = "/home/tarmellion/code/tarmellion/players.in";

#specify the absolute path to the report files
$rep = "/home/tarmellion/code/tarmellion/";

$Faction=0;
push(@factions, $Faction);

#parsing the players.in file
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    #find the factions
    if (/^Faction:\s(\d*)/){
	$Faction = $1;
	$entry{$Faction} = "Faction: $Faction\n";
	push(@factions, $Faction);
	next;
    }
    #find hero status
    if (/^Hero:\s(\d)/){
	$herocount{$Faction} = $1;
	next;
    }
    #saving all listed data
    $entry{$Faction} .= $_."\n";
}
close(PIN);

foreach $fact (@factions){
    $herocount{$fact}=0;
    next if ($fact eq "0");
    next if ($fact eq "1");
    next if ($fact eq "2");
    next if ($fact < 15);
    $end = 0;
    $report=$rep."report.".$fact;
    open(REP, "<$report");
    $ownunit = "no";
    while (<REP>) {
	chomp;
	next if ($end == 1);
	if (/^Orders Template/){
	    $end = 1;
	    next;
	}
	if ((/^\*\s(\.)*/) || (/^(\s)*\*s(\.)*/)){
	    ($dummy, $unit, $faction) = split(/\(/,$_);
	    ($unitnumber, $dummy) = split(/\)/,$unit);
	    ($factionnumber, $dummy) = split(/\)/,$faction);
	    $ownunit = "yes";
	}
	if (/^\-\s(\.)*/){
	    $ownunit = "no";
	}
	if (/^(\s)*\-\s(\.)*/){
	    $ownunit = "no";
	}
	if (/\[HERO\]/){
	    if ($ownunit eq "yes") {
		$herocount{$fact}++;
	    }
	}
    }
    close(REP);
}

#rewriting the players.in file
open(NEW, ">$pin");
foreach $fact (@factions){
    if ($herocount{$fact} > 1){
	print "WARNING: Faction $fact hat $herocount{$fact} heros\n";
    }
    print NEW $entry{$fact};
    print NEW "Hero: ".$herocount{$fact}."\n" unless ($fact == 0);
}
close(NEW);


		


















