#!/usr/bin/perl
# script to choose rewards
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

#specify the relative path to the players.in file
$pin = "/home/tarmellion/atlantis/tarmellion/players.in";
$noname = 0;

#prepare options for races and skills
$el = "<option>buccaneer<option>republican<option>headhunter<option>sandling<option>amazon<option>serpent people<option>black keep<option>uruk<option>hobgoblin<option>troll<option>ogre<option>minotaur<option>drow<option>duergar";
$en = "<option>buccaneer<option>republican<option>headhunter<option>sandling<option>amazon<option>serpent people<option>black keep<option>orc<option>goblin<option>gnoll<option>ogre<option>minotaur<option>drow<option>duergar";
$gl = "<option>human<option>barbarian<option>tribesman<option>viking<option>ffolk<option>highlander<option>dervish<option>imperial<option>darkman<option>elf<option>gnome<option>lowlander<option>dwarf<option>cajun folk";
$gn = "<option>human<option>barbarian<option>tribesman<option>viking<option>ffolk<option>clansman<option>nomad<option>imperial<option>darkman<option>halfelf<option>gnome<option>halfing<option>dwarf<option>lizardman";
$fight = "<option>combat<option>archery<option>crossbow<option>riding<option>healing";
$ressource = "<option>mining<option>lumberjack<option>quarrying<option>hunting<option>fishing<option>herb lore<option>entertainment<option>farming<option>ranching<option>sailing<option>animal training";
$product = "<option>smithing<option>toolmaking<option>building<option>shipbuilding<option>cooking";

$q = new CGI;

#parsing the players.in file
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    #find the factions
    if (/^Faction:\s(\d*)/){
	$Faction = $1;
	next;
    }
    #find old times rewards
    if (/^Timespunkte:\s(\d*)/){
      ($dummy,$reward{$Faction}) = split(/\s/,$_);
      next;
    }
}
close(PIN);

$addon = $q->path_info();
$addon =~ s|^/||;

($faction, $type, $align) = split(/_/,$addon);

$rewardpoints = $reward{$faction};

#preparing the html page
print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE>\n";
print "<base target=Datei></HEAD>\n";
print "<body bgcolor=white>\n";
print "<h2>Times Reward Form for Faction $faction</h2><p>\n";
print "<p>You have $rewardpoints reward points left.\n";

