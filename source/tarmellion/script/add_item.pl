#!/usr/bin/perl
# script to add items to city markets

#initializing variables
$market = $ARGV[0];
$item = $ARGV[1];
$price = $ARGV[2];
$basepop = $ARGV[3];
$baseamount = $ARGV[4];
$toppop = $ARGV[5];
$topamount = $ARGV[6];

$dir = "/home/tarmellion/code/tarmellion/";
$dir2 = $dir."script/";
#$dir = "/home/hgrimm/atlantis/cvs/tarmellion/";
$Faction = 0;
push(@factions, $Faction);

#specify the relative path to the players.in and players.in.new files
$pin = $dir."players.in";
$npin = $pin;
#$npin = $pin.".new";
$all_settlements = $dir2."cities2";

#parsing the players.in file to get the current stats
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
    #saving all listed data
    $entry{$Faction} .= $_."\n";
}
close(PIN);

#adding new item to markets

open(ALL, "<$all_settlements");
while (<ALL>) {
  chomp;
  next if ($_ eq "");
  $entry{1} .= "Loc: ".$_."\n";
  $entry{1} .= "Market: ";
  if ($market eq "SELL"){
    $entry{1} .= "SELL ";
   } else {
     $entry{1} .= "BUY ";
   }
  $entry{1} .= $item." ".$price." ".$basepop." ".$baseamount." ".$toppop." ".$topamount."\n";
}

#writing players.in.new
open(NEW, ">$npin")|| trouble opening players.in.new file;
foreach $Faction (@factions){
  print NEW $entry{$Faction};
}
close(NEW);


