#!/usr/bin/perl
# script to create a nav page for rewards
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

$q = new CGI;

#specify the relative path to the players.in file
$pin = "/home/tarmellion/code/tarmellion/players.in";
$Faction = $q->path_info();
$Faction =~ s|^/||;
$goodraces = "elf human dwarf ffolk viking dervish nomad darkman halfelf barbarian highlander clansman imperial tribesman ffolk cajun folk cajun man lowlander gnome halfing lizardman";

#preparing the html page
print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE></HEAD>\n";
print "<base target=Datei>\n";
print "<body background=/images/background.jpg>\n";
print "<h2>Available Times Rewards for Faction $Faction</h2><p>\n";

#parsing the players.in file
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    #find the faction
    if (/^Faction:\s(\d*)/){
	$pfact = $1;
    }
    #find the reward points
    if ((/^Timespunkte:\s(\d*)/) && ($pfact eq $Faction)){
	$timespoints = $1;
    }
    #find race, alignment and profession of faction
    if ((/^Rasse:\s(\.*)/) && ($pfact eq $Faction)){
      ($dummy,$race) = split(/\s/,$_);
	$race =~ s/"//g;
	if ($goodraces =~ $race){
	$alignment = "good";
      } else {
	$alignment = "evil";
      }
    }
    #find hero status of faction
    if ((/^Hero:\s(\d)/) && ($pfact eq $Faction)){
	($dummy,$herocount) = split(/\s/,$_);
    }
}

print"<p>current Reward points: ".$timespoints."\n<ul>";

#writing the reward links
print "<li>";
#points for money
if ($timespoints > 0) {
  print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_money_".$alignment.">";
}
print "unclaimed money reward";
if ($timespoints > 0) {
  print"</a>";
}
print "<br>TRP: 1+";
print " (x points)</li>\n<li>";
#changing starting location
if ($timespoints > 1) {
  print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_loc_".$alignment.">";
}
print "change starting location";
if ($timespoints > 1) {
  print"</a>";
}
print "<br>TRP: 2+";
print " (2 points)</li>\n<li>";
#normal units with ressource production skills
if ($timespoints > 9) {
  print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_unit10_".$alignment.">";
}
print "laborers";
if ($timespoints > 9) {
  print"</a>";
}
print "<br>TRP: 10+";
print " (10 each)</li>\n<li>";
#normal units with production skills
if ($timespoints > 11) {
  print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_unit20_".$alignment.">";
}
print "artisans";
if ($timespoints > 11) {
  print"</a>";
}
print "<br>TRP: 12+";
print " (12 each)</li>\n<li>";
#normal units with combat skills
if ($timespoints > 11) {
  print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_soldier_".$alignment.">";
}
print "soldiers";
if ($timespoints > 11) {
  print"</a>";
}
print "<br>TRP: 12+";
print " (12 each)</li>\n<li>";
#leader units with ressource production skills
if ($timespoints > 24) {
  print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_leader10_".$alignment.">";
}
print "master laborers";
if ($timespoints > 24) {
  print"</a>";
}
print "<br>TRP: 25+";
print " (25 each)</li>\n<li>";
#leader units with product production skills
if ($timespoints > 27) {
  print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_leader20_".$alignment.">";
}
print "master artisans";
if ($timespoints > 27) {
  print"</a>";
}
print "<br>TRP: 28+";
print " (28 each)</li>\n<li>";
#leader soldiers
if ($timespoints > 29) {
  print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_elitesoldier_".$alignment.">";
}
print "elite soldiers";
if ($timespoints > 29) {
  print"</a>";
}
print "<br>TRP: 30+";
print " (30 each)</li>\n<li>";
#animals
if ($timespoints > 29) {
  print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_animal_".$alignment.">";
}
print "creatures";
if ($timespoints > 29) {
  print"</a>";
}
print "<br>TRP: 30+";
print " (x each)</li>\n<li>";
#weapons
if ($timespoints > 29) {
    print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_weapon_".$alignment.">";
}
print "weapons";
if ($timespoints > 29) {
  print"</a>";
}
print "<br>TRP: 30+";
print " (x each)</li>\n<li>";
#other specialists
if ($timespoints > 39) {
  print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_specialist_".$alignment.">";
}
print "other specialists";
if ($timespoints > 39) {
  print"</a>";
}
print "<br>TRP: 40+";
print " (40+ each)</li>\n<li>";
#replacement heros
if (($herocount < 1) && ($timespoints > 49)) {
    print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_hero_".$alignment.">";
}
print "replacement hero";
if (($herocount < 1) && ($timespoints > 49)) {
  print"</a>";
}
print "<br>TRP: 50+";
print " (50 points)</li>\n<li>";
#mages
if ($timespoints > 99) {
  print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_mage_".$alignment.">";
}
print "mage";
if ($timespoints > 99) {
  print"</a>";
}
print "<br>TRP: 100+";
print " (100 each)</li>\n<li>";
#magic items
if ($timespoints > 99) {
    print "<a href=/cgi-bin/tarmellion/reward-form.pl/".$Faction."_magicitem_".$alignment.">";
}
print "magic items";
if ($timespoints > 99) {
  print"</a>";
}
print "<br>TRP: 100+";
print " (x each)</li>\n";
print "</ul><br>";
#completing everything
print "<p><a href=/tarmellion/mechanics-nav.html target=Navigation>back</a>\n";
print "</body></html>\n";


