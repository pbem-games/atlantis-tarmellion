#!/usr/bin/perl
# script to create a current player list
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

#specify the relative path to the players.in file
$pin = "/home/tarmellion/code/tarmellion/players.in";
#specify the relative path to the faction description dir
$desc = "/home/tarmellion/faction_descriptions/";
#initializing existing faction counter
$efc = 0;
#parsing the faction description dir
opendir(DESC, "$desc");
@descriptions = readdir(DESC);
foreach $file (@descriptions) {
    next if ($file eq ".");
    next if ($file eq "..");
    ($faction,$dummy) = split(/\./,$file);
    $description{$faction} = 1;
}
close(DESC);

#preparing the html page
print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE></HEAD>\n";
print "<body background=/images/background.jpg>\n";
print "<h2>Current Player List of Tarmellion</h2>\n";
print "<p>Explanation:\n";
print "<br>faction description - a click on the faction name will open a page where a short description of the faction can be seen, if the player has written one. <a href=/tarmellion/description-form.html>Here</a> you can describe your faction to give a few hints for other players about you.\n";
print "<br>race - there are good and evil races in Tarmellion; belonging to a good or evil race will have an impact on gameplay.\n";
print "<br>reward points - the reward points are a reward for good roleplaying and good times submissions of the player; a click on the link will open a web form where the player can order some goodies that will help his faction.\n";
print "<br>missed turns - the first number gives the amount of missed turns in a row of the faction; if a faction misses more than 10 turns in a row, the GM can delete the faction; the second number gives the total number of missed turns in the game.\n";
print "<p><table border=1>\n";
print "<tr><td align=right>Nr.</td><td>Faction</td><td>email</td><td>race</td><td>Times Reward Points</td><td>Times Reward Points Total</td><td>missed turns</td></tr>\n";
#parsing the players.in file
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    #find the faction
    if (/^Faction:\s(\d*)/){
	$faction = $1;
	push(@factions,$faction);	
	$efc++;
    }
    #find the faction name
    if (/^Name:\s(.*)/){
	$name{$faction} = $1;
    }
    #find its email address
    if (/^Email:\s(.*)/){
	$email{$faction} = $1;
	$email2{$faction} = $email{$faction};
	$email2{$faction} =~ s/\@/ \[at\] /g;
    }
    #find its race
    if (/^Rasse:\s(.*)/){
	$race{$faction} = $1;
	$race{$faction} =~ s/\"//g;
    }
    #find its profession
    if (/^Profession:\s(.*)/){
	$profession{$faction} = $1;
    }
    #find its missed turns
    if (/^SendOrder:\s(\d*)\s(\d*)/){
	($dummy,$missedturns{$faction},$allmissedturns{$faction}) = split(/\s/,$_);
    }
    if (/^Timespunkte:\s(\d*)/){
	$timespoints{$faction} = $1;
    }
    if (/^Totaltimespunkte:\s(\d*)/){
	$totaltimespoints{$faction} = $1;
    }
}



foreach $Faction (@factions){
    #writing a table row for each faction
    print "<tr><td align=right>".$Faction."</td><td>";
    if ($description{$Faction} == 1) {
      print "<a href=/cgi-bin/tarmellion/description-show.pl/".$Faction.">";
    }
    print $name{$Faction};
    if ($description{$Faction} == 1) {
      print "</a>";
    }
print "</td><td>".$email2{$Faction}."</td><td>".$race{$Faction}."</td><td align=right><a href=/cgi-bin/tarmellion/reward-nav.pl/".$Faction." target=Navigation>".$timespoints{$Faction}."</a></td><td>".$totaltimespoints{$Faction}."</td><td>".$missedturns{$Faction}."/".$allmissedturns{$Faction}."</td></tr>\n";
}

#completing everything
print "</table></body>\n";
print "<p>There are ".$efc." factions currently.\n</html>\n";






