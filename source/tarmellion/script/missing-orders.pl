#!/usr/bin/perl
#script to check if a faction has not send in orders this turn

#initializing variables
$dir = "/home/tarmellion/code/tarmellion/";
$turn = $ARGV[0];
$Faction = 0;
push(@factions, $Faction);
$webpath = $dir."/web";
$mailpath = $dir."/email";
$advancepath = $dir."/web-advance/".$turn;

$pin = $dir."/players.in";

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
  #saving number of submitted orders
  if (/^SendOrder:\s(\d*)\s(\d*)/){
    $missed{$Faction} = $1;
    $allmissed{$Faction} = $2;
    next;
  }
  #saving all listed data
  $entry{$Faction} .= $_."\n";
}

$webliste = "";
$mailliste = "";
$advanceliste = "";

chdir("$webpath");
open(LS1, "ls |");
while(<LS1>) {
    chomp;
  $webliste .= $_." ";
}
close(LS1);

chdir("$mailpath");
open(LS2, "ls |");
while(<LS2>) {
    chomp;
  $mailliste .= $_." ";
}
close(LS2);

chdir("$advancepath");
open(LS3, "ls |");
while(<LS3>) {
    chomp;
  $advanceliste .= $_." ";
}
close(LS3);

$orderliste = $webliste.$mailliste.$advanceliste;

open(NEW, ">$pin")|| trouble opening players.in.new file;
foreach $Faction (@factions){
  print NEW $entry{$Faction};
  next if ($Faction eq "0");
  next if ($Faction eq "1");
  next if ($Faction eq "2");
#  next if ($Faction eq "3");
#  next if ($Faction eq "4");
#  next if ($Faction eq "5");
#  next if ($Faction eq "6");
#  next if ($Faction eq "7");
#  next if ($Faction eq "8");
#  next if ($Faction eq "9");
#  next if ($Faction eq "10");
#  next if ($Faction eq "11");
#  next if ($Faction eq "12");
#  next if ($Faction eq "13");
#  next if ($Faction eq "14");
  $Faction++;
  $Faction--;
  if ($Faction < 15){
      print NEW "SendOrder: 0 0\n";
      next;
  }
  $item = "orders.".$Faction." ";
 if ($orderliste =~ $item){
#      print "$Faction hat orders geschickt\n";
    print NEW "SendOrder: 0 ".$allmissed{$Faction}."\n";
  } else {
    $missed{$Faction}++;
    $allmissed{$Faction}++;
#      print "$Faction hat keine orders geschickt\n";
    print NEW "SendOrder: ".$missed{$Faction}." ".$allmissed{$Faction}."\n";
  }
}
close(NEW);






