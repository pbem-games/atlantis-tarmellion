#!/usr/bin/perl
# script to choose rewards
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

#specify the relative path to the players.in file
$pin = "/home/tarmellion/code/tarmellion/players.in";
$goodraces = "elf human dwarf ffolk viking dervish nomad darkman halfelf barbarian highlander clansman imperial tribesman ffolk cajun folk lowlander gnome halfing lizardman";
$Faction=0;
push(@factions, $Faction);
$newnumber = 0;

$q = new CGI;

@fields = $q->param();

foreach $field (@fields) {
  $$field = $q->param($field);
}

#preparing the html page
print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE></HEAD>\n";
print "<base target=Datei>\n";
print "<body background=/images/background.jpg>\n";
print "<h2>Times Rewards for Faction $faction</h2><p>\n";

#parsing the players.in file
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    #find the factions
    if (/^Faction:\s(\d*)/){
	$Faction = $1;
	$entry{$Faction} = "Faction: $Faction\n";
	push(@factions, $Faction);
	next;
    }
    #find old times rewards
    if (/^Timespunkte:\s(\d*)/){
      ($dummy,$reward{$Faction}) = split(/\s/,$_);
      $oldreward{$Faction} = $reward{$Faction};
      next;
    }
    #find Location of faction
    if (/^Location:\s(\d*)\s(\d*)\s(\d*)/){
	  ($dummy,$loc1,$loc2,$loc3) = split(/\s/,$_);
	  $location1{$Faction} = $loc1;
	  $location2{$Faction} = $loc2;
	  $location3{$Faction} = $loc3;
	  next;
    }
    #find old rewards
    if (/^Reward:\s(\d*)\sSILV/){
      $silver{$Faction} = $1;
      next;
    }
    #find hero status
    if (/^Hero:\s(\d)/){
	$herocount{$Faction} = $1;
	next;
    }

    #saving all listed data
    $entry{$Faction} .= $_."\n";
    #find race, alignment and profession of faction
    if (/^Rasse:\s(\.*)/){
      	($dummy,$race{$Faction}) = split(/\s/,$_);
		$race{$Faction} =~ s/\"//g;
		if ($goodraces =~ $race{$Faction}){
			$alignment{$Faction} = "good";
      	} else {
			$alignment{$Faction} = "evil";
      	}
    }
    if (/^Profession:\s(\.*)/){
	  ($dummy,$prof) = split(/\s/,$_);
	  $profession{$Faction} = $prof;
    }
    #find Password and character of faction
    if (/^Password:\s(\.*)/){
	  ($dummy,$pass) = split(/\s/,$_);
	  $password{$Faction} = $pass;
    }
    if (/^Character:\s(\d*)/){
	  ($dummy,$chara) = split(/\s/,$_);
	  $character{$Faction} = $chara;
    }
    #find the highest added unit number
    if (/^NewUnit:\s(\d*)/){
	($dummy,$unitnumber) = split(/\s/,$_);
	if ($unitnumber > $newnumber){
	    $newnumber = $unitnumber;
	}
    }
}
close(PIN);

#specify the relative path to the newplayers.in file
$npin = "/home/tarmellion/code/tarmellion/newplayers.in";
open(NPIN, "<$npin");
while (<NPIN>) {
    chomp;
    #find the last added unit
    if (/^NewUnit:\s(\d*)/){
	($dummy,$unitnumber) = split(/\s/,$_);
	if ($unitnumber > $newnumber){
	    $newnumber = $unitnumber;
	}
    }
}
close(NPIN);

$newnumber++;

$okay = "ja";

#implementing the changes from reward-form.html
if ($type eq "loc"){
  $location1{$faction} = $xcoor;
  $location2{$faction} = $ycoor;
  $location3{$faction} = $level;
  unless ($xcoor =~ /\d*/){
    print "<p>You have to specify a valid coordinate!\n";
	$okay = "nein";
  }
  unless ($yccor =~/\d*/){
	print "<p>You have to specify a valid coordinate!\n";
    $okay = "nein";
  }
  unless ($yccor =~/\d*/){
	print "<p>You have to specify a valid level!\n";
	$okay = "nein";
  }   
  if ($okay ne "nein"){
  	$reward{$faction} = $reward{$faction} - 2;
  	print "<p>Your starting locations has been changed to $xcoor $ycoor $level.\n";
  }
}

elsif ($type eq "money"){
  $silver{$faction} = $silver{$faction} + ($money * 25);
  $reward{$faction} = $reward{$faction} - $money;
  print "<p>".($money * 25)." silver has been credited to your unclaimed account.\n";
}

