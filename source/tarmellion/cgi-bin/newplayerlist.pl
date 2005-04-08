#!/usr/bin/perl
# script to create a list of new players
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

#specify the relative path to the players.in files
$npin = "/home/tarmellion/code/tarmellion/newplayers.in";
$pin = "/home/tarmellion/code/tarmellion/players.in";

#initialize variables
$warrior = 0;
$officer = 0;
$merchant = 0;
$artificer = 0;
$noble = 0;
$seaman = 0;
$mage = 0;
$assassin = 0;

$blackkeepcity = 0;
$amazonnation = 0;
$republicofpelisham = 0;
$buccaneerfellowship = 0;
$duergarkingdom = 0;
$drowmatriarchy = 0;
$ramishempire = 0;
$silsalon = 0;
$halfingdemocracy = 0;
$vikingconfederacy = 0;
$dwarvenkingdom = 0;
$darkmankingdom = 0;

$human = 0;
$barbarian = 0;
$tribesman = 0;
$viking = 0;
$highlander = 0;
$ffolk = 0;
$dervish = 0;
$cajunman = 0;
$nomad = 0;
$clansman = 0;
$lowlander = 0;
$darkman = 0;
$imperial = 0;
$elf = 0;
$gnome = 0;
$halfing = 0;
$halfelf = 0;
$dwarf = 0;
$lizardman = 0;
$buccaneer = 0;
$republican = 0;
$headhunter = 0;
$sandling = 0;
$serpentpeople = 0;
$blackkeep = 0;
$amazon = 0;
$hobgoblin = 0;
$troll = 0;
$orc = 0;
$goblin = 0;
$gnoll = 0;
$ogre = 0;
$minotaur = 0;
$uruk = 0;
$drow = 0;
$duergar = 0;

$good = 0;
$evil = 0;

