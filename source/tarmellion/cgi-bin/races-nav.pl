#!/usr/bin/perl
# script to create a nav-frame for race descriptions
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

$q = new CGI;

#specify the relative path to the race description file
$rfile = "/home/tarmellion/html/rassen.txt";

#preparing the html page
print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE>\n";
print "<base target=Datei>\n</head>\n";
print "<body background=/images/background.jpg>\n";
print "<h2>Races of Tarmellion</h2><p>\n";

#parsing the race description file
open(RFILE, "<$rfile");
while (<RFILE>) {
    chomp;
    #find the race
    if (/^Race:\s(.*)/){
	$race = $1;
	push(@races,$race);
    }
    #find the description
    if (/^Description:\s(.*)/){
	$description{$race} = $1;
    }
}

foreach $entry (sort @races){
  $link = $entry;
  $link =~ s/\s/_/g;
  $print = "<p>";
  if ($description{$entry} ne ""){
    $print .= "<a href=/cgi-bin/tarmellion/races-desc.pl/$link>";
  }
  $print .= "$entry";
  if ($description{$entry} ne ""){
    $print .= "</a>";
  }
  $print .= "\n";
  print $print;
}

#completing everything
print "<p><a href=/tarmellion/flavour-nav.html target=Navigation>back</a>\n";
print "</body></html>\n";