elsif ($type eq "unit10"){
    if ($amount eq "") {
      print "<br>You have to order at least one man!\n";
      $okay = "nein";
    }
    if ($racenew eq "human"){
      $race = "RHUMA";
    } elsif ($racenew eq "barbarian"){
      $race = "RBARB";
    } elsif ($racenew eq "tribesman"){
      $race = "RTRIB";
    } elsif ($racenew eq "viking"){
      $race = "RVIKI";
    } elsif ($racenew eq "ffolk"){
      $race = "RFFOL";
    } elsif ($racenew eq "clansman"){
      $race = "RCLAN";
    } elsif ($racenew eq "nomad"){
      $race = "RNOMA";
    } elsif ($racenew eq "imperial"){
      $race = "RIMPE";
    } elsif ($racenew eq "darkman"){
      $race = "RDARK";
    } elsif ($racenew eq "halfelf"){
      $race = "RHELF";
    } elsif ($racenew eq "gnome"){
      $race = "RYGNO";
    } elsif ($racenew eq "halfing"){
      $race = "RHALF";
    } elsif ($racenew eq "dwarf"){
      $race = "RYDWA";
    } elsif ($racenew eq "lizardman"){
      $race = "RLIZA";
    } elsif ($racenew eq "buccaneer"){
      $race = "RBUCC";
    } elsif ($racenew eq "republican"){
      $race = "RREPU";
    } elsif ($racenew eq "headhunter"){
      $race = "RHEAD";
    } elsif ($racenew eq "sandling"){
      $race = "RSAND";
    } elsif ($racenew eq "amazon"){
      $race = "RAMAZ";
    } elsif ($racenew eq "serpent people"){
      $race = "RSERP";
    } elsif ($racenew eq "black keep"){
      $race = "RBLAC";
    } elsif ($racenew eq "orc"){
      $race = "RORCS";
    } elsif ($racenew eq "goblin"){
      $race = "RGOBL";
    } elsif ($racenew eq "gnoll"){
      $race = "RGNOL";
    } elsif ($racenew eq "ogre"){
      $race = "ROGRE";
    } elsif ($racenew eq "minotaur"){
      $race = "RMINO";
    } elsif ($racenew eq "drow"){
      $race = "RDROW";
    } elsif ($racenew eq "duergar"){
      $race = "RDUER";
    } else {
	print "<p>You have to choose a race.";
	$okay = "nein";
    }
    if ($skillnew eq "mining"){
      $skill = "MINI";
      $tool = "PICK";
      $beruf = "Miners";
    } elsif ($skillnew eq "lumberjack"){
      $skill = "LUMB";
      $tool = "AXE";
      $beruf = "Woodcutters";
    } elsif ($skillnew eq "quarrying"){
      $skill = "QUAR";
      $tool = "PICK";
      $beruf = "Stonecutters";
    } elsif ($skillnew eq "hunting"){
      $skill = "HUNT";
      $tool = "JAVE";
      $beruf = "Hunters";
    } elsif ($skillnew eq "fishing"){
      $skill = "FISH";
      $tool = "NET";
      $beruf = "Fishermen";
    } elsif ($skillnew eq "herb lore"){
      $skill = "HERB";
      $tool = "BAG";
      $beruf = "Herb Gatherers";
    } elsif ($skillnew eq "entertainment"){
      $skill = "ENTE";
      $tool = "SILV";
      $beruf = "Harpers";
    } elsif ($skillnew eq "farming"){
      $skill = "FARM";
      $tool = "BAG";
      $beruf = "Farmers";
    } elsif ($skillnew eq "ranching"){
      $skill = "RANC";
      $tool = "LASS";
      $beruf = "Ranchers";
    } elsif ($skillnew eq "sailing"){
      $skill = "SAIL";
      $tool = "KNIF";
      $beruf = "Sailors";
    } elsif ($skillnew eq "animal training"){
      $skill = "ATRA";
      $tool = "LASS";
      $beruf = "Animal Trainers";
    } else {
	print "<p>You have to choose a skill.";
	$okay = "nein";
    }
    if ($reward{$faction} < ($amount * 10)){
      print "<p>You cannot levy that many men!!!\n";
      $okay = "nein";
    }
    if ($okay ne "nein"){
      $newline = "Loc: ".$location1{$faction}." ".$location2{$faction}." ".$location3{$faction}."\n";
      $newline .= "NewUnit: ".$newnumber."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$race."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." CLOS\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$tool."\n";
      $newline .= "Skill: gm".$newnumber." ".$skill." 90\n";
      $newline .= "Order: gm".$newnumber." AVOID 1\n";
      if ($name eq "") {
	$newline .= "Order: gm".$newnumber." NAME UNIT ".$beruf."\n";
      } else {
	$newline .= "Order: gm".$newnumber." NAME UNIT \"".$name."\"\n";
      }
      if ($descript eq "") {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"a group of new followers to your cause\"\n";
      } else {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"".$descript."\"\n";
      }
      $entry{$faction} .= $newline;
      $reward{$faction} = $reward{$faction} - ($amount * 10);
      print "<p>$amount $beruf will join you shortly.\n"; 
    }
}
elsif ($type eq "unit20"){
    if ($amount eq "") {
      print "<br>You have to order at least one man!\n";
      $okay = "nein";
    }
    if ($racenew eq "human"){
      $race = "RHUMA";
    } elsif ($racenew eq "barbarian"){
      $race = "RBARB";
    } elsif ($racenew eq "tribesman"){
      $race = "RTRIB";
    } elsif ($racenew eq "viking"){
      $race = "RVIKI";
    } elsif ($racenew eq "ffolk"){
      $race = "RFFOL";
    } elsif ($racenew eq "clansman"){
      $race = "RCLAN";
    } elsif ($racenew eq "nomad"){
      $race = "RNOMA";
    } elsif ($racenew eq "imperial"){
      $race = "RIMPE";
    } elsif ($racenew eq "darkman"){
      $race = "RDARK";
    } elsif ($racenew eq "halfelf"){
      $race = "RHELF";
    } elsif ($racenew eq "gnome"){
      $race = "RYGNO";
    } elsif ($racenew eq "halfing"){
      $race = "RHALF";
    } elsif ($racenew eq "dwarf"){
      $race = "RYDWA";
    } elsif ($racenew eq "lizardman"){
      $race = "RLIZA";
    } elsif ($racenew eq "buccaneer"){
      $race = "RBUCC";
    } elsif ($racenew eq "republican"){
      $race = "RREPU";
    } elsif ($racenew eq "headhunter"){
      $race = "RHEAD";
    } elsif ($racenew eq "sandling"){
      $race = "RSAND";
    } elsif ($racenew eq "amazon"){
      $race = "RAMAZ";
    } elsif ($racenew eq "serpent people"){
      $race = "RSERP";
    } elsif ($racenew eq "black keep"){
      $race = "RBLAC";
    } elsif ($racenew eq "orc"){
      $race = "RORCS";
    } elsif ($racenew eq "goblin"){
      $race = "RGOBL";
    } elsif ($racenew eq "gnoll"){
      $race = "RGNOL";
    } elsif ($racenew eq "ogre"){
      $race = "ROGRE";
    } elsif ($racenew eq "minotaur"){
      $race = "RMINO";
    } elsif ($racenew eq "drow"){
      $race = "RDROW";
    } elsif ($racenew eq "duergar"){
      $race = "RDUER";
    } else {
	  print "<p>You have to choose a race.";
      $okay = "nein";
    }
    if ($skillnew eq "smithing"){
      $skill = "SMIT";
      $tool = "HAMM";
      $beruf = "Smiths";
    } elsif ($skillnew eq "toolmaking"){
      $skill = "TOOL";
      $tool = "AXE";
      $beruf = "Toolmakers";
    } elsif ($skillnew eq "building"){
      $skill = "BUIL";
      $tool = "SAW";
      $beruf = "Builders";
    } elsif ($skillnew eq "shipbuilding"){
      $skill = "SHIP";
      $tool = "AXE";
      $beruf = "Shipbuilders";
    } elsif ($skillnew eq "cooking"){
      $skill = "COOK";
      $tool = "CUTL";
      $beruf = "Cooks";
    } else {
	print "<p>You have to select a skill.";
      $okay = "nein";
    }
    if ($reward{$faction} < ($amount * 12)){
      print "<p>You cannot levy that many men!!!\n";
      $okay = "nein";
    }
    if ($okay ne "nein"){
      $newline = "Loc: ".$location1{$faction}." ".$location2{$faction}." ".$location3{$faction}."\n";
      $newline .= "NewUnit: ".$newnumber."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$race."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." GARM\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$tool."\n";
      $newline .= "Skill: gm".$newnumber." ".$skill." 90\n";
      $newline .= "Order: gm".$newnumber." AVOID 1\n";
      if ($name eq "") {
	$newline .= "Order: gm".$newnumber." NAME UNIT ".$beruf."\n";
      } else {
	$newline .= "Order: gm".$newnumber." NAME UNIT \"".$name."\"\n";
      }
      if ($descript eq "") {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"a group of new followers to your cause\"\n";
      } else {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"".$descript."\"\n";
      }
      $entry{$faction} .= $newline;
      $reward{$faction} = $reward{$faction} - ($amount * 12);
      print "<p>$amount $beruf will join you shortly.\n";
    }
}