#preparing the html page
print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE></HEAD>\n";
print "<body background=/images/background.jpg>\n";
print "<h2>List of new Players of Tarmellion</h2>\n";
print "<p><table border=1>\n";
print "<tr><td>Faction ID</td><td>Faction</td><td>email</td><td>race</td></tr>\n";
#parsing the newplayers.in file
$faction = 0;
open(NPIN, "<$npin");
while (<NPIN>) {
    chomp;
    #find the faction
    if (/^Faction:\s(\d*)/){
	$faction++;
	push(@factions,$faction);
    }
    #find the faction numbers
    if (/^NextFaction:\s(\.*)/){
	($dummy,$nextfaction) = split(/\s/,$_);
	$nextfaction--;
	$factionnumber{$faction} = $nextfaction;
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
	if ($race{$faction} eq "human") {
	    $human++;
	    $good++;
	} elsif ($race{$faction} eq "barbarian") {
	    $barbarian++;
	    $good++;
	} elsif ($race{$faction} eq "tribesman") {
	    $tribesman++;
	    $good++;
	} elsif ($race{$faction} eq "viking") {
	    $viking++;
	    $good++;
	} elsif ($race{$faction} eq "highlander") {
	    $highlander++;
	    $good++;
	} elsif ($race{$faction} eq "ffolk") {
	    $ffolk++;
	    $good++;
	} elsif ($race{$faction} eq "dervish") {
	    $dervish++;
	    $good++;
	} elsif ($race{$faction} eq "cajun man") {
	    $cajunman++;
	    $good++;
	} elsif ($race{$faction} eq "nomad") {
	    $nomad++;
	    $good++;
	} elsif ($race{$faction} eq "clansman") {
	    $clansman++;
	    $good++;
	} elsif ($race{$faction} eq "lowlander") {
	    $lowlander++;
	    $good++;
	} elsif ($race{$faction} eq "darkman") {
	    $darkman++;
	    $good++;
	} elsif ($race{$faction} eq "imperial") {
	    $imperial++;
	    $good++;
	} elsif ($race{$faction} eq "elf") {
	    $elf++;
	    $good++;
	} elsif ($race{$faction} eq "gnome") {
	    $gnome++;
	    $good++;
	} elsif ($race{$faction} eq "halfing") {
	    $halfing++;
	    $good++;
	} elsif ($race{$faction} eq "halfelf") {
	    $halfelf++;
	    $good++;
	} elsif ($race{$faction} eq "dwarf") {
	    $dwarf++;
	    $good++;
	} elsif ($race{$faction} eq "lizardman") {
	    $lizardman++;
	    $good++;
	} elsif ($race{$faction} eq "buccaneer") {
	    $buccaneer++;
	    $evil++;
	} elsif ($race{$faction} eq "republican") {
	    $republican++;
	    $evil++;
	} elsif ($race{$faction} eq "headhunter") {
	    $headhunter++;
	    $evil++;
	} elsif ($race{$faction} eq "sandling") {
	    $sandling++;
	    $evil++;
	} elsif ($race{$faction} eq "serpent people") {
	    $serpentpeople++;
	    $evil++;
	} elsif ($race{$faction} eq "black keep leader") {
	    $blackkeep++;
	    $evil++;
	} elsif ($race{$faction} eq "amazon") {
	    $amazon++;
	    $evil++;
	} elsif ($race{$faction} eq "hobgoblin") {
	    $hobgoblin++;
	    $evil++;
	} elsif ($race{$faction} eq "troll") {
	    $troll++;
	    $evil++;
	} elsif ($race{$faction} eq "orc") {
	    $orc++;
	    $evil++;
	} elsif ($race{$faction} eq "goblin") {
	    $goblin++;
	    $evil++;
	} elsif ($race{$faction} eq "gnoll") {
	    $gnoll++;
	    $evil++;
	} elsif ($race{$faction} eq "ogre male") {
	    $ogre++;
	    $evil++;
	} elsif ($race{$faction} eq "minotaur") {
	    $minotaur++;
	    $evil++;
	} elsif ($race{$faction} eq "uruk") {
	    $uruk++;
	    $evil++;
	} elsif ($race{$faction} eq "drow female") {
	    $drow++;
	    $evil++;
	} elsif ($race{$faction} eq "duergar") {
	    $duergar++;
	    $evil++;
	}
    }
    #find its profession
    if (/^Profession:\s(.*)/){
	$profession{$faction} = $1;
	if ($profession{$faction} eq "warrior"){
	    $warrior++;
	} elsif ($profession{$faction} eq "seaman"){
	    $seaman++;
	} elsif ($profession{$faction} eq "merchant"){
	    $merchant++;
	} elsif ($profession{$faction} eq "artificer"){
	    $artificer++;
	} elsif ($profession{$faction} eq "noble"){
	    $noble++;
	} elsif ($profession{$faction} eq "officer"){
	    $officer++;
	} elsif ($profession{$faction} eq "mage"){
	    $mage++;
	} elsif ($profession{$faction} eq "assassin"){
	    $assassin++;
	}
    }
    #find its starting city
    if (/^StartingPlace:\s(.*)/){
	($dummy, $startplace) = split(/ /,$_);
	if ($startplace eq "3"){
	    $blackkeepcity++;
	} elsif ($startplace eq "4"){
	    $ramishempire++;
	} elsif ($startplace eq "5"){
	    $amazonnation++;
	} elsif ($startplace eq "6"){
	    $silsalon++;
	} elsif ($startplace eq "7"){
	    $republicofpelisham++;
	} elsif ($startplace eq "8"){
	    $halfingdemocracy++;
	} elsif ($startplace eq "9"){
	    $buccaneerfellowship++;
	} elsif ($startplace eq "10"){
	    $vikingconfederacy++;
	} elsif ($startplace eq "11"){
	    $duergarkingdom++;
	} elsif ($startplace eq "12"){
	    $dwarvenkingdom++;
	} elsif ($startplace eq "13"){
	    $drowmatriarchy++;
	} elsif ($startplace eq "14"){
	    $darkmankingdom++;
	}
    }
}
close(NPIN);

