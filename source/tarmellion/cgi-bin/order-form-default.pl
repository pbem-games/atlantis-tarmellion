#!/usr/bin/perl
#script to submit orders from a preloaded default order form
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

$pin = "/home/tarmellion/code/tarmellion/players.in";
$path = "/home/tarmellion/code/tarmellion";
$oldorders = "";

@fields = param();

foreach $field (@fields) {
  $$field = param($field);
  $$field =~ s/\r//g;
}

open(PIN, "<$pin") || die "Couldn't open $pin: $!\n";
while (<PIN>) {
	chomp;
	if (/^Faction:\s(\d*)/){
		$pfact = $1;
	}

	if (/^Password:\s(.*)/){
		$ppasswd = $1;
	}

	$pass{$pfact} = $ppasswd;
}

if ($password eq $pass{$faction}) {
  if (-f $path."/web/orders.".$faction) {
    $file = $path."/web/orders.".$faction;
  } elsif (-f $path."/email/orders.".$faction) {
    $file = $path."/email/orders.".$faction;
  } else {
    $file = $path."/orders.".$faction;
  }
  open(FILE, "<$file") || die "Couldn't open $file: $!\n";
  while (<FILE>){
    $oldorders .= $_;
  }
  close(FILE);
}

print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE></HEAD>\n";
print "<BODY background=/images/background.jpg>\n";
print "<h2>Order Input Form for Faction $faction</h2>\n";
print "<p><b>usage</b>\n";
print " <ul>\n";
print " <li> faction number and password are necessary for storing the turn.</li>\n";
print " <li> in the orders field, your orders must start with #atlantis and end with #end. Any orders missing these lines won't be executed.</li>\n";
print " <li>The web orders take precedence over the email orders, so be sure to change web orders only by submitting new web orders.</li>\n";
print "</ul>\n";
print " <table border>\n";
print " <form action=/cgi-bin/tarmellion/order-form.pl method=POST>\n";
print " <tr><td>faction number:</td><td><input name=faction size=12 value=".$faction."></td></tr>\n";
print " <tr><td>password:</td><td><input name=password type=password size=12 value=".$pass{$faction}."></td></tr>\n";
print " <tr><td colspan=2>orders:</td></tr>\n";
print " <tr><td colspan=2><textarea name=orders cols=80 rows=30 wrap=virtual>".$oldorders."</textarea></td></tr>\n";
print " <tr><td colspan=2 align=center><input type=submit value=Save></td></tr>\n";
print " </form>\n";
print " </table>\n";
print " </body>\n";
print " </html>\n";