elsif ($type eq "soldier"){
    if ($amount eq "") {
      print "<br>You have to order at least one man!\n";
      $okay = "nein";
    }
    if ($racenew eq "human"){
      $race = "RHUMA";
    } elsif ($racenew eq "barbarian"){
      $race = "RBARB";
    } elsif ($racenew eq "tribesman"){
      $race = "RTRIB";
    } elsif ($racenew eq "viking"){
      $race = "RVIKI";
    } elsif ($racenew eq "ffolk"){
      $race = "RFFOL";
    } elsif ($racenew eq "clansman"){
      $race = "RCLAN";
    } elsif ($racenew eq "nomad"){
      $race = "RNOMA";
    } elsif ($racenew eq "imperial"){
      $race = "RIMPE";
    } elsif ($racenew eq "darkman"){
      $race = "RDARK";
    } elsif ($racenew eq "halfelf"){
      $race = "RHELF";
    } elsif ($racenew eq "gnome"){
      $race = "RYGNO";
    } elsif ($racenew eq "halfing"){
      $race = "RHALF";
    } elsif ($racenew eq "dwarf"){
      $race = "RYDWA";
    } elsif ($racenew eq "lizardman"){
      $race = "RLIZA";
    } elsif ($racenew eq "buccaneer"){
      $race = "RBUCC";
    } elsif ($racenew eq "republican"){
      $race = "RREPU";
    } elsif ($racenew eq "headhunter"){
      $race = "RHEAD";
    } elsif ($racenew eq "sandling"){
      $race = "RSAND";
    } elsif ($racenew eq "amazon"){
      $race = "RAMAZ";
    } elsif ($racenew eq "serpent people"){
      $race = "RSERP";
    } elsif ($racenew eq "black keep"){
      $race = "RBLAC";
    } elsif ($racenew eq "orc"){
      $race = "RORCS";
    } elsif ($racenew eq "goblin"){
      $race = "RGOBL";
    } elsif ($racenew eq "gnoll"){
      $race = "RGNOL";
    } elsif ($racenew eq "ogre"){
      $race = "ROGRE";
    } elsif ($racenew eq "minotaur"){
      $race = "RMINO";
    } elsif ($racenew eq "drow"){
      $race = "RDROW";
    } elsif ($racenew eq "duergar"){
      $race = "RDUER";
    } else {
	  print "<p>You have to choose a race.";
      $okay = "nein";
    }
    if ($skillnew eq "combat"){
      $skill = "COMB";
      $weap = "ISWO";
      $armo = "LARM";
      $beruf = "Infantry";
    } elsif ($skillnew eq "archery"){
      $skill = "ARCH";
      $weap = "SBOW";
      $armo = "CLAR";
      $beruf = "Archers";
    } elsif ($skillnew eq "crossbow"){
      $skill = "XBOW";
      $weap = "LXBO";
      $armo = "CLAR";
      $beruf = "Archers";
    } elsif ($skillnew eq "riding"){
      $skill = "RIDI";
      $weap = "LANC";
      $armo = "HORS";
      $beruf = "Cavalry";
    } elsif ($skillnew eq "healing"){
      $skill = "HEAL";
      $weap = "PDAG";
      $armo = "MEPL";
      $beruf = "Medics";
    } else {
	print "<p>You have to select a skill.";
      $okay = "nein";
    }
    if ($reward{$faction} < ($amount * 12)){
      print "<p>You cannot levy that many men!!!\n";
      $okay = "nein";
    }
    if ($okay ne "nein"){
      $newline = "Loc: ".$location1{$faction}." ".$location2{$faction}." ".$location3{$faction}."\n";
      $newline .= "NewUnit: ".$newnumber."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$race."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$weap."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$armo."\n";
      $newline .= "Skill: gm".$newnumber." ".$skill." 90\n";
      $newline .= "Order: gm".$newnumber." AVOID 1\n";
      if ($name eq "") {
	$newline .= "Order: gm".$newnumber." NAME UNIT ".$beruf."\n";
      } else {
	$newline .= "Order: gm".$newnumber." NAME UNIT \"".$name."\"\n";
      }
      if ($descript eq "") {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"a unit of soldiers eager to fight for your cause\"\n";
      } else {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"".$descript."\"\n";
      }
      $entry{$faction} .= $newline;
      $reward{$faction} = $reward{$faction} - ($amount * 12);
      print "<p>$amount $beruf reporting for duty.\n";
    }
}

elsif ($type eq "leader10"){
    if ($amount eq "") {
      print "<br>You have to order at least one man!\n";
      $okay = "nein";
    }
    if ($racenew eq "human"){
      $race = "LHUMA";
    } elsif ($racenew eq "barbarian"){
      $race = "LBARB";
    } elsif ($racenew eq "tribesman"){
      $race = "LTRIB";
    } elsif ($racenew eq "viking"){
      $race = "LVIKI";
    } elsif ($racenew eq "ffolk"){
      $race = "LFFOL";
    } elsif ($racenew eq "highlander"){
      $race = "LHIGH";
    } elsif ($racenew eq "dervish"){
      $race = "LDERV";
    } elsif ($racenew eq "imperial"){
      $race = "LIMPE";
    } elsif ($racenew eq "darkman"){
      $race = "LDARK";
    } elsif ($racenew eq "elf"){
      $race = "LELF";
    } elsif ($racenew eq "gnome"){
      $race = "LGNOM";
    } elsif ($racenew eq "lowlander"){
      $race = "LLOWL";
    } elsif ($racenew eq "dwarf"){
      $race = "LDWAR";
    } elsif ($racenew eq "cajun folk"){
      $race = "LCAJU";
    } elsif ($racenew eq "buccaneer"){
      $race = "LBUCC";
    } elsif ($racenew eq "republican"){
      $race = "LREPU";
    } elsif ($racenew eq "headhunter"){
      $race = "LHEAD";
    } elsif ($racenew eq "sandling"){
      $race = "LSAND";
    } elsif ($racenew eq "amazon"){
      $race = "LAMAZ";
    } elsif ($racenew eq "serpent people"){
      $race = "LSERP";
    } elsif ($racenew eq "black keep"){
      $race = "LBLAC";
    } elsif ($racenew eq "uruk"){
      $race = "LURUK";
    } elsif ($racenew eq "hobgoblin"){
      $race = "LHOBG";
    } elsif ($racenew eq "troll"){
      $race = "LTROL";
    } elsif ($racenew eq "ogre"){
      $race = "LOGRE";
    } elsif ($racenew eq "minotaur"){
      $race = "LMINO";
    } elsif ($racenew eq "drow"){
      $race = "LDROW";
    } elsif ($racenew eq "duergar"){
      $race = "LDUER";
    } else {
	  print "<p>You have to choose a race.";
      $okay = "nein";
    }
    if ($skillnew eq "mining"){
      $skill = "MINI";
      $tool = "PICK";
      $beruf = "Master Miner";
    } elsif ($skillnew eq "lumberjack"){
      $skill = "LUMB";
      $tool = "AXE";
      $beruf = "Master Woodcutter";
    } elsif ($skillnew eq "quarrying"){
      $skill = "QUAR";
      $tool = "PICK";
      $beruf = "Master Stonecutter";
    } elsif ($skillnew eq "hunting"){
      $skill = "HUNT";
      $tool = "JAVE";
      $beruf = "Master Hunter";
    } elsif ($skillnew eq "fishing"){
      $skill = "FISH";
      $tool = "NET";
      $beruf = "Master Fisherman";
    } elsif ($skillnew eq "herb lore"){
      $skill = "HERB";
      $tool = "BAG";
      $beruf = "Master Herb Gatherer";
    } elsif ($skillnew eq "entertainment"){
      $skill = "ENTE";
      $tool = "SILV";
      $beruf = "Master Harper";
    } elsif ($skillnew eq "farming"){
      $skill = "FARM";
      $tool = "BAG";
      $beruf = "Master Farmer";
    } elsif ($skillnew eq "ranching"){
      $skill = "RANC";
      $tool = "LASS";
      $beruf = "Master Rancher";
    } elsif ($skillnew eq "sailing"){
      $skill = "SAIL";
      $tool = "KNIF";
      $beruf = "Master Sailor";
    } elsif ($skillnew eq "animal training"){
      $skill = "ATRA";
      $tool = "LASS";
      $beruf = "Master Animal Trainer";
    } else {
	  print "<p>You have to choose a skill.";
      $okay = "nein";
    }
    if ($reward{$faction} < ($amount * 25)){
      print "<p>You cannot levy that many men!!!\n";
      $okay = "nein";
    }
	if ($okay ne "nein"){
      $newline = "Loc: ".$location1{$faction}." ".$location2{$faction}." ".$location3{$faction}."\n";
      $newline .= "NewUnit: ".$newnumber."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$race."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." CLOS\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$tool."\n";
      $newline .= "Skill: gm".$newnumber." ".$skill." 180\n";
      $newline .= "Order: gm".$newnumber." AVOID 1\n";
      if ($name eq "") {
	$newline .= "Order: gm".$newnumber." NAME UNIT ".$beruf."\n";
      } else {
	$newline .= "Order: gm".$newnumber." NAME UNIT \"".$name."\"\n";
      }
      if ($descript eq "") {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"a master of his profession\"\n";
      } else {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"".$descript."\"\n";
      }
      $entry{$faction} .= $newline;
      $reward{$faction} = $reward{$faction} - ($amount * 25);
      print "<p>$amount $beruf will join you shortly.\n"; 
    }
}