#parsing the players.in file
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    #find its race
    if (/^Rasse:\s(.*)/){
	$race = $1;
	$race =~ s/\"//g;
	if ($race eq "human") {
	    $human++;
	    $good++;
	} elsif ($race eq "barbarian") {
	    $barbarian++;
	    $good++;
	} elsif ($race eq "tribesman") {
	    $tribesman++;
	    $good++;
	} elsif ($race eq "viking") {
	    $viking++;
	    $good++;
	} elsif ($race eq "highlander") {
	    $highlander++;
	    $good++;
	} elsif ($race eq "ffolk") {
	    $ffolk++;
	    $good++;
	} elsif ($race eq "dervish") {
	    $dervish++;
	    $good++;
	} elsif ($race eq "cajun man") {
	    $cajunman++;
	    $good++;
	} elsif ($race eq "nomad") {
	    $nomad++;
	    $good++;
	} elsif ($race eq "clansman") {
	    $clansman++;
	    $good++;
	} elsif ($race eq "lowlander") {
	    $lowlander++;
	    $good++;
	} elsif ($race eq "darkman") {
	    $darkman++;
	    $good++;
	} elsif ($race eq "imperial") {
	    $imperial++;
	    $good++;
	} elsif ($race eq "elf") {
	    $elf++;
	    $good++;
	} elsif ($race eq "gnome") {
	    $good++;
	    $gnome++;
	} elsif ($race eq "halfing") {
	    $halfing++;
	    $good++;
	} elsif ($race eq "halfelf") {
	    $halfelf++;
	    $good++;
	} elsif ($race eq "dwarf") {
	    $dwarf++;
	    $good++;
	} elsif ($race eq "lizardman") {
	    $lizardman++;
	    $good++;
	} elsif ($race eq "buccaneer") {
	    $buccaneer++;
	    $evil++;
	} elsif ($race eq "republican") {
	    $republican++;
	    $evil++;
	} elsif ($race eq "headhunter") {
	    $headhunter++;
	    $evil++;
	} elsif ($race eq "sandling") {
	    $sandling++;
	    $evil++;
	} elsif ($race eq "serpent people") {
	    $serpentpeople++;
	    $evil++;
	} elsif ($race eq "black keep leader") {
	    $blackkeep++;
	    $evil++;
	} elsif ($race eq "amazon") {
	    $amazon++;
	    $evil++;
	} elsif ($race eq "hobgoblin") {
	    $hobgoblin++;
	    $evil++;
	} elsif ($race eq "troll") {
	    $troll++;
	    $evil++;
	} elsif ($race eq "orc") {
	    $orc++;
	    $evil++;
	} elsif ($race eq "goblin") {
	    $goblin++;
	    $evil++;
	} elsif ($race eq "gnoll") {
	    $gnoll++;
	    $evil++;
	} elsif ($race eq "ogre male") {
	    $ogre++;
	    $evil++;
	} elsif ($race eq "minotaur") {
	    $minotaur++;
	    $evil++;
	} elsif ($race eq "uruk") {
	    $uruk++;
	    $evil++;
	} elsif ($race eq "drow female") {
	    $drow++;
	    $evil++;
	} elsif ($race eq "duergar") {
	    $duergar++;
	    $evil++;
	}
    }
    #find its profession
    if (/^Profession:\s(.*)/){
	$profession = $1;
	if ($profession eq "warrior"){
	    $warrior++;
	} elsif ($profession eq "seaman"){
	    $seaman++;
	} elsif ($profession eq "merchant"){
	    $merchant++;
	} elsif ($profession eq "artificer"){
	    $artificer++;
	} elsif ($profession eq "noble"){
	    $noble++;
	} elsif ($profession eq "officer"){
	    $officer++;
	} elsif ($profession eq "mage"){
	    $mage++;
	} elsif ($profession eq "assassin"){
	    $assassin++;
	}
    }
    #find its starting city
    if (/^StartingPlace:\s(.*)/){
	($dummy, $startplace) = split(/ /,$_);
	if ($startplace eq "3"){
	    $blackkeepcity++;
	} elsif ($startplace eq "4"){
	    $ramishempire++;
	} elsif ($startplace eq "5"){
	    $amazonnation++;
	} elsif ($startplace eq "6"){
	    $silsalon++;
	} elsif ($startplace eq "7"){
	    $republicofpelisham++;
	} elsif ($startplace eq "8"){
	    $halfingdemocracy++;
	} elsif ($startplace eq "9"){
	    $buccaneerfellowship++;
	} elsif ($startplace eq "10"){
	    $vikingconfederacy++;
	} elsif ($startplace eq "11"){
	    $duergarkingdom++;
	} elsif ($startplace eq "12"){
	    $dwarvenkingdom++;
	} elsif ($startplace eq "13"){
	    $drowmatriarchy++;
	} elsif ($startplace eq "14"){
	    $darkmankingdom++;
	}
    }
}
close(PIN);


foreach $Faction (@factions){
    #writing a table row for each faction
    print "<tr><td>".$factionnumber{$Faction}."</td><td>".$name{$Faction}."</td><td>".$email2{$Faction}."</td><td>".$race{$Faction}."</td></tr>\n";
}
print "</table>";

    print "<p><table border=0>\n";
    print "<tr><td rowspan=2 valign=top>\n";

#starting cities