if ($type eq "animal") {
  $laber1 = "<p>Due to the immense Karma your faction leader has as represented by your reward points, some creatures want to follow your cause.\n<p>In the following form, you can specify which and how many creatures will come, but each will cost you reward points as listed with the creature and all of them another 2 points for delivery.\n<p>\n";
  if ($align eq "evil"){
    $laber2 = "<tr><td>amount</td><td colspan=2>creature</td><td>reward points</td></tr>\n<tr><td><input name=amountPONY size=4><input name=costPONY type=hidden value=3 size=4></td><td colspan=2>pony</td><td>3</td></tr>\n<tr><td><input name=amountMWOL size=4><input name=costMWOL type=hidden value=3 size=4></td><td colspan=2>giant wolf</td><td>3</td></tr>\n<tr><td><input name=amountMSPI size=4><input name=costMSPI type=hidden value=3 size=4></td><td colspan=2>giant spider</td><td>3</td></tr>\n<tr><td><input name=amountHORS size=4><input name=costHORS type=hidden value=6 size=4></td><td colspan=2>horse</td><td>6</td></tr>\n<tr><td><input name=amountCAME size=4><input name=costCAME type=hidden value=6 size=4></td><td colspan=2>camel</td><td>6</td></tr>\n<tr><td><input name=amountMLIZ size=4><input name=costMLIZ type=hidden value=6 size=4></td><td colspan=2>giant lizard</td><td>6</td></tr>\n<tr><td><input name=amountMEAG size=4><input name=costMEAG type=hidden value=8 size=4></td><td colspan=2>giant eagle</td><td>8</td></tr>\n<tr><td><input name=amountMBAT size=4><input name=costMBAT type=hidden value=8 size=4></td><td colspan=2>giant bat</td><td>8</td></tr>\n<tr><td><input name=amountELEP size=4><input name=costELEP type=hidden value=15 size=4></td><td colspan=2>elephant</td><td>15</td></tr>\n<tr><td><input name=amountMAMM size=4><input name=costMAMM type=hidden value=15 size=4></td><td colspan=2>mammoth</td><td>15</td></tr>\n<tr><td><input name=amountMOLE size=4><input name=costMOLE type=hidden value=15 size=4></td><td colspan=2>giant mole</td><td>15</td></tr>\n<tr><td><input name=amountWING size=4><input name=costWING type=hidden value=30 size=4></td><td colspan=2>winged horse</td><td>30</td></tr>\n<tr><td><input name=amountMFLI size=4><input name=costMFLI type=hidden value=30 size=4></td><td colspan=2>flying lizard</td><td>30</td></tr>\n<tr><td><input name=amountWARG size=4><input name=costWARG type=hidden value=15 size=4></td><td colspan=2>war dog</td><td>15</td></tr>\n<tr><td><input name=amountWHOR size=4><input name=costWHOR type=hidden value=20 size=4></td><td colspan=2>warhorse</td><td>20</td></tr>\n<tr><td><input name=amountWLIZ size=4><input name=costWLIZ type=hidden value=20 size=4></td><td colspan=2>warlizard</td><td>20</td></tr>\n<tr><td><input name=amountHARY size=4><input name=costHARY type=hidden value=25 size=4></td><td colspan=2>harpy</td><td>25</td></tr>\n<tr><td><input name=amountGCEN size=4><input name=costGCEN type=hidden value=25 size=4></td><td colspan=2>giant centipede</td><td>25</td></tr>\n<tr><td><input name=amountCHIM size=4><input name=costCHIM type=hidden value=35 size=4></td><td colspan=2>chimera</td><td>35</td></tr>\n<tr><td><input name=amountWMAM size=4><input name=costWMAM type=hidden value=35 size=4></td><td colspan=2>war mammoth</td><td>35</td></tr>\n<tr><td><input name=amountWALF size=4><input name=costWALF type=hidden value=35 size=4></td><td colspan=2>walking fortress</td><td>35</td></tr>\n<tr><td><input name=amountFLIZ size=4><input name=costFLIZ type=hidden value=50 size=4></td><td colspan=2>fire lizard</td><td>50</td></tr>\n";
  } else {
    $laber2 = "<tr><td>amount</td><td colspan=2>creature</td><td>reward points</td></tr>\n<tr><td><input name=amountPONY size=4><input name=costPONY type=hidden value=3 size=4></td><td colspan=2>pony</td><td>3</td></tr>\n<tr><td><input name=amountMWOL size=4><input name=costMWOL type=hidden value=3 size=4></td><td colspan=2>giant wolf</td><td>3</td></tr>\n<tr><td><input name=amountMSPI size=4><input name=costMSPI type=hidden value=3 size=4></td><td colspan=2>giant spider</td><td>3</td></tr>\n<tr><td><input name=amountHORS size=4><input name=costHORS type=hidden value=6 size=4></td><td colspan=2>horse</td><td>6</td></tr>\n<tr><td><input name=amountCAME size=4><input name=costCAME type=hidden value=6 size=4></td><td colspan=2>camel</td><td>6</td></tr>\n<tr><td><input name=amountMLIZ size=4><input name=costMLIZ type=hidden value=6 size=4></td><td colspan=2>giant lizard</td><td>6</td></tr>\n<tr><td><input name=amountMEAG size=4><input name=costMEAG type=hidden value=8 size=4></td><td colspan=2>giant eagle</td><td>8</td></tr>\n<tr><td><input name=amountMBAT size=4><input name=costMBAT type=hidden value=8 size=4></td><td colspan=2>giant bat</td><td>8</td></tr>\n<tr><td><input name=amountELEP size=4><input name=costELEP type=hidden value=15 size=4></td><td colspan=2>elephant</td><td>15</td></tr>\n<tr><td><input name=amountMAMM size=4><input name=costMAMM type=hidden value=15 size=4></td><td colspan=2>mammoth</td><td>15</td></tr>\n<tr><td><input name=amountMOLE size=4><input name=costMOLE type=hidden value=15 size=4></td><td colspan=2>giant mole</td><td>15</td></tr>\n<tr><td><input name=amountWING size=4><input name=costWING type=hidden value=30 size=4></td><td colspan=2>winged horse</td><td>30</td></tr>\n<tr><td><input name=amountMFLI size=4><input name=costMFLI type=hidden value=30 size=4></td><td colspan=2>flying lizard</td><td>30</td></tr>\n<tr><td><input name=amountHUND size=4><input name=costHUND type=hidden value=15 size=4></td><td colspan=2>hunting dog</td><td>15</td></tr>\n<tr><td><input name=amountCHAR size=4><input name=costCHAR type=hidden value=20 size=4></td><td colspan=2>charger</td><td>20</td></tr>\n<tr><td><input name=amountBLIZ size=4><input name=costBLIZ type=hidden value=20 size=4></td><td colspan=2>battlelizard</td><td>20</td></tr>\n<tr><td><input name=amountUNIC size=4><input name=costUNIC type=hidden value=25 size=4></td><td colspan=2>unicorn</td><td>25</td></tr>\n<tr><td><input name=amountDRAK size=4><input name=costDRAK type=hidden value=25 size=4></td><td colspan=2>drake</td><td>25</td></tr>\n<tr><td><input name=amountGRIF size=4><input name=costGRIF type=hidden value=35 size=4></td><td colspan=2>griffon</td><td>35</td></tr>\n<tr><td><input name=amountWELE size=4><input name=costWELE type=hidden value=35 size=4></td><td colspan=2>war elephant</td><td>35</td></tr>\n<tr><td><input name=amountBEHE size=4><input name=costBEHE type=hidden value=35 size=4></td><td colspan=2>behemoth</td><td>35</td></tr>\n<tr><td><input name=amountFDRA size=4><input name=costFDRA type=hidden value=50 size=4></td><td colspan=2>fire drake</td><td>50</td></tr>\n";
  }
} elsif ($type eq "elitesoldier") {
  $laber1 = "<p>Due to the immense Karma your faction leader has as represented by your reward points, some soldiers can be swayed to follow your cause.\n<p>In the following form, you can specify which race the new men should have and in which skill they should be provicient. You can also specify how many soldiers will come, but each will cost you 30 reward points.\n<p>";
  $laber2 = "<tr><td>&nbsp;</td><td>amount</td><td>race</td><td>skill</td></tr>\n<tr><td>races:</td><td><input name=amount size=4></td>\n";
  $laber2 .= "<td><select name=racenew size=1>";
  if ($align eq "evil") {
    $laber2 .= $el;
  } else {
    $laber2 .= $gl;
  }
  $laber2 .= "</select></td>\n<td><select name=skillnew size=1>".$fight."</select></td></tr>\n";
} elsif ($type eq "leader10") {
  $laber1 = "<p>Due to the immense Karma your faction leader has as represented by your reward points, some skilled followers can be swayed to follow your cause.\n<p>In the following form, you can specify which race the new men should have and in which skill they should be provicient. You can also specify how many followers will come, but each will cost you 25 reward points.\n<p>\n";
  $laber2 = "<tr><td>&nbsp;</td><td>amount</td><td>race</td><td>skill</td></tr>\n<tr><td>races:</td><td><input name=amount size=4></td>\n";
  $laber2 .= "<td><select name=racenew size=1>";
  if ($align eq "evil") {
    $laber2 .= $el;
  } else {
    $laber2 .= $gl;
  }
  $laber2 .= "</select></td>\n<td><select name=skillnew size=1>".$ressource."</select></td></tr>\n";

} elsif ($type eq "leader20") {
  $laber1 = "<p>Due to the immense Karma your faction leader has as represented by your reward points, some skilled followers can be swayed to follow your cause.\n<p>In the following form, you can specify which race the new men should have and in which skill they should be provicient. You can also specify how many followers will come, but each will cost you 28 reward points.\n<p>\n";
  $laber2 = "<tr><td>&nbsp;</td><td>amount</td><td>race</td><td>skill</td></tr>\n<tr><td>races:</td><td><input name=amount size=4></td>\n";
  $laber2 .= "<td><select name=racenew size=1>";
  if ($align eq "evil") {
    $laber2 .= $el;
  } else {
    $laber2 .= $gl;
  }
  $laber2 .= "</select></td>\n<td><select name=skillnew size=1>".$product."</select></td></tr>\n";

} elsif ($type eq "loc") {
$laber1 = "<p>The starting location is the hex where the starting character of the faction first saw the light of Tarmellion. Each further times reward will arrive in that hex too. To change this location fill in the following form and after spending 2 reward points, all new times rewards will arrive in the new location.\n<p>To specify the new hex where your rewards should appear fill in \"row\" with the first number of your hex coordinates, \"line\" with the second number and \"level\" with 1 for surface, 2 for underworld, 3 for deep underworld, 4 for very deep underworld, 5 for underdeeps, 6 for deep underdeeps and 7 for very deep underdeeps.\n";
$laber2 = "<tr><td>action</td><td>row</td><td>line</td><td>level</td></tr>\n<tr><td>change location<input type=hidden name=type value=loc></td><td><input name=xcoor size=2></td><td><input name=ycoor size=2></td><td><input name=level size=2></td></tr>\n";
$noname = 1;

} elsif ($type eq "mage") {
$laber1 = "<p>Due to the immense Karma your faction leader has as represented by your reward points, a mighty Pattern mage will join your faction.\n<p>In the following form, you can specify which race he should have. He will cost you 100 reward points.\n<p>\n";
if ($align eq "evil") {

} else {
  $laber2 = "<tr><td colspan=4>mage</td></tr>\n<tr>\n<td colspan=4><select name=mage size=1>\n<option>human fire sorcerer\n<option>viking wind sorcerer\n<option>viking sea wizard\n<option>dervish fire sorcerer\n<option>dervish wind sorcerer\n<option>ffolk ice sorcerer\n<option>ffolk weather wizard\n<option>cajun folk ice sorcerer\n<option>lowlander fire sorcerer\n<option>lowlander weather wizard\n<option>gnome fire sorcerer\n<option>gnome ice sorcerer\n<option>barbarian priest\n<option>barbarian enchanter\n<option>tribesman priest\n<option>tribesman illusionist\n<option>tribesman inquisitor\n<option>darkman priest\n<option>darkman enchanter\n<option>highlander runesmith\n<option>imperial gate magician\n<option>elf druid\n<option>elf wolf master\n<option>elf bird master\n<option>dwarf runesmith\n</select></td>\n";
}

} elsif ($type eq "money") {
$laber1 = "<p>The Times reward points can be exchanged for unclaimed money.\n<p>Just fill in how many points should be exchanged, but make sure that you do not exceed your reward points account.\n<p>\n";
$laber2 = "<tr><td colspan=2>action</td><td colspan=2>reward points</td></tr>\n<tr><td colspan=2>make unclaimed silver out of<input type=hidden name=type value=money></td><td colspan=2><input name=money size=4></td></tr>\n";
$noname = 1;

} elsif ($type eq "soldier") {
$laber1 = "<p>Due to the immense Karma your faction leader has as represented by your reward points, some soldiers can be swayed to follow your cause.\n<p>In the following form, you can specify which race the new men should have and in which skill they should be provicient. You can also specify how many soldiers will come, but each will cost you 12 reward points.\n<p>\n";
  $laber2 = "<tr><td>&nbsp;</td><td>amount</td><td>race</td><td>skill</td></tr>\n<tr><td>races:</td><td><input name=amount size=4></td>\n";
  $laber2 .= "<td><select name=racenew size=1>";
  if ($align eq "evil") {
    $laber2 .= $en;
  } else {
    $laber2 .= $gn;
  }
  $laber2 .= "</select></td>\n<td><select name=skillnew size=1>".$fight."</select></td></tr>\n";

} elsif ($type eq "specialist") {
$laber1 = "<p>Due to the immense Karma your faction leader has as represented by your reward points, some specialists can be talked into following your cause.\n<p>In the following form, you can specify which race the new men should have and in which skill they should be provicient. You can also specify how many specialists will come, but each will cost you as many reward points as listed with the skill.\n<p>\n";
  $laber2 = "<tr><td>&nbsp;</td><td>amount</td><td>race</td><td>skill</td></tr>\n<tr><td>races:</td><td><input name=amount size=4></td>\n";
  $laber2 .= "<td><select name=racenew size=1>";
  if ($align eq "evil") {
    $laber2 .= $el;
  } else {
    $laber2 .= $gl;
  }
  $laber2 .= "</select></td>\n<td><select name=skillnew size=1><option>tactics (80 RP)<option>observation (40 RP)<option>stealth (40 RP)<option>scouting (40 RP)</select></td></tr>\n";

} elsif ($type eq "unit10") {
$laber1 = "<p>Due to the immense Karma your faction leader has as represented by your reward points, some skilled followers can be swayed to follow your cause.\n<p>In the following form, you can specify which race the new men should have and in which skill they should be provicient. You can also specify how many followers will come, but each will cost you 10 reward points.\n<p>\n";
  $laber2 = "<tr><td>&nbsp;</td><td>amount</td><td>race</td><td>skill</td></tr>\n<tr><td>races:</td><td><input name=amount size=4></td>\n";
  $laber2 .= "<td><select name=racenew size=1>";
  if ($align eq "evil") {
    $laber2 .= $en;
  } else {
    $laber2 .= $gn;
  }
  $laber2 .= "</select></td>\n<td><select name=skillnew size=1>".$ressource."</select></td></tr>\n";

} elsif ($type eq "unit20") {
$laber1 = "<p>Due to the immense Karma your faction leader has as represented by your reward points, some skilled followers can be swayed to follow your cause.\n<p>In the following form, you can specify which race the new men should have and in which skill they should be provicient. You can also specify how many followers will come, but each will cost you 12 reward points.\n<p>\n";
  $laber2 = "<tr><td>&nbsp;</td><td>amount</td><td>race</td><td>skill</td></tr>\n<tr><td>races:</td><td><input name=amount size=4></td>\n";
  $laber2 .= "<td><select name=racenew size=1>";
  if ($align eq "evil") {
    $laber2 .= $en;
  } else {
    $laber2 .= $gn;
  }
  $laber2 .= "</select></td>\n<td><select name=skillnew size=1>".$product."</select></td></tr>\n";

} elsif ($type eq "weapon") {
$laber1 = "<p>Due to the immense Karma your faction leader has as represented by your reward points, several advanced arms and armours are available for you.\n<p>In the following form, you can specify what and how many weapons you want, but each will cost you reward points as listed with the item and all of them another 2 points for delivery.\n<p>\n";
$laber2 = "<tr><td>amount</td><td colspan=2>item</td><td>reward points</td></tr>\n<tr><td><input name=amountELXB size=4><input name=costELXB type=hidden value=15 size=4></td><td colspan=2>enchanted light crossbow</td><td>15</td></tr>\n<tr><td><input name=amountXBOW size=4><input name=costXBOW type=hidden value=10 size=4></td><td colspan=2>crossbow</td><td>10</td></tr>\n<tr><td><input name=amountHXBO size=4><input name=costHXBO type=hidden value=20 size=4></td><td colspan=2>heavy crossbow</td><td>20</td></tr>\n<tr><td><input name=amountESHB size=4><input name=costESHB type=hidden value=15 size=4></td><td colspan=2>enchanted shortbow</td><td>15</td></tr>\n<tr><td><input name=amountBOW size=4><input name=costBOW type=hidden value=10 size=4></td><td colspan=2>bow</td><td>10</td></tr>\n<tr><td><input name=amountLBOW size=4><input name=costLBOW type=hidden value=22 size=4></td><td colspan=2>longbow</td><td>22</td></tr>\n<tr><td><input name=amountESPE size=4><input name=costESPE type=hidden value=15 size=4></td><td colspan=2>enchanted spear</td><td>15</td></tr>\n<tr><td><input name=amountBSPE size=4><input name=costBSPE type=hidden value=10 size=4></td><td colspan=2>battle spear</td><td>10</td></tr>\n<tr><td><input name=amountWSPE size=4><input name=costWSPE type=hidden value=20 size=4></td><td colspan=2>war spear</td><td>20</td></tr>\n<tr><td><input name=amountEPIK size=4><input name=costEPIK type=hidden value=17 size=4></td><td colspan=2>enchanted pike</td><td>17</td></tr>\n<tr><td><input name=amountHALB size=4><input name=costHALB type=hidden value=12 size=4></td><td colspan=2>halberd</td><td>12</td></tr>\n<tr><td><input name=amountBARD size=4><input name=costBARD type=hidden value=24 size=4></td><td colspan=2>bardiche</td><td>24</td></tr>\n<tr><td><input name=amountELAN size=4><input name=costELAN type=hidden value=15 size=4></td><td colspan=2>enchanted lance</td><td>15</td></tr>\n<tr><td><input name=amountBLAN size=4><input name=costBLAN type=hidden value=10 size=4></td><td colspan=2>battle lance</td><td>10</td></tr>\n<tr><td><input name=amountWLAN size=4><input name=costWLAN type=hidden value=20 size=4></td><td colspan=2>war lance</td><td>20</td></tr>\n<tr><td><input name=amountEQUA size=4><input name=costEQUA type=hidden value=15 size=4></td><td colspan=2>enchanted quarterstaff</td><td>15</td></tr>\n<tr><td><input name=amountBSTA size=4><input name=costBSTA type=hidden value=10 size=4></td><td colspan=2>battle staff</td><td>10</td></tr>\n<tr><td><input name=amountWSTA size=4><input name=costWSTA type=hidden value=20 size=4></td><td colspan=2>war staff</td><td>20</td></tr>\n<tr><td><input name=amountECLU size=4><input name=costECLU type=hidden value=12 size=4></td><td colspan=2>enchanted club</td><td>12</td></tr>\n<tr><td><input name=amountEMAC size=4><input name=costEMAC type=hidden value=17 size=4></td><td colspan=2>enchanted mace</td><td>17</td></tr>\n<tr><td><input name=amountBMAC size=4><input name=costBMAC type=hidden value=12 size=4></td><td colspan=2>battle mace</td><td>12</td></tr>\n<tr><td><input name=amountWMAC size=4><input name=costWMAC type=hidden value=24 size=4></td><td colspan=2>war mace</td><td>24</td></tr>\n<tr><td><input name=amountEMST size=4><input name=costEMST type=hidden value=15 size=4></td><td colspan=2>enchanted morningstar</td><td>15</td></tr>\n<tr><td><input name=amountTSTA size=4><input name=costTSTA type=hidden value=10 size=4></td><td colspan=2>tristar</td><td>10</td></tr>\n<tr><td><input name=amountESTA size=4><input name=costESTA type=hidden value=20 size=4></td><td colspan=2>evening star</td><td>20</td></tr>\n<tr><td><input name=amountEBHA size=4><input name=costEBHA type=hidden value=17 size=4></td><td colspan=2>enchanted battle hammer</td><td>17</td></tr>\n<tr><td><input name=amountSKUL size=4><input name=costSKUL type=hidden value=12 size=4></td><td colspan=2>skullcrusher</td><td>12</td></tr>\n<tr><td><input name=amountWHAM size=4><input name=costWHAM type=hidden value=24 size=4></td><td colspan=2>war hammer</td><td>24</td></tr>\n<tr><td><input name=amountEBAX size=4><input name=costEBAX type=hidden value=17 size=4></td><td colspan=2>enchanted battle axe</td><td>17</td></tr>\n<tr><td><input name=amountCLEA size=4><input name=costCLEA type=hidden value=12 size=4></td><td colspan=2>cleaver</td><td>12</td></tr>\n<tr><td><input name=amountWAXE size=4><input name=costWAXE type=hidden value=24 size=4></td><td colspan=2>war axe</td><td>24</td></tr>\n<tr><td><input name=amountEDAG size=4><input name=costEDAG type=hidden value=10 size=4></td><td colspan=2>enchanted dagger</td><td>10</td></tr>\n<tr><td><input name=amountEPDA size=4><input name=costEPDA type=hidden value=10 size=4></td><td colspan=2>enchanted parrying dagger</td><td>10</td></tr>\n<tr><td><input name=amountMDAG size=4><input name=costMDAG type=hidden value=7 size=4></td><td colspan=2>mithril dagger</td><td>7</td></tr>\n<tr><td><input name=amountADAG size=4><input name=costADAG type=hidden value=13 size=4></td><td colspan=2>admantium dagger</td><td>13</td></tr>\n<tr><td><input name=amountESAB size=4><input name=costESAB type=hidden value=17 size=4></td><td colspan=2>enchanted sabre</td><td>17</td></tr>\n<tr><td><input name=amountEPEE size=4><input name=costEPEE type=hidden value=12 size=4></td><td colspan=2>epee</td><td>12</td></tr>\n<tr><td><input name=amountFOIL size=4><input name=costFOIL type=hidden value=24 size=4></td><td colspan=2>foil</td><td>24</td></tr>\n<tr><td><input name=amountESWO size=4><input name=costESWO type=hidden value=15 size=4></td><td colspan=2>enchanted sword</td><td>15</td></tr>\n<tr><td><input name=amountEBSW size=4><input name=costEBSW type=hidden value=17 size=4></td><td colspan=2>enchanted broadsword</td><td>17</td></tr>\n<tr><td><input name=amountELSW size=4><input name=costELSW type=hidden value=22 size=4></td><td colspan=2>enchanted longsword</td><td>22</td></tr>\n<tr><td><input name=amountMSWO size=4><input name=costMSWO type=hidden value=10 size=4></td><td colspan=2>mithril sword</td><td>10</td></tr>\n<tr><td><input name=amountMBSW size=4><input name=costMBSW type=hidden value=12 size=4></td><td colspan=2>mithril broadsword</td><td>12</td></tr>\n<tr><td><input name=amountMLSW size=4><input name=costMLSW type=hidden value=15 size=4></td><td colspan=2>mithril longsword</td><td>15</td></tr>\n<tr><td><input name=amountASWO size=4><input name=costASWO type=hidden value=20 size=4></td><td colspan=2>admantium sword</td><td>20</td></tr>\n<tr><td><input name=amountABSW size=4><input name=costABSW type=hidden value=24 size=4></td><td colspan=2>admantium broadsword</td><td>24</td></tr>\n<tr><td><input name=amountALSW size=4><input name=costALSW type=hidden value=30 size=4></td><td colspan=2>admantium longsword</td><td>30</td></tr>\n<tr><td><input name=amountECLA size=4><input name=costECLA type=hidden value=10 size=4></td><td colspan=2>enchanted cloth armor</td><td>10</td></tr>\n<tr><td><input name=amountELAR size=4><input name=costELAR type=hidden value=15 size=4></td><td colspan=2>enchanted leather armor</td><td>15</td></tr>\n<tr><td><input name=amountECAR size=4><input name=costECAR type=hidden value=20 size=4></td><td colspan=2>enchanted chain armor</td><td>20</td></tr>\n<tr><td><input name=amountESCA size=4><input name=costESCA type=hidden value=25 size=4></td><td colspan=2>enchanted scale armor</td><td>25</td></tr>\n<tr><td><input name=amountEPLA size=4><input name=costEPLA type=hidden value=30 size=4></td><td colspan=2>enchanted plate armor</td><td>30</td></tr>\n<tr><td><input name=amountBPLA size=4><input name=costBPLA type=hidden value=10 size=4></td><td colspan=2>barbarian plate armor</td><td>10</td></tr>\n<tr><td><input name=amountMCAR size=4><input name=costMCAR type=hidden value=16 size=4></td><td colspan=2>mithril chain armor</td><td>16</td></tr>\n<tr><td><input name=amountMSCA size=4><input name=costMSCA type=hidden value=20 size=4></td><td colspan=2>mithril scale armor</td><td>20</td></tr>\n<tr><td><input name=amountMPLA size=4><input name=costMPLA type=hidden value=40 size=4></td><td colspan=2>mithril plate armor</td><td>40</td></tr>\n<tr><td><input name=amountACAR size=4><input name=costACAR type=hidden value=42 size=4></td><td colspan=2>admantium chain armor</td><td>42</td></tr>\n<tr><td><input name=amountASCA size=4><input name=costASCA type=hidden value=60 size=4></td><td colspan=2>admantium scale armor</td><td>60</td></tr>\n<tr><td><input name=amountAPLA size=4><input name=costAPLA type=hidden value=100 size=4></td><td colspan=2>admantium plate armor</td><td>100</td></tr>\n";

}

print $laber1;
print "<table border=1><form action=/cgi-bin/tarmellion/reward-form.pl method=POST><input type=hidden name=type value=".$type.">\n";
print $laber2;
print "<tr><td>alternate name</td><td colspan=3><input name=name size=20></td></tr>\n" unless ($noname == 1);
print "<tr><td>alternate description</td><td colspan=3><input name=descript size=20></td></tr>\n" unless ($noname == 1);
print "<tr><td><input name=faction type=hidden value=$faction>Password</td><td colspan=3><input type=password name=passwd size=12></td></tr>\n<tr><td colspan=4><input type=submit value=order></td></tr>\n</form>\n</table>\n";
print "</body>\n</html>\n";