elsif ($type eq "leader20"){
    if ($amount eq "") {
      print "<br>You have to order at least one man!\n";
      $okay = "nein";
    }
    if ($racenew eq "human"){
      $race = "LHUMA";
    } elsif ($racenew eq "barbarian"){
      $race = "LBARB";
    } elsif ($racenew eq "tribesman"){
      $race = "LTRIB";
    } elsif ($racenew eq "viking"){
      $race = "LVIKI";
    } elsif ($racenew eq "ffolk"){
      $race = "LFFOL";
    } elsif ($racenew eq "highlander"){
      $race = "LHIGH";
    } elsif ($racenew eq "dervish"){
      $race = "LDERV";
    } elsif ($racenew eq "imperial"){
      $race = "LIMPE";
    } elsif ($racenew eq "darkman"){
      $race = "LDARK";
    } elsif ($racenew eq "elf"){
      $race = "LELF";
    } elsif ($racenew eq "gnome"){
      $race = "LGNOM";
    } elsif ($racenew eq "lowlander"){
      $race = "LLOWL";
    } elsif ($racenew eq "dwarf"){
      $race = "LDWAR";
    } elsif ($racenew eq "cajun folk"){
      $race = "LCAJU";
    } elsif ($racenew eq "buccaneer"){
      $race = "LBUCC";
    } elsif ($racenew eq "republican"){
      $race = "LREPU";
    } elsif ($racenew eq "headhunter"){
      $race = "LHEAD";
    } elsif ($racenew eq "sandling"){
      $race = "LSAND";
    } elsif ($racenew eq "amazon"){
      $race = "LAMAZ";
    } elsif ($racenew eq "serpent people"){
      $race = "LSERP";
    } elsif ($racenew eq "black keep"){
      $race = "LBLAC";
    } elsif ($racenew eq "uruk"){
      $race = "LURUK";
    } elsif ($racenew eq "hobgoblin"){
      $race = "LHOBG";
    } elsif ($racenew eq "troll"){
      $race = "LTROL";
    } elsif ($racenew eq "ogre"){
      $race = "LOGRE";
    } elsif ($racenew eq "minotaur"){
      $race = "LMINO";
    } elsif ($racenew eq "drow"){
      $race = "LDROW";
    } elsif ($racenew eq "duergar"){
      $race = "LDUER";
    } else {
	  print "<p>You have to choose a race.";
      $okay = "nein";
    }
    if ($skillnew eq "smithing"){
      $skill = "SMIT";
      $tool = "HAMM";
      $beruf = "Master Smith";
    } elsif ($skillnew eq "toolmaking"){
      $skill = "TOOL";
      $tool = "AXE";
      $beruf = "Master Toolmaker";
    } elsif ($skillnew eq "building"){
      $skill = "BUIL";
      $tool = "SAW";
      $beruf = "Master Builder";
    } elsif ($skillnew eq "shipbuilding"){
      $skill = "SHIP";
      $tool = "AXE";
      $beruf = "Master Shipbuilder";
    } elsif ($skillnew eq "cooking"){
      $skill = "COOK";
      $tool = "CUTL";
      $beruf = "Master Cook";
    } else {
	print "<p>You have to select a skill.";
      $okay = "nein";
    }
    if ($reward{$faction} < ($amount * 28)){
      print "<p>You cannot levy that many men!!!\n";
      $okay = "nein";
    }
    if ($okay ne "nein"){
      $newline = "Loc: ".$location1{$faction}." ".$location2{$faction}." ".$location3{$faction}."\n";
      $newline .= "NewUnit: ".$newnumber."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$race."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." GARM\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$tool."\n";
      $newline .= "Skill: gm".$newnumber." ".$skill." 180\n";
      $newline .= "Order: gm".$newnumber." AVOID 1\n";
      if ($name eq "") {
	$newline .= "Order: gm".$newnumber." NAME UNIT ".$beruf."\n";
      } else {
	$newline .= "Order: gm".$newnumber." NAME UNIT \"".$name."\"\n";
      }
      if ($descript eq "") {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"a master of his profession\"\n";
      } else {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"".$descript."\"\n";
      }
      $entry{$faction} .= $newline;
      $reward{$faction} = $reward{$faction} - ($amount * 28);
      print "<p>$amount $beruf will join you shortly.\n";
    }
}

elsif ($type eq "elitesoldier"){
    if ($amount eq "") {
      print "<br>You have to order at least one man!\n";
      $okay = "nein";
    }
    if ($racenew eq "human"){
      $race = "LHUMA";
    } elsif ($racenew eq "barbarian"){
      $race = "LBARB";
    } elsif ($racenew eq "tribesman"){
      $race = "LTRIB";
    } elsif ($racenew eq "viking"){
      $race = "LVIKI";
    } elsif ($racenew eq "ffolk"){
      $race = "LFFOL";
    } elsif ($racenew eq "highlander"){
      $race = "LHIGH";
    } elsif ($racenew eq "dervish"){
      $race = "LDERV";
    } elsif ($racenew eq "imperial"){
      $race = "LIMPE";
    } elsif ($racenew eq "darkman"){
      $race = "LDARK";
    } elsif ($racenew eq "elf"){
      $race = "LELF";
    } elsif ($racenew eq "gnome"){
      $race = "LGNOM";
    } elsif ($racenew eq "lowlander"){
      $race = "LLOWL";
    } elsif ($racenew eq "dwarf"){
      $race = "LDWAR";
    } elsif ($racenew eq "cajun folk"){
      $race = "LCAJU";
    } elsif ($racenew eq "buccaneer"){
      $race = "LBUCC";
    } elsif ($racenew eq "republican"){
      $race = "LREPU";
    } elsif ($racenew eq "headhunter"){
      $race = "LHEAD";
    } elsif ($racenew eq "sandling"){
      $race = "LSAND";
    } elsif ($racenew eq "amazon"){
      $race = "LAMAZ";
    } elsif ($racenew eq "serpent people"){
      $race = "LSERP";
    } elsif ($racenew eq "black keep"){
      $race = "LBLAC";
    } elsif ($racenew eq "uruk"){
      $race = "LURUK";
    } elsif ($racenew eq "hobgoblin"){
      $race = "LHOBG";
    } elsif ($racenew eq "troll"){
      $race = "LTROL";
    } elsif ($racenew eq "ogre"){
      $race = "LOGRE";
    } elsif ($racenew eq "minotaur"){
      $race = "LMINO";
    } elsif ($racenew eq "drow"){
      $race = "LDROW";
    } elsif ($racenew eq "duergar"){
      $race = "LDUER";
    } else {
	  print "<p>You have to choose a race.";
      $okay = "nein";
    }
    if ($skillnew eq "combat"){
      $skill = "COMB";
      $weap = "ILSW";
      $armo = "ICAR";
      $beruf = "Fighter";
    } elsif ($skillnew eq "archery"){
      $skill = "ARCH";
      $weap = "SBOW";
      $armo = "LARM";
      $beruf = "Master Bowman";
    } elsif ($skillnew eq "crossbow"){
      $skill = "XBOW";
      $weap = "LXBO";
      $armo = "LARM";
      $beruf = "Master Crossbowman";
    } elsif ($skillnew eq "riding"){
      $skill = "RIDI";
      $weap = "LANC";
      if ($alignment{$faction} eq "evil"){
	  $armo = "WHOR";
      } else {
	  $armo = "CHAR";
      }
      $beruf = "Knight";
    } elsif ($skillnew eq "healing"){
      $skill = "HEAL";
      $weap = "PDAG";
      $armo = "MEDI";
      $beruf = "Surgeon";
    } else {
	print "<p>You have to select a skill.";
      $okay = "nein";
    }
    if ($reward{$faction} < ($amount * 30)){
      print "<p>You cannot levy that many men!!!\n";
      $okay = "nein";
    }
    if ($okay ne "nein"){
      $newline = "Loc: ".$location1{$faction}." ".$location2{$faction}." ".$location3{$faction}."\n";
      $newline .= "NewUnit: ".$newnumber."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$race."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$weap."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$armo."\n";
      $newline .= "Skill: gm".$newnumber." ".$skill." 180\n";
      $newline .= "Order: gm".$newnumber." AVOID 1\n";
      if ($name eq "") {
	$newline .= "Order: gm".$newnumber." NAME UNIT ".$beruf."\n";
      } else {
	$newline .= "Order: gm".$newnumber." NAME UNIT \"".$name."\"\n";
      }
      if ($descript eq "") {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"a professional soldier\"\n";
      } else {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"".$descript."\"\n";
      }
      $entry{$faction} .= $newline;
      $reward{$faction} = $reward{$faction} - ($amount * 30);
      print "<p>$amount $beruf reporting for duty.\n";
    }
}