print "<p>Here you can see how many faction are citizens of each starting city (including the new signups):\n";
print "<p><table border=1>\n";
print "<tr><td>starting city</td><td>number of factions</td></tr>\n";
print "<tr><td>Black Keep</td><td>".$blackkeepcity."</td></tr>\n";
print "<tr><td>Ram</td><td>".$ramishempire."</td></tr>\n";
print "<tr><td>Xanthara</td><td>".$amazonnation."</td></tr>\n";
print "<tr><td>Silsalon</td><td>".$silsalon."</td></tr>\n";
print "<tr><td>Pel'Hash</td><td>".$republicofpelisham."</td></tr>\n";
print "<tr><td>Ashhaim</td><td>".$halfingdemocracy."</td></tr>\n";
print "<tr><td>Bucc Den</td><td>".$buccaneerfellowship."</td></tr>\n";
print "<tr><td>Port Sass</td><td>".$vikingconfederacy."</td></tr>\n";
print "<tr><td>Vaulthall</td><td>".$duergarkingdom."</td></tr>\n";
print "<tr><td>Minehome</td><td>".$dwarvenkingdom."</td></tr>\n";
print "<tr><td>Parth'Tir'Dallon</td><td>".$drowmatriarchy."</td></tr>\n";
print "<tr><td>Char'ar'ton</td><td>".$darkmankingdom."</td></tr>\n";
print "<tr><td>total:</td><td>".($blackkeepcity + $ramishempire + $amazonnation + $silsalon + $republicofpelisham + $halfingdemocracy + $buccaneerfellowship + $vikingconfederacy + $duergarkingdom + $dwarvenkingdom + $drowmatriarchy + $darkmankingdom)."</td></tr>\n";
print "</table>\n";

    print "</td><td valign=top>\n";

#printing profession quota

print "<p>Here you can see aproximately how many of each profession are playing currently (including the new signups):\n";
print "<p><table border=1>\n";
print "<tr><td>profession</td><td>number of factions</td></tr>\n";
print "<tr><td>warrior</td><td>";
if ($warrior == 0){
    print "none";
} elsif ($warrior > 36){
    print "dozens";
} elsif ($warrior > 24){
    print "lots";
} elsif ($warrior > 16){
    print "many";
} elsif ($warrior > 12){
    print "a dozen";
} elsif ($warrior > 8){
    print "some";
} elsif ($warrior > 4){
    print "half a dozen";
} elsif ($warrior > 0){
    print "few";
}
print "</td></tr>\n";
print "<tr><td>officer</td><td>";
if ($officer == 0){
    print "none";
} elsif ($officer > 36){
    print "dozens";
} elsif ($officer > 24){
    print "lots";
} elsif ($officer > 16){
    print "many";
} elsif ($officer > 12){
    print "a dozen";
} elsif ($officer > 8){
    print "some";
} elsif ($officer > 4){
    print "half a dozen";
} elsif ($officer > 0){
    print "few";
}
print "</td></tr>\n";
print "<tr><td>merchant</td><td>";
if ($merchant == 0){
    print "none";
} elsif ($merchant > 36){
    print "dozens";
} elsif ($merchant > 24){
    print "lots";
} elsif ($merchant > 16){
    print "many";
} elsif ($merchant > 12){
    print "a dozen";
} elsif ($merchant > 8){
    print "some";
} elsif ($merchant > 4){
    print "half a dozen";
} elsif ($merchant > 0){
    print "few";
}
print "</td></tr>\n";
print "<tr><td>artificer</td><td>";
if ($artificer == 0){
    print "none";
} elsif ($artificer > 36){
    print "dozens";
} elsif ($artificer > 24){
    print "lots";
} elsif ($artificer > 16){
    print "many";
} elsif ($artificer > 12){
    print "a dozen";
} elsif ($artificer > 8){
    print "some";
} elsif ($artificer > 4){
    print "half a dozen";
} elsif ($artificer > 0){
    print "few";
}
print "</td></tr>\n";
print "<tr><td>noble</td><td>";
if ($noble == 0){
    print "none";
} elsif ($noble > 36){
    print "dozens";
} elsif ($noble > 24){
    print "lots";
} elsif ($noble > 16){
    print "many";
} elsif ($noble > 12){
    print "a dozen";
} elsif ($noble > 8){
    print "some";
} elsif ($noble > 4){
    print "half a dozen";
} elsif ($noble > 0){
    print "few";
}
print "</td></tr>\n";
print "<tr><td>seaman</td><td>";
if ($seaman == 0){
    print "none";
} elsif ($seaman > 36){
    print "dozens";
} elsif ($seaman > 24){
    print "lots";
} elsif ($seaman > 16){
    print "many";
} elsif ($seaman > 12){
    print "a dozen";
} elsif ($seaman > 8){
    print "some";
} elsif ($seaman > 4){
    print "half a dozen";
} elsif ($seaman > 0){
    print "few";
}
print "</td></tr>\n";
print "<tr><td>mage</td><td>";
if ($mage == 0){
    print "none";
} elsif ($mage > 36){
    print "dozens";
} elsif ($mage > 24){
    print "lots";
} elsif ($mage > 16){
    print "many";
} elsif ($mage > 12){
    print "a dozen";
} elsif ($mage > 8){
    print "some";
} elsif ($mage > 4){
    print "half a dozen";
} elsif ($mage > 0){
    print "few";
}
print "</td></tr>\n";
print "<tr><td>assassin</td><td>";
if ($assassin == 0){
    print "none";
} elsif ($assassin > 36){
    print "dozens";
} elsif ($assassin > 24){
    print "lots";
} elsif ($assassin > 16){
    print "many";
} elsif ($assassin > 12){
    print "a dozen";
} elsif ($assassin > 8){
    print "some";
} elsif ($assassin > 4){
    print "half a dozen";
} elsif ($assassin > 0){
    print "few";
}
print "</td></tr>\n";
print "</table>\n";
print "<p>none < few < half a dozen < some < a dozen < many < lots < dozens\n";

    print "</td></tr></table>\n";

