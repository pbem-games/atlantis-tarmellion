#!/usr/bin/perl

$turn = $ARGV[0];

$dir = "/home/tarmellion/code/tarmellion/";
$dir2 = $dir."script/";
$pin = $dir."players.in";

print "Sending reports for ";
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
	$cmd = 'Mail -s "report.'.$pfact.' for turn '.$turn.'" -a "Reply-To:tarmellion-orders@herobane.de" '.$pemail.' < '.$dir.'report.'.$pfact;
    print "$pfact ";
    system("$cmd 1>/dev/null");
}
#$email{$pfact} = $pemail;
}
print "\n";
#$i = 3;
#while ($i < 64) {
#    $cmd = 'Mail -s "report.'.$i.'" '.$email{$i}.' < report.'.$i;
#    print "$cmd 1>/dev/null";
#    system("$cmd 1>/dev/null");
#    $i++;
#    next;
#}
#You must specify direct recipients with -s, -c, or -b.