elsif ($type eq "specialist"){
    $amount = 1;
    if ($racenew eq "human"){
      $race = "LHUMA";
    } elsif ($racenew eq "barbarian"){
      $race = "LBARB";
    } elsif ($racenew eq "tribesman"){
      $race = "LTRIB";
    } elsif ($racenew eq "viking"){
      $race = "LVIKI";
    } elsif ($racenew eq "ffolk"){
      $race = "LFFOL";
    } elsif ($racenew eq "highlander"){
      $race = "LHIGH";
    } elsif ($racenew eq "dervish"){
      $race = "LDERV";
    } elsif ($racenew eq "imperial"){
      $race = "LIMPE";
    } elsif ($racenew eq "darkman"){
      $race = "LDARK";
    } elsif ($racenew eq "elf"){
      $race = "LELF";
    } elsif ($racenew eq "gnome"){
      $race = "LGNOM";
    } elsif ($racenew eq "lowlander"){
      $race = "LLOWL";
    } elsif ($racenew eq "dwarf"){
      $race = "LDWAR";
    } elsif ($racenew eq "cajun folk"){
      $race = "LCAJU";
    } elsif ($racenew eq "buccaneer"){
      $race = "LBUCC";
    } elsif ($racenew eq "republican"){
      $race = "LREPU";
    } elsif ($racenew eq "headhunter"){
      $race = "LHEAD";
    } elsif ($racenew eq "sandling"){
      $race = "LSAND";
    } elsif ($racenew eq "amazon"){
      $race = "LAMAZ";
    } elsif ($racenew eq "serpent people"){
      $race = "LSERP";
    } elsif ($racenew eq "black keep"){
      $race = "LBLAC";
    } elsif ($racenew eq "uruk"){
      $race = "LURUK";
    } elsif ($racenew eq "hobgoblin"){
      $race = "LHOBG";
    } elsif ($racenew eq "troll"){
      $race = "LTROL";
    } elsif ($racenew eq "ogre"){
      $race = "LOGRE";
    } elsif ($racenew eq "minotaur"){
      $race = "LMINO";
    } elsif ($racenew eq "drow"){
      $race = "LDROW";
    } elsif ($racenew eq "duergar"){
      $race = "LDUER";
    } else {
	  print "<p>You have to choose a race.";
      $okay = "nein";
    }
    if ($skillnew eq "tactics (80 RP)"){
      $skill = "TACT";
      $weap = "ILSW";
      $armo = "ISCA";
      if ($alignment{$faction} eq "evil"){
	  $mount = "WHOR";
      } else {
	  $armo = "CHAR";
      }
      $beruf = "General";
      $cost = 80;
    } elsif ($skillnew eq "observation (40 RP)"){
      $skill = "OBSE";
      $weap = "PDAG";
      $armo = "CLAR";
      $beruf = "Detective";
      $cost = 40;
    } elsif ($skillnew eq "stealth (40 RP)"){
      $skill = "STEA";
      $weap = "IDAG";
      $armo = "CLAR";
      $beruf = "Agent";
      $cost = 40;
    } elsif ($skillnew eq "scouting (40 RP)"){
      $skill = "SCOU";
      $weap = "IDAG";
      $armo = "CLAR";
      $beruf = "Scout";
      $cost = 40;
    } else {
	print "<p>You have to select a skill.";
      $okay = "nein";
    }
    if ($reward{$faction} < ($amount * $cost)){
      print "<p>You cannot levy that many men!!!\n";
      $okay = "nein";
    }
    if ($okay ne "nein"){
      $newline = "Loc: ".$location1{$faction}." ".$location2{$faction}." ".$location3{$faction}."\n";
      $newline .= "NewUnit: ".$newnumber."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$race."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$weap."\n";
      $newline .= "Item: gm".$newnumber." ".$amount." ".$armo."\n";
      if ($mount ne ""){
	  $newline .= "Item: gm".$newnumber." ".$amount." ".$mount."\n";
      }
      $newline .= "Skill: gm".$newnumber." ".$skill." 180\n";
      $newline .= "Order: gm".$newnumber." AVOID 1\n";
      if ($name eq "") {
	$newline .= "Order: gm".$newnumber." NAME UNIT ".$beruf."\n";
      } else {
	$newline .= "Order: gm".$newnumber." NAME UNIT \"".$name."\"\n";
      }
      if ($descript eq "") {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"a much needed specialist\"\n";
      } else {
	$newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"".$descript."\"\n";
      }
      $entry{$faction} .= $newline;
      $reward{$faction} = $reward{$faction} - ($amount * $cost);
      print "<p>a $beruf will put his skills to your cause.\n";
    }
}

