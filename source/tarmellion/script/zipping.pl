#!/usr/bin/perl

$dir = "/home/tarmellion/code/tarmellion/";
$pin = $dir."players.in";

print "Zipping reports for ";
open(PIN, "<$pin");
while (<PIN>) {
chomp;
  if (/^Faction:\s(\d*)/){
    $pfact = $1;
    next if ($pfact < 15);
    print "$pfact ";
    $target = $dir."report.".$pfact.".zip";
    $source = $dir."report.".$pfact;
    `zip -q $target $source` if (-f $source);
  }
}
print "\n";
