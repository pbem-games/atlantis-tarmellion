#!/usr/bin/perl
# script to display race descriptions
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

$q = new CGI;

$entry = $q->path_info();
$entry =~ s|^/||;
$entry =~ s/_/ /g;

#specify the relative path to the race description file
$rfile = "/home/tarmellion/html/rassen.txt";

#preparing the html page
print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE>\n";
print "<base target=Datei>\n</head>\n";
print "<body background=/images/background.jpg>\n";
print "<h2>$entry</h2><p>\n";

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

print "<p>".$description{$entry}."\n";

print "</body></html>\n";