elsif ($type eq "mage"){
    if ($reward{$faction} < 100){
	print "<p>You do not have enough reward points left!!!\n";
	$okay = "nein";
    }
    if ($mage eq "hobgoblin fire sorcerer") {
	$race = "LHOBG";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "FIRE";
	$skill2wert = 90;
	$skill3 = "WSHI";
	$skill3wert = 90;
    } elsif ($mage eq "buccaneer sea wizard") {
	$race = "LBUCC";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "WEAT";
	$skill2wert = 90;
	$skill3 = "SWIN";
	$skill3wert = 90;
    } elsif ($mage eq "buccaneer wind sorcerer") {
	$race = "LBUCC";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "WEAT";
	$skill2wert = 90;
	$skill3 = "SSTO";
	$skill3wert = 90;
    } elsif ($mage eq "sandling fire sorcerer") {
	$race = "LSAND";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "FIRE";
	$skill2wert = 90;
	$skill3 = "WSHI";
	$skill3wert = 90;
    } elsif ($mage eq "sandling wind sorcerer") {
	$race = "LSAND";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "WEAT";
	$skill2wert = 90;
	$skill3 = "SSTO";
	$skill3wert = 90;
    } elsif ($mage eq "uruk ice sorcerer") {
	$race = "LURUK";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "ICE";
	$skill2wert = 90;
	$skill3 = "ESHI";
	$skill3wert = 90;
    } elsif ($mage eq "uruk weather wizard") {
	$race = "LURUK";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "WEAT";
	$skill2wert = 90;
	$skill3 = "CLEA";
	$skill3wert = 90;
    } elsif ($mage eq "ogre fire sorcerer") {
	$race = "LOGRE";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "FIRE";
	$skill2wert = 90;
	$skill3 = "WSHI";
	$skill3wert = 90;
    } elsif ($mage eq "ogre weather wizard") {
	$race = "LOGRE";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "WEAT";
	$skill2wert = 90;
	$skill3 = "CLEA";
	$skill3wert = 90;
    } elsif ($mage eq "serpent people ice sorcerer") {
	$race = "LSERP";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "ICE";
	$skill2wert = 90;
	$skill3 = "ESHI";
	$skill3wert = 90;
    } elsif ($mage eq "drow ice sorcerer") {
	$race = "LDROW";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "ICE";
	$skill2wert = 90;
	$skill3 = "ESHI";
	$skill3wert = 90;
    } elsif ($mage eq "drow demon summoner") {
	$race = "LDROW";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "DEMO";
	$skill2wert = 90;
	$skill3 = "SUIM";
	$skill3wert = 90;
    } elsif ($mage eq "troll priest") {
	$race = "LTROL";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "SSHI";
	$skill2wert = 90;
	$skill3 = "MIND";
	$skill3wert = 90;
    } elsif ($mage eq "troll enchanter") {
	$race = "LTROL";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "MREA";
	$skill2wert = 90;
	$skill3 = "FARS";
	$skill3wert = 90;
    } elsif ($mage eq "black keep necromancer") {
	$race = "LBLAC";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "NECR";
	$skill2wert = 90;
	$skill3 = "SUSK";
	$skill3wert = 90;
    } elsif ($mage eq "headhunter priest") {
	$race = "LHEAD";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "MIND";
	$skill2wert = 90;
	$skill3 = "SSHI";
	$skill3wert = 90;
    } elsif ($mage eq "headhunter illusionist") {
	$race = "LHEAD";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "ILLU";
	$skill2wert = 90;
	$skill3 = "INVI";
	$skill3wert = 90;
    } elsif ($mage eq "headhunter inquisitor") {
	$race = "LHEAD";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "ILLU";
	$skill2wert = 90;
	$skill3 = "TRUE";
	$skill3wert = 90;
    } elsif ($mage eq "minotaur priest") {
	$race = "LMINO";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "MIND";
	$skill2wert = 90;
	$skill3 = "SSHI";
	$skill3wert = 90;
    } elsif ($mage eq "minotaur enchanter") {
	$race = "LMINO";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "FARS";
	$skill2wert = 90;
	$skill3 = "MREA";
	$skill3wert = 90;
    } elsif ($mage eq "republican gate magician") {
	$race = "LREPU";
	$skill1 = "PATT";
	$skill1wert = 90;
	$skill2 = "GATE";
	$skill2wert = 90;
	$skill3 = "CGAT";
	$skill3wert = 90;
    } elsif ($mage eq "amazon runesmith") {
	$race = "LAMAZ";
	$skill1 = "PATT";
	$skill1wert = 90;
	$skill2 = "ARTI";
	$skill2wert = 90;
	$skill3 = "EARM";
	$skill3wert = 45;
	$skill4 = "EWEA";
	$skill4wert = 45;
    } elsif ($mage eq "duergar runesmith") {
	$race = "LDUER";
	$skill1 = "PATT";
	$skill1wert = 90;
	$skill2 = "ARTI";
	$skill2wert = 90;
	$skill3 = "EARM";
	$skill3wert = 45;
	$skill4 = "EWEA";
	$skill4wert = 45;
    } elsif ($mage eq "human fire sorcerer") {
	$race = "LHUMA";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "FIRE";
	$skill2wert = 90;
	$skill3 = "WSHI";
	$skill3wert = 90;
    } elsif ($mage eq "viking wind sorcerer") {
	$race = "LVIKI";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "WEAT";
	$skill2wert = 90;
	$skill3 = "SSTO";
	$skill3wert = 90;
    } elsif ($mage eq "viking sea wizard") {
	$race = "LVIKI";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "WEAT";
	$skill2wert = 90;
	$skill3 = "SWIN";
	$skill3wert = 90;
    } elsif ($mage eq "dervish fire sorcerer") {
	$race = "LDERV";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "FIRE";
	$skill2wert = 90;
	$skill3 = "WSHI";
	$skill3wert = 90;
    } elsif ($mage eq "dervish wind sorcerer") {
	$race = "LDERV";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "WEAT";
	$skill2wert = 90;
	$skill3 = "SSTO";
	$skill3wert = 90;
    } elsif ($mage eq "ffolk ice sorcerer") {
	$race = "LFFOL";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "ICE";
	$skill2wert = 90;
	$skill3 = "ESHI";
	$skill3wert = 90;
    } elsif ($mage eq "ffolk weather wizard") {
	$race = "LFFOL";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "WEAT";
	$skill2wert = 90;
	$skill3 = "CLEA";
	$skill3wert = 90;
    } elsif ($mage eq "cajun folk ice sorcerer") {
	$race = "LCAJU";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "ICE";
	$skill2wert = 90;
	$skill3 = "ESHI";
	$skill3wert = 90;
    } elsif ($mage eq "lowlander fire sorcerer") {
	$race = "LLOWL";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "FIRE";
	$skill2wert = 90;
	$skill3 = "WSHI";
	$skill3wert = 90;
    } elsif ($mage eq "lowlander weather wizard") {
	$race = "LLOWL";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "WEAT";
	$skill2wert = 90;
	$skill3 = "CLEA";
	$skill3wert = 90;
    } elsif ($mage eq "gnome fire sorcerer") {
	$race = "LGNOM";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "FIRE";
	$skill2wert = 90;
	$skill3 = "WSHI";
	$skill3wert = 90;
    } elsif ($mage eq "gnome ice sorcerer") {
	$race = "LGNOM";
	$skill1 = "FORC";
	$skill1wert = 90;
	$skill2 = "ICE";
	$skill2wert = 90;
	$skill3 = "ESHI";
	$skill3wert = 90;
    } elsif ($mage eq "barbarian priest") {
	$race = "LBARB";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "MIND";
	$skill2wert = 90;
	$skill3 = "SSHI";
	$skill3wert = 90;
    } elsif ($mage eq "barbarian enchanter") {
	$race = "LBARB";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "FARS";
	$skill2wert = 90;
	$skill3 = "MREA";
	$skill3wert = 90;
    } elsif ($mage eq "tribesman priest") {
	$race = "LTRIB";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "MIND";
	$skill2wert = 90;
	$skill3 = "SSHI";
	$skill3wert = 90;
    } elsif ($mage eq "tribesman illusionist") {
	$race = "LTRIB";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "ILLU";
	$skill2wert = 90;
	$skill3 = "INVI";
	$skill3wert = 90;
    } elsif ($mage eq "tribesman inquisitor") {
	$race = "LTRIB";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "ILLU";
	$skill2wert = 90;
	$skill3 = "TRUE";
	$skill3wert = 90;
    } elsif ($mage eq "darkman priest") {
	$race = "LDARK";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "MIND";
	$skill2wert = 90;
	$skill3 = "SSHI";
	$skill3wert = 90;
    } elsif ($mage eq "darkman enchanter") {
	$race = "LDARK";
	$skill1 = "SPIR";
	$skill1wert = 90;
	$skill2 = "FARS";
	$skill2wert = 90;
	$skill3 = "MREA";
	$skill3wert = 90;
    } elsif ($mage eq "highlander runesmith") {
	$race = "LHIGH";
	$skill1 = "PATT";
	$skill1wert = 90;
	$skill2 = "ARTI";
	$skill2wert = 90;
	$skill3 = "EARM";
	$skill3wert = 45;
	$skill4 = "EWEA";
	$skill4wert = 45;
    } elsif ($mage eq "imperial gate magician") {
	$race = "LIMPE";
	$skill1 = "PATT";
	$skill1wert = 90;
	$skill2 = "GATE";
	$skill2wert = 90;
	$skill3 = "CGAT";
	$skill3wert = 90;
    } elsif ($mage eq "elf druid") {
	$race = "LELF";
	$skill1 = "PATT";
	$skill1wert = 90;
	$skill2 = "EART";
	$skill2wert = 90;
	$skill3 = "MHEA";
	$skill3wert = 90;
    } elsif ($mage eq "elf wolf master") {
	$race = "LELF";
	$skill1 = "PATT";
	$skill1wert = 90;
	$skill2 = "EART";
	$skill2wert = 90;
	$skill3 = "WOLF";
	$skill3wert = 90;
    } elsif ($mage eq "elf bird master") {
	$race = "LELF";
	$skill1 = "PATT";
	$skill1wert = 90;
	$skill2 = "EART";
	$skill2wert = 90;
	$skill3 = "BIRD";
	$skill3wert = 90;
    } elsif ($mage eq "dwarf runesmith") {
	$race = "LDWAR";
	$skill1 = "PATT";
	$skill1wert = 90;
	$skill2 = "ARTI";
	$skill2wert = 90;
	$skill3 = "EARM";
	$skill3wert = 45;
	$skill4 = "EWEA";
	$skill4wert = 45;
    }
    if ($okay ne "nein"){
	$newline = "Loc: ".$location1{$faction}." ".$location2{$faction}." ".$location3{$faction}."\n";
	$newline .= "NewUnit: ".$newnumber."\n";
	$newline .= "Item: gm".$newnumber." 1 ".$race."\n";
	$newline .= "Item: gm".$newnumber." 1 BSTA\n";
	$newline .= "Item: gm".$newnumber." 1 GARM\n";
	$newline .= "Skill: gm".$newnumber." ".$skill1." ".$skill1wert."\n";
	$newline .= "Skill: gm".$newnumber." ".$skill2." ".$skill2wert."\n";
	$newline .= "Skill: gm".$newnumber." ".$skill3." ".$skill3wert."\n";
	if ($skill4 ne ""){
	    $newline .= "Skill: gm".$newnumber." ".$skill4." ".$skill4wert."\n";
	}
	$newline .= "Order: gm".$newnumber." AVOID 1\n";
	if ($name eq "") {
	  $newline .= "Order: gm".$newnumber." NAME UNIT \"".$mage."\"\n";
	} else {
	  $newline .= "Order: gm".$newnumber." NAME UNIT \"".$name."\"\n";
	}
	if ($descript eq "") {
	  $newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"a new mystic ally\"\n";
	} else {
	  $newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"".$descript."\"\n";
	}
	$entry{$faction} .= $newline;
	$reward{$faction} = $reward{$faction} - 100;
	print "<p>a $mage hastening to your side.\n";
    }
}

