#!/usr/bin/perl
#script to send out gm messages to all players

$turn = $ARGV[0];

$dir = "/home/tarmellion/code/tarmellion/";
$dir2 = $dir."script/";
$pin = $dir."players.in";

print "Sending message to ";
open(PIN, "<$pin");
while (<PIN>) {
chomp;
if (/^Faction:\s(\d*)/){
  $pfact = $1;
  next if ($pfact < 15);
}
if (/^Email:\s(.*)/){
  $pemail = $1;
  next if ($pemail eq "NoAddress");
  next if ($pfact < 15);
  print "$pfact ";
  $allemails .= ", $pemail" if ($allemails ne "");
  $allemails = $pemail if ($allemails eq "");
}
}
print "\n";
#$allemails = "hgrimm\@nexgo.de, tarmellion\@yahoo.de";
$cmd = 'Mail -s "message from the GM" -c "'.$allemails.'" herobane@nexgo.de < '.$dir.'message.txt';
print $cmd."\n";
system("$cmd 1>/dev/null");
