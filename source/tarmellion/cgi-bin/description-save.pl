#!/usr/bin/perl
#script to save the faction description
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

$pfact=0;
$ppasswd="";
@fields = param();

foreach $field (@fields) {
  $$field = param($field);
  $$field =~ s/\r//g;
}
push(@pfacts,$pfact);

$pin = "/home/tarmellion/code/tarmellion/players.in";
$descdir = "/home/tarmellion/faction_descriptions/";
$file = $descdir.$Faction.".desc";

open(PIN, "<$pin") || die "Couldn't open $pin: $!\n";
while (<PIN>) {
    chomp;
    if (/^Faction:\s(\d*)/){
	$pfact = $1;
	$entry{$pfact} = "Faction: $pfact\n";
	push(@pfacts,$pfact);
	next;
    }
    #find old times rewards
    if (/^Timespunkte:\s(\d*)/){
      ($dummy,$reward{$pfact}) = split(/\s/,$_);
      $oldreward{$pfact} = $reward{$pfact};
      next;
    }
    #find old total times rewards
    if (/^Totaltimespunkte:\s(\d*)/){
	($dummy,$totalreward{$pfact}) = split(/\s/,$_);
	$totaltimesreward{$pfact} = $totalreward{$pfact};
	next;
    }
    #find old rewards
    if (/^Reward:\s(\d*)\sSILV/){
	$silver{$pfact} = $1;
	next;
    }
    #saving all listed data
    $entry{$pfact} .= $_."\n";
    if (/^Password:\s(.*)/){
	$ppasswd = $1;
	$passwd{$pfact} = $ppasswd;
    }
    
}
close(PIN);

if ($entry{$Faction} eq ""){
    $ok = 2;
} else {

$change = 0;
$change = 1 if (-f $file);

if ($change == 1){
    if ($silver{$Faction} == 500){
	$reward{$Faction} = $reward{$Faction};
    } elsif ($silver{$Faction} == 50){
	$reward{$Faction} = $reward{$Faction};
    } else {
		$totaltimesreward{$Faction} = $totaltimesreward{$Faction} + 5;
	$reward{$Faction} = $reward{$Faction} + 5;
	$silver{$Faction} = $silver{$Faction} + 50;
    }
} else {
    push(@factions,$Faction);
	$totaltimesreward{$Faction} = $totaltimesreward{$Faction} + 50;
    $reward{$Faction} = $reward{$Faction} + 50;
    $silver{$Faction} = $silver{$Faction} + 500;
}

#Saving the players.in again
if ($password ne $passwd{$Faction}) {
  print "<p>wrong password given!!!!!<br>Cannot save the rewards.<br>Please try again\n";
} else {
  #storing reward for description 
  open(NEW, ">$pin");
  foreach $fact (@pfacts){
      print NEW $entry{$fact};
      if ($silver{$fact} > 0) {
	  print NEW "Reward: ".$silver{$fact}." SILV\n";
      }
      print NEW "Timespunkte: ".$reward{$fact}."\n" if ($fact > 0);
      print NEW "Totaltimespunkte: ".$totaltimesreward{$fact}."\n" if ($fact > 0);
  }
  close(NEW);

  #storing description
  open(FILE, ">$file") || "Couldn't open $file: $!\n";
  print FILE $desc."\n";
  close(FILE);
  `chmod 664 $file`;
  $ok = 1;
}
}

print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE></HEAD>";
if ($ok == 1) {
    print "<BODY background=/images/background.jpg><p>The description was stored.\n";
} elsif ($ok == 2){
	print "<BODY background=/images/background.jpg><p>You have no faction yet. Please wait for this until you recieve your first turn report.\n";
} else {
	print "<BODY background=/images/background><p><strong>Wrong password!</strong> <p>Description was not stored.";
}
print "</BODY></HTML>";