#printing race quota
$total = $viking + $barbarian + $tribesman + $highlander + $clansman + $ffolk + $dervish + $nomad + $cajunman + $lizardman + $lowlander + $halfing + $imperial + $elf + $halfelf + $human + $gnome + $dwarf + $darkman + $buccaneer + $republican + $headhunter + $sandling + $serpentpeople + $blackkeep + $amazon + $hobgoblin + $goblin + $troll + $gnoll + $ogre + $uruk + $orc + $minotaur + $drow + $duergar;
print "<p>Here you can see how many factions of each race are playing currently (including the new signups):\n";
print "<p><table border=1>\n";
print "<tr><td>good race</td><td>number of factions</td><td>evil race</td><td>number of factions</td></tr>\n";
print "<tr><td>human</td><td>".$human."</td><td>hobgoblin and goblin</td><td>".($hobgoblin + $goblin)."</td></tr>\n";
print "<tr><td>viking</td><td>".$viking."</td><td>buccaneer</td><td>".$buccaneer."</td></tr>\n";
print "<tr><td>barbarian</td><td>".$barbarian."</td><td>troll and gnoll</td><td>".($troll + $gnoll)."</td></tr>\n";
print "<tr><td>tribesman</td><td>".$tribesman."</td><td>headhunter</td><td>".$headhunter."</td></tr>\n";
print "<tr><td>highlander and clansman</td><td>".($highlander + $clansman)."</td><td>uruk and orc</td><td>".($uruk + $orc)."</td></tr>\n";
print "<tr><td>ffolk</td><td>".$ffolk."</td><td>ogre</td><td>".$ogre."</td></tr>\n";
print "<tr><td>dervish and nomad</td><td>".($dervish + $nomad)."</td><td>sandling</td><td>".$sandling."</td></tr>\n";
print "<tr><td>cajun man and lizardman</td><td>".($cajunman + $lizardman)."</td><td>serpent people</td><td>".$serpentpeople."</td></tr>\n";
print "<tr><td>lowlander and halfing</td><td>".($lowlander + $halfing)."</td><td>republican</td><td>".$republican."</td></tr>\n";
print "<tr><td>imperial</td><td>".$imperial."</td><td>black keep</td><td>".$blackkeep."</td></tr>\n";
print "<tr><td>elf and halfelf</td><td>".($elf + $halfelf)."</td><td>amazon</td><td>".$amazon."</td></tr>\n";
print "<tr><td>gnome</td><td>".$gnome."</td><td>drow</td><td>".$drow."</td></tr>\n";
print "<tr><td>dwarf</td><td>".$dwarf."</td><td>duergar</td><td>".$duergar."</td></tr>\n";
print "<tr><td>darkman</td><td>".$darkman."</td><td>minotaur</td><td>".$minotaur."</td></tr>\n";
print "<tr><td>good</td><td>".$good."</td><td>evil</td><td>".$evil."</td></tr>\n";
print "<tr><td colspan=2>total</td><td colspan=2>".$total."</td></tr>\n";
print "</table>\n";


#completing everything
print "</body></html>\n";






