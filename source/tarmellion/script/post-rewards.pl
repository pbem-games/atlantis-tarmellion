#!/usr/bin/perl
# script to add rewards for times posts to players.in file

#initializing variables
$dir = "/home/tarmellion/code/tarmellion/testturn/";
$Faction = 0;
push(@nextfactions, $Faction);
$option = $ARGV[0];

#specify the relative path to the players.in and players.in.new files
$pin = $dir."players.in";
if ($option eq "real") {
    $npin = $pin;
} else {
    $npin = $pin.".new";
}
$posts = $dir."timesrewards.new";

#parsing the players.in file to get the current stats
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    #find the turn number
    if (/^TurnNumber:\s(\d*)/){
	($dummy,$turn) = split(/\s/,$_);
    }
    #find the factions
    if (/^NextFaction:\s(\d*)/){
	$Faction = $1;
	$entry{$Faction} = "NextFaction: $Faction\n";
	push(@nextfactions, $Faction);
#	print "Faction $Faction\n";
	next;
    }
    #find old times rewards
    if (/^Timespunkte:\s(\d*)/){
      ($dummy,$reward{$Faction}) = split(/\s/,$_);
      $oldreward{$Faction} = $reward{$Faction};
#	print "Faction $Faction Timespunkte: $reward{$Faction}\n";
	next;
    }
    #find old totaltimes rewards
    if (/^Totaltimespunkte:\s(\d*)/){
      ($dummy,$totalreward{$Faction}) = split(/\s/,$_);
      $oldtotalreward{$Faction} = $totalreward{$Faction};
      next;
    }
    #find old rewards
    if (/^Reward:\s(\d*)/){
      $silver{$Faction} = $1;
      next;
    }
    #saving all listed data
    $entry{$Faction} .= $_."\n";
}
close(PIN);

#$Faction++;
#push(@nextfactions,$Faction);

$max = $turn + 12;

#reading post reward file
if (open(POST, "<$posts")){
    while(<POST>){
	chomp;
	if (/^Faction: (\d+)/) {
	    $nowfaction = $1;
	    $nextfaction = $nowfaction + 1;
	} elsif (/^Reward: (\d+)/) {
	    $silver{$nextfaction} = $silver{$nextfaction} + $1;
	} elsif (/^Timespunkte: (\d+)/) {
	    if ($1 > $max){
		print "$nowfaction ($max) ";
		$reward{$nextfaction} = $reward{$nextfaction} + $max;
		$totalreward{$nextfaction} = $totalreward{$nextfaction} + $max;
	    } else {
		print "$nowfaction ($1) ";
		$reward{$nextfaction} = $reward{$nextfaction} + $1;
		$totalreward{$nextfaction} = $totalreward{$nextfaction} + $1;
	    }
	} 	 
    }
    print "\n";
    close(POST);
#writing players.in.new
    open(NEW, ">$npin")|| trouble opening players.in.new file;
    foreach $Faction (@nextfactions){
	if ($Faction ne "1") {
#  print "NextFaction: ".$Faction."\n";
#  print "Timespunkte: ".$reward{$Faction}."\n" if ($reward{$Faction} > 0);
	    print NEW "Timespunkte: ".$reward{$Faction}."\n";
	    print NEW "Totaltimespunkte: ".$totalreward{$Faction}."\n";
	    if ($silver{$Faction} > 0){
		print NEW "Reward: ".$silver{$Faction}." SILV\n";
	    }
	}
#	print "Faction $Faction Timespunkte $reward{$Faction}\n";
	print NEW $entry{$Faction};
#  print "Timespunkte: ".$reward{$Faction}."\n";
    }
    close(NEW);
    
}


