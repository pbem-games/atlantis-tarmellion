#!/usr/bin/perl
# script to view faction description
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

$q = new CGI;

#specify the relative path to the faction description dir
$desc = "/home/tarmellion/faction_descriptions/";

$Faction = $q->path_info();
$Faction =~ s|^/||;

#preparing the html page
print "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Strict//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">\n\n";
print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE></HEAD>\n";
print "<base target=Datei>\n";
print "<body background=/images/background.jpg>\n";

#parsing the faction description file
$file = $desc.$Faction.".desc";
$description = "";
open(DESC, "<$file");
while (<DESC>) {
    $description .= $_;
}
close(DESC);

if ($description eq ""){
    print "<p>Here would be the description for faction ".$Faction.", if the player had written any.\n";
} else {
    if ($description =~ /^</){
	print $description;
    } else {
	print "<pre>\n$description\n</pre>\n";
    }
}

print "<p><p align=right><a href=/cgi-bin/tarmellion/description-form.pl/".$Faction.">edit</a>\n";
print "</body></html>\n";