elsif ($type eq "animal"){
    $okay = "nein";
    push(@animals,PONY);
    push(@animals,MWOL);
    push(@animals,MSPI);
    push(@animals,HORS);
    push(@animals,CAME);
    push(@animals,MLIZ);
    push(@animals,MEAG);
    push(@animals,MBAT);
    push(@animals,ELEP);
    push(@animals,MAMM);
    push(@animals,MOLE);
    push(@animals,WING);
    push(@animals,MFLI);
    push(@animals,WARG);
    push(@animals,WHOR);
    push(@animals,WLIZ);
    push(@animals,HARY);
    push(@animals,GCEN);
    push(@animals,CHIM);
    push(@animals,WMAM);
    push(@animals,WALF);
    push(@animals,FLIZ);
    push(@animals,HUND);
    push(@animals,CHAR);
    push(@animals,BLIZ);
    push(@animals,UNIC);
    push(@animals,DRAK);
    push(@animals,GRIF);
    push(@animals,WELE);
    push(@animals,BEHE);
    push(@animals,FDRA);
    if ($alignment{$faction} eq "evil"){
	$race = "RGOBL";
	$name2 = "Loki";
    } else {
	$race = "RHUMA";
	$name2 = "Hermes";
    }
    $newline = "Loc: ".$location1{$faction}." ".$location2{$faction}." ".$location3{$faction}."\n";
    $newline .= "NewUnit: ".$newnumber."\n";
    $newline .= "Item: gm".$newnumber." 1 ".$race."\n";
    $newline .= "Order: gm".$newnumber." AVOID 1\n";
    if ($name eq "") {
      $newline .= "Order: gm".$newnumber." NAME UNIT \"".$name2."\"\n";
    } else {
      $newline .= "Order: gm".$newnumber." NAME UNIT \"".$name."\"\n";
    }
    if ($descript eq "") {
      $newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"a messenger from the gods delivering your reward\"\n";
    } else {
      $newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"".$descript."\"\n";
    }
    foreach $animal (@animals){
	$name1 = "amount".$animal;
	$name2 = "cost".$animal;
	if ($$name1 ne ""){
	    $newline .= "Item: gm".$newnumber." ".$$name1." ".$animal."\n";
	    $cost = $cost + ($$name1 * $$name2);
	    $okay = "ja";
	}
    }
	if (($cost + 2) > $reward{$faction}){
		$okay = "nein";
		print "<p>You have ordered more than you can afford.\n";
	}
	if ($okay eq "ja"){
	$cost = $cost + 2;
	$entry{$faction} .= $newline;
	$reward{$faction} = $reward{$faction} - $cost;
	print "<p>Your creatures will be delivered soon.\n";
    } else {
	print "<p>You have to select at least one creature.\n";
    }
}

