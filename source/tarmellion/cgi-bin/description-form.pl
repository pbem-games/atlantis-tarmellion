#!/usr/bin/perl
# script to describe a faction in the playerlist
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

$q = new CGI;

$Faction = $q->path_info();
$Faction =~ s|^/||;

#specify the relative path to the faction description dir
$desc = "/home/tarmellion/faction_descriptions/";
#specify the relative path to the faction description file
$file = $desc.$Faction.".desc";

#preparing the html page
print "<html>\n";
print "<head><title>Tarmellion</title></head>\n";
print "<body background=/images/background.jpg>\n";

#parsing the faction description file
open(FILE, "<$file")|| warn "Can´t open $file\n";
while(<FILE>){
    $description{$Faction} .= $_;
#	print "<p>$_\n";
}
close(FILE);

#preparing the html page
#print "<HTML>\n";
#print "<HEAD><TITLE>Tarmellion</TITLE></HEAD>\n";
#print "<body background=/images/background.jpg>\n";
print "<h2>faction description web form</h2>\n";
print "<p>You can use html-tags for the description of your faction, but if you do not want to use them, plain text is okay too.\n";
print "<p>\n";
print "<table border=1>\n";
print "<form action=/cgi-bin/tarmellion/description-save.pl method=POST>\n";
print "<tr><td valign=top>Faction number:</td><td><input name=Faction size=10 value=$Faction></td></tr>\n";
print "<tr><td valign=top>Password:</td><td><input name=password type=password size=20></td></tr>\n";
print "<tr><td valign=top> Description:</td><td><textarea name=desc cols=80 rows=30>".$description{$Faction}."</textarea></td></tr>\n";
print "<tr><td colspan=2 align=center><input type=submit value=save></td></tr>\n";
print "</table>\n";
#print "<p>$description{$Faction} $file\n";
print "<p>\n";
print "</body>\n";
print "</html>\n";
