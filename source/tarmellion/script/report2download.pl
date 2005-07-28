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
  	$report = "report.".$faction;
	$sourcereport = $dir.$report;
	$targetdir = "/home/tarmellion/html/report/";
	$targetreport = $targetdir.$report.".".$password{$faction};
	$sourcezip = $sourcereport.".zip";
	$targetzip = $targetdir.$report.".zip.".$password{$faction};
	`cp $report "$targetreport"`;
	next if ($faction < 15);
        `cp $sourcezip "$targetzip"`;
}