elsif ($type eq "weapon"){
    $okay = "nein";
    push(@weapons,ELXB);
    push(@weapons,XBOW);
    push(@weapons,HXBO);
    push(@weapons,ESHB);
    push(@weapons,BOW);
    push(@weapons,LBOW);
    push(@weapons,ESPE);
    push(@weapons,BSPE);
    push(@weapons,WSPE);
    push(@weapons,EPIK);
    push(@weapons,HALB);
    push(@weapons,BARD);
    push(@weapons,ELAN);
    push(@weapons,BLAN);
    push(@weapons,WLAN);
    push(@weapons,EQUA);
    push(@weapons,BSTA);
    push(@weapons,WSTA);
    push(@weapons,ECLU);
    push(@weapons,EMAC);
    push(@weapons,BMAC);
    push(@weapons,WMAC);
    push(@weapons,EMST);
    push(@weapons,TSTA);
    push(@weapons,ESTA);
    push(@weapons,EBHA);
    push(@weapons,SKUL);
    push(@weapons,WHAM);
    push(@weapons,EBAX);
    push(@weapons,CLEA);
    push(@weapons,WAXE);
    push(@weapons,EDAG);
    push(@weapons,EPDA);
    push(@weapons,MDAG);
    push(@weapons,ADAG);
    push(@weapons,ESAB);
    push(@weapons,EPEE);
    push(@weapons,FOIL);
    push(@weapons,ESWO);
    push(@weapons,EBSW);
    push(@weapons,ELSW);
    push(@weapons,MSWO);
    push(@weapons,MBSW);
    push(@weapons,MLSW);
    push(@weapons,ASWO);
    push(@weapons,ABSW);
    push(@weapons,ALSW);
    push(@weapons,ECLA);
    push(@weapons,ELAR);
    push(@weapons,ECAR);
    push(@weapons,ESCA);
    push(@weapons,EPLA);
    push(@weapons,BPLA);
    push(@weapons,MCAR);
    push(@weapons,MSCA);
    push(@weapons,MPLA);
    push(@weapons,ACAR);
    push(@weapons,ASCA);
    push(@weapons,APLA);
    if ($alignment{$faction} eq "evil"){
	$race = "RGOBL";
	$name2 = "Loki";
    } else {
	$race = "RHUMA";
	$name2 = "Hermes";
    }
    $newline = "Loc: ".$location1{$faction}." ".$location2{$faction}." ".$location3{$faction}."\n";
    $newline .= "NewUnit: ".$newnumber."\n";
    $newline .= "Item: gm".$newnumber." 1 ".$race."\n";
    $newline .= "Order: gm".$newnumber." AVOID 1\n";
    if ($name eq "") {
      $newline .= "Order: gm".$newnumber." NAME UNIT \"".$name2."\"\n";
    } else {
      $newline .= "Order: gm".$newnumber." NAME UNIT \"".$name."\"\n";
    }
    if ($descript eq "") {
      $newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"a messenger from the gods delivering your reward\"\n";
    } else {
      $newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"".$descript."\"\n";
    }
    foreach $weapon (@weapons){
	$name1 = "amount".$weapon;
	$name2 = "cost".$weapon;
	if ($$name1 ne ""){
	    $newline .= "Item: gm".$newnumber." ".$$name1." ".$weapon."\n";
	    $cost = $cost + ($$name1 * $$name2);
	    $okay = "ja";
	}
    }
	if (($cost + 2) > $reward{$faction}){
		$okay = "nein";
		print "<p>You have ordered more than you could afford.\n";
	}
    if ($okay eq "ja"){
	$cost = $cost + 2;
	$entry{$faction} .= $newline;
	$reward{$faction} = $reward{$faction} - $cost;
	print "<p>Your arms and armour will be delivered soon.\n";
    } else {
	print "<p>You have to select at least one item.\n";
    }
} 
elsif ($type eq "hero"){
    $newline = "Loc: ".$location1{$faction}." ".$location2{$faction}." ".$location3{$faction}."\n";
    $newline .= "NewUnit: ".$newnumber."\n";
    $newline .= "Item: gm".$newnumber." 1 HERO\n";
    $newline .= "Order: gm".$newnumber." AVOID 1\n";
    if ($name eq "") {
      $newline .= "Order: gm".$newnumber." NAME UNIT \"Crown Prince\"\n";
    } else {
      $newline .= "Order: gm".$newnumber." NAME UNIT \"".$name."\"\n";
    }
    if ($descript eq "") {
      $newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"assuming command\"\n";
    } else {
      $newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"".$descript."\"\n";
    }
    $entry{$faction} .= $newline;
    $reward{$faction} = $reward{$faction} - 50;
    $herocount{$faction}++;
    print "<p>Your new leader will enter the world soon.\n";
} 
elsif ($type eq "magicitem"){
    $okay = "nein";
    push(@magicitems,BOOT);
    push(@magicitems,CARP);
    push(@magicitems,PORT);
    push(@magicitems,WMAS);
    push(@magicitems,AOBS);
    push(@magicitems,FCHA);
    push(@magicitems,COPR);
    push(@magicitems,AMPR);
    push(@magicitems,SHST);
    push(@magicitems,INVP);
    push(@magicitems,CRIN);
    push(@magicitems,RINV);
    push(@magicitems,AORS);
    push(@magicitems,GTRU);
    push(@magicitems,AMTS);
    push(@magicitems,RWAR);
    push(@magicitems,MAST);
    push(@magicitems,ICSW);
    push(@magicitems,SEAT);
    push(@magicitems,FSWO);
    push(@magicitems,RUNE);
    push(@magicitems,BLSW);
    push(@magicitems,BCLO);
    push(@magicitems,CLOA);
    push(@magicitems,STAS);
    push(@magicitems,STPO);
    push(@magicitems,STFL);
    push(@magicitems,STCO);
    push(@magicitems,STAI);
    push(@magicitems,STAM);
    push(@magicitems,STAF);
    push(@magicitems,RISE);
    push(@magicitems,RICP);
    push(@magicitems,RIHO);
    push(@magicitems,RIAI);
    push(@magicitems,RIAR);
    push(@magicitems,AMHC);
    push(@magicitems,AMAR);
    push(@magicitems,AMDO);
    push(@magicitems,AMCA);
    push(@magicitems,AMSM);
    push(@magicitems,HECH);
    push(@magicitems,FUCH);
    push(@magicitems,VECH);
    push(@magicitems,BECH);
    push(@magicitems,FICH);
    push(@magicitems,WICH);
    push(@magicitems,HLCH);
    push(@magicitems,ORCH);
    push(@magicitems,WOCH);
    push(@magicitems,ROCH);
    push(@magicitems,PTCH);
    if ($alignment{$faction} eq "evil"){
	$race = "RGOBL";
	$name2 = "Loki";
    } else {
	$race = "RHUMA";
	$name2 = "Hermes";
    }
    $newline = "Loc: ".$location1{$faction}." ".$location2{$faction}." ".$location3{$faction}."\n";
    $newline .= "NewUnit: ".$newnumber."\n";
    $newline .= "Item: gm".$newnumber." 1 ".$race."\n";
    $newline .= "Order: gm".$newnumber." AVOID 1\n";
    if ($name eq "") {
      $newline .= "Order: gm".$newnumber." NAME UNIT \"".$name2."\"\n";
    } else {
      $newline .= "Order: gm".$newnumber." NAME UNIT \"".$name."\"\n";
    }
    if ($descript eq "") {
      $newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"a messenger from the gods delivering your reward\"\n";
    } else {
      $newline .= "Order: gm".$newnumber." DESCRIBE UNIT \"".$descript."\"\n";
    }
    foreach $magicitem (@magicitems){
	$name3 = "amount".$magicitem;
	$name4 = "cost".$magicitem;
	if ($$name3 ne ""){
	    $newline .= "Item: gm".$newnumber." ".$$name3." ".$magicitem."\n";
	    $cost = $cost + ($$name3 * $$name4);
	    $okay = "ja";
	}
    }
	if (($cost + 2) > $reward{$faction}){
		$okay = "nein";
		print "<p>You have ordered more than you could afford.\n";
	}
    if ($okay eq "ja"){
	$cost = $cost + 2;
	$entry{$faction} .= $newline;
	$reward{$faction} = $reward{$faction} - $cost;
	print "<p>Your magic items will be delivered soon.\n";
    } else {
	print "<p>You have to select at least one item.\n";
    }

}

if ($name ne "") {
  $reward{$faction} = $reward{$faction} + 1;
  $silver{$fact} = $silver{$fact} + 10;
}
if ($descript ne "") {
  $reward{$faction} = $reward{$faction} + 1;
  $silver{$fact} = $silver{$fact} + 10;
}

#Saving the players.in again
if ($password{$faction} ne $passwd) {
  print "<p>wrong password given!!!!!<br>Cannot save the rewards.<br>Please try again\n";
} else {
  if ($okay eq "ja"){
  open(NEW, ">$pin");
  foreach $fact (@factions){
      print NEW $entry{$fact};
	  next if ($fact == 0);
	  print NEW "Hero: ".$herocount{$fact}."\n";
      print NEW "Location: ".$location1{$fact}." ".$location2{$fact}." ".$location3{$fact}."\n" if ($fact > 0);
      if ($silver{$fact} > 0) {
	print NEW "Reward: ".$silver{$fact}." SILV\n";
      }
      print NEW "Timespunkte: ".$reward{$fact}."\n" if ($fact > 0);
  }
  close(NEW);
  print "<p>Reward stored.\n";
  }
}

print "</body></html>\n";
