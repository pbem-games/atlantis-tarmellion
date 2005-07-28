#!/usr/bin/perl
# script to add city guards and special characters to settlements

#initializing variables
$option1 = $ARGV[0]; #which targets? either 100, 200, 300, 400 or all
$option2 = $ARGV[1]; #what to do? either new_leader, new_guards or new_skills
$option3 = $ARGV[2]; #how much? either X (number of leaders), XXX (number of additional guards), XXXB (number of maximum guards) or XXXdaysSKILL (study days of Skill) 

if (($option1 eq "") || ($option2 eq "") || ($option3 eq "")){
  print "Unbedingt Auswahl treffen!\n";
  exit;
}

$newnumber = 0;

$dir = "/home/tarmellion/code/tarmellion/";
$dir2 = $dir."script/";
#$dir = "/home/hgrimm/atlantis/cvs/tarmellion/";
$Faction = 0;
push(@factions, $Faction);

#specify the relative path to the players.in and players.in.new files
$pin = $dir."players.in";
$npin = $dir."newplayers.in";
$data = $dir2."village-database";
$datasave = $data.".save"; 
`cp $data $datasave`;

#parsing the players.in file to get the current stats
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
    #saving all listed data
    $entry{$Faction} .= $_."\n";
    #find the highest added unit number
    if (/^NewUnit:\s(\d*)/){
        ($dummy,$unitnumber) = split(/\s/,$_);
        if ($unitnumber > $newnumber){
            $newnumber = $unitnumber;
        }
    }
}
close(PIN);
#parsing the newplayers.in file to get the current stats
open(NPIN, "<$npin");
while (<NPIN>) {
    chomp;
    #find the highest added unit number
    if (/^NewUnit:\s(\d*)/){
        ($dummy,$unitnumber) = split(/\s/,$_);
        if ($unitnumber > $newnumber){
            $newnumber = $unitnumber;
        }
    }
}
close(NPIN);

$newnumber++;
$j = 0;
$y = 0;
$count = 0;
print "newnumber is $newnumber\nChanging ";
#adding new units

open(ALL, "<$data");
while (<ALL>) {
    chomp;
    ($coord, $name, $settlementrace, $leader, $guards, $killturn) = split(/\|/,$_);
   $oldguards = $guards;
   if ($option2 eq "new_guards"){
      if ($option3 =~/B/){
        $max_anzahl = $option3;
	chop($max_anzahl);
	if (($guards < $max_anzahl) && ($guards > 0)){
          $guards = $max_anzahl;
	}
      } else {
        $guards = $guards + $option3;
      }
    }
    $entry{$coord} = "$coord|$name|$settlementrace|$leader|$guards|$killturn\n";
    push(@coords,$coord);
    #Auswahl welche villages
    next if ($leader eq "keiner");
    if ($option1 ne "all"){
	next if ($oldguards > $option1);
    }
    $count++;
    print "$count ";
    #Rassenunterscheidung
    $Rrace = "RGOBL";
    $Lrace = "LHOBG";
    $racename = "Grulluk";
    $weapons = "ISWO";
    $armour = "LARM";
    if ($settlementrace eq "humans"){
	$Rrace = "RHUMA";
	$Lrace = "LHUMA";
	$racename = "Haldan",
	$weapons = "ISWO";
	$j++;
	$y++;
    } elsif ($settlementrace eq "vikings"){
	$Rrace = "RVIKI";
	$Lrace = "LVIKI";
	$racename = "Tjure";
	$weapons = "BAXE";
	$j--;
	$y++;
    } elsif ($settlementrace eq "nomads"){
	$Rrace = "RNOMA";
	$Lrace = "LDERV";
	$racename = "Achmad";
	$weapons = "SABR";
	$j++;
	$y--;
    } elsif ($settlementrace eq "barbarians"){
	$Rrace = "RBARB";
	$Lrace = "LBARB";
	$racename = "Klingor";
	$weapons = "MACE";
	$j--;
	$y--;
    } elsif ($settlementrace eq "clansmen"){
	$Rrace = "RCLAN";
	$Lrace = "LHIGH";
	$racename = "Hugh Campell";
	$weapons = "IBSW";
	$j++;
	$y++;
    } elsif ($settlementrace eq "imperials"){
	$Rrace = "RIMPE";
	$Lrace = "LIMPE";
	$racename = "Marcus Authenticus";
	$weapons = "ILSW";
	$j--;
	$y++;
    } elsif ($settlementrace eq "tribesmen"){
	$Rrace = "RTRIB";
	$Lrace = "LTRIB";
	$racename = "Ingala";
	$weapons = "SPEA";
	$j++;
	$y--;
    } elsif ($settlementrace eq "ffolks"){
	$Rrace = "RFFOL";
	$Lrace = "LFFOL";
	$racename = "Connor";
	$weapons = "QUAR";
	$j--;
	$y--;
    } elsif ($settlementrace eq "lizardmen"){
	$Rrace = "RLIZA";
	$Lrace = "LCAJU";
	$racename = "Marvin";
	$weapons = "IDAG";
	$j++;
	$y--;
    } elsif ($settlementrace eq "halfings"){
	$Rrace = "RHALF";
	$Lrace = "LLOWL";
	$racename = "Tasslebuff";
	$weapons = "PIKE";
	$j--;
	$y++;
    } elsif ($settlementrace eq "halfelves"){
	$Rrace = "RHELF";
	$Lrace = "LELF";
	$racename = "Silgarthos";
	$weapons = "SABR";
	$j++;
	$y++;
    } elsif ($settlementrace eq "darkmen"){
	$Rrace = "RDARK";
	$Lrace = "LDARK";
	$racename = "Melgor";
	$weapons = "MSTA";
	$j++;
	$y--;
    } elsif ($settlementrace eq "young gnomes"){
	$Rrace = "RYGNO";
	$Lrace = "LGNOM";
	$racename = "Patronak";
	$weapons = "IDAG";
	$j--;
	$y++;
    } elsif ($settlementrace eq "young dwarves"){
	$Rrace = "RYDWA";
	$Lrace = "LDWAR";
	$racename = "Gwerin Goldwing";
	$weapons = "BAXE";
	$j--;
	$y--;
    } elsif ($settlementrace eq "buccaneers"){
	$Rrace = "RBUCC";
	$Lrace = "LBUCC";
	$racename = "Jeremiah Black";
	$weapons = "BAXE";
	$j++;
	$y++;
    } elsif ($settlementrace eq "sandlings"){
	$Rrace = "RSAND";
	$Lrace = "LSAND";
	$racename = "Jussuf";
	$weapons = "SABR";
	$j--;
	$y++;
    } elsif ($settlementrace eq "gnolls"){
	$Rrace = "RGNOL";
	$Lrace = "LTROL";
	$racename = "Gnarish";
	$weapons = "BHAM";
	$j++;
	$y--;
    } elsif ($settlementrace eq "orcs"){
	$Rrace = "RORC";
	$Lrace = "LURUK";
	$racename = "Mishrak";
	$weapons = "IBSW";
	$j--;
	$y--;
    } elsif ($settlementrace eq "black keep followers"){
	$Rrace = "RBLAC";
	$Lrace = "LBLAC";
	$racename= "Tarratus";
	$weapons = "PIKE";
	$j++;
	$y++;
    } elsif ($settlementrace eq "headhunters"){
	$Rrace = "RHEAD";
	$Lrace = "LHEAD";
	$racename = "Missan";
	$weapons = "SPEA";
	$j++;
	$y--;
    } elsif ($settlementrace eq "ogre females"){
	$Rrace = "ROGRE";
	$Lrace = "LOGRE";
	$racename = "Kelmut";
	$weapons = "MACE";
	$j--;
	$y++;
    } elsif ($settlementrace eq "serpent people"){
	$Rrace = "RSERP";
	$Lrace = "LSERP";
	$racename = "Ssinshazz";
	$weapons = "IDAG";
	$j--;
	$y--;
    } elsif ($settlementrace eq "republicans"){
	$Rrace = "RREPU";
	$Lrace = "LREPU";
	$racename = "Sheldon";
	$weapons = "ILSW";
	$j++;
	$y++;
    } elsif ($settlementrace eq "amazons"){
	$Rrace = "RAMAZ";
	$Lrace = "LAMAZ";
	$racename = "Kitiara";
	$weapons = "SABR";
	$j--;
	$y++;
    } elsif ($settlementrace eq "young minotuars"){
	$Rrace = "RMINO";
	$Lrace = "LMINO";
	$racename = "Mortuk";
	$weapons = "BHAM";
	$j++;
	$y--;
    } elsif ($settlementrace eq "drow males"){
	$Rrace = "RDROW";
	$Lrace = "LDROW";
	$racename = "Eanthor Dwaeru";
	$weapons = "SABR";
	$j--;
	$y--;
    } elsif ($settlementrace eq "young duergars"){
	$Rrace = "RDUER";
	$Lrace = "LDUER";
	$racename = "Gargin Bloodbath";
	$weapons = "BAXE";
	$j++;
	$y++;
    }
    
    $entry{1} .= "Loc: ".$coord."\n";
    
    
    
#Einfügen Anführer
    if ($option2 eq "new_leader"){
	$i = 0;
	while ($i < $option3){
	    $i++;
	    print "Adding ".$i.". Leader\n";
	    $entry{1} .= "NewUnit: ".$newnumber."\n";
	    $entry{1} .= "Item: gm".$newnumber." 1 HERO \n";
	    $entry{1} .= "Order: gm".$newnumber." BEHIND 1\n";
	    $entry{1} .= "Order: gm".$newnumber." REVEAL FACTION\n";
	    $entry{1} .= "Skill: gm".$newnumber." OBSE 840\n";
	    #10 Anführertypen
	    $mody = ($y % 10);
	    if ($mody == 0) {
		$entry{1} .= "Item: gm".$newnumber." 1 MAST\n";
		$entry{1} .= "Item: gm".$newnumber." 1 GARM\n";
		$entry{1} .= "Skill: gm".$newnumber." FORC 180\n";
		$entry{1} .= "Skill: gm".$newnumber." FIRE 180\n";
		$entry{1} .= "Skill: gm".$newnumber." TACT 90\n";
		$entry{1} .= "Order: gm".$newnumber." COMBAT FIRE\n";
	    } elsif ($mody == 1) {
		$entry{1} .= "Item: gm".$newnumber." 1 MAST\n";
		$entry{1} .= "Item: gm".$newnumber." 1 GARM\n";
		$entry{1} .= "Skill: gm".$newnumber." FORC 180\n";
		$entry{1} .= "Skill: gm".$newnumber." ICE 180\n";
		$entry{1} .= "Skill: gm".$newnumber." TACT 90\n";
		$entry{1} .= "Order: gm".$newnumber." COMBAT ICE\n";
	    } elsif ($mody == 2) {
		$entry{1} .= "Item: gm".$newnumber." 1 MAST\n";
		$entry{1} .= "Item: gm".$newnumber." 1 GARM\n";
		$entry{1} .= "Skill: gm".$newnumber." FORC 180\n";
		$entry{1} .= "Skill: gm".$newnumber." FSHI 180\n";
		$entry{1} .= "Skill: gm".$newnumber." TACT 90\n";
		$entry{1} .= "Order: gm".$newnumber." COMBAT FSHI\n";
	    } elsif ($mody == 3) {
		$entry{1} .= "Item: gm".$newnumber." 1 MAST\n";
		$entry{1} .= "Item: gm".$newnumber." 1 GARM\n";
		$entry{1} .= "Skill: gm".$newnumber." FORC 180\n";
		$entry{1} .= "Skill: gm".$newnumber." ESHI 180\n";
		$entry{1} .= "Skill: gm".$newnumber." TACT 90\n";
		$entry{1} .= "Order: gm".$newnumber." COMBAT ESHI\n";
	    } elsif ($mody == 4) {
		$entry{1} .= "Item: gm".$newnumber." 1 MAST\n";
		$entry{1} .= "Item: gm".$newnumber." 1 GARM\n";
		$entry{1} .= "Skill: gm".$newnumber." FORC 180\n";
		$entry{1} .= "Skill: gm".$newnumber." WSHI 180\n";
		$entry{1} .= "Skill: gm".$newnumber." TACT 90\n";
		$entry{1} .= "Order: gm".$newnumber." COMBAT WSHI\n";
	    } elsif ($mody == 5) {
		$entry{1} .= "Item: gm".$newnumber." 1 MAST\n";
		$entry{1} .= "Item: gm".$newnumber." 1 GARM\n";
		$entry{1} .= "Skill: gm".$newnumber." SPIR 180\n";
		$entry{1} .= "Skill: gm".$newnumber." SSHI 180\n";
		$entry{1} .= "Skill: gm".$newnumber." TACT 90\n";
		$entry{1} .= "Order: gm".$newnumber." COMBAT SSHI\n";
	    } elsif ($mody == 6) {
		$entry{1} .= "Item: gm".$newnumber." 1 MAST\n";
		$entry{1} .= "Item: gm".$newnumber." 1 GARM\n";
		$entry{1} .= "Skill: gm".$newnumber." SPIR 180\n";
		$entry{1} .= "Skill: gm".$newnumber." MIND 180\n";
		$entry{1} .= "Skill: gm".$newnumber." TACT 90\n";
		$entry{1} .= "Order: gm".$newnumber." COMBAT MIND\n";
	    } elsif ($mody == 7) {
		$entry{1} .= "Item: gm".$newnumber." 1 MSWO\n";
		$entry{1} .= "Item: gm".$newnumber." 1 MPLA\n";
		$entry{1} .= "Item: gm".$newnumber." 1 HXBO\n";
		$entry{1} .= "Item: gm".$newnumber." 1 SHST\n";
		$entry{1} .= "Skill: gm".$newnumber." COMB 180\n";
		$entry{1} .= "Skill: gm".$newnumber." XBOW 450\n";
		$entry{1} .= "Skill: gm".$newnumber." TACT 90\n";
	    } elsif ($mody == 8) {
		$entry{1} .= "Item: gm".$newnumber." 1 MSWO\n";
		$entry{1} .= "Item: gm".$newnumber." 1 MPLA\n";
		$entry{1} .= "Item: gm".$newnumber." 1 LBOW\n";
		$entry{1} .= "Item: gm".$newnumber." 1 COPR\n";
		$entry{1} .= "Skill: gm".$newnumber." COMB 180\n";
		$entry{1} .= "Skill: gm".$newnumber." ARCH 450\n";
		$entry{1} .= "Skill: gm".$newnumber." TACT 90\n";
	    } elsif ($mody == 9) {
		$entry{1} .= "Item: gm".$newnumber." 1 MSWO\n";
		$entry{1} .= "Item: gm".$newnumber." 1 MPLA\n";
		$entry{1} .= "Item: gm".$newnumber." 1 XBOW\n";
		$entry{1} .= "Item: gm".$newnumber." 1 AMPR\n";
		$entry{1} .= "Skill: gm".$newnumber." COMB 180\n";
		$entry{1} .= "Skill: gm".$newnumber." ARCH 180\n";
		$entry{1} .= "Skill: gm".$newnumber." TACT 180\n";
	    }
	    #10 Anführernamen und Beschreibungen
	    $modj = ($j % 10);
	    if ($modj == 0) {
		$entry{1} .= 'Order: gm'.$newnumber.' NAME UNIT "King '.$racename.'"';
		$entry{1} .= "\n";
		$entry{1} .= 'Order: gm'.$newnumber.' DESCRIBE UNIT "living in luxury while the population starves"';
		$entry{1} .= "\n";
	    } elsif ($modj == 1) {
		$entry{1} .= 'Order: gm'.$newnumber.' NAME UNIT "Clan Chief '.$racename.'"';
		$entry{1} .= "\n";
		$entry{1} .= 'Order: gm'.$newnumber.' DESCRIBE UNIT "oppresses the masses"';
		$entry{1} .= "\n";
	    } elsif ($modj == 2) {
		$entry{1} .= 'Order: gm'.$newnumber.' NAME UNIT "Bandit Leader '.$racename.'"';
		$entry{1} .= "\n";
		$entry{1} .= 'Order: gm'.$newnumber.' DESCRIBE UNIT "pillaging the settlement"';
		$entry{1} .= "\n";
	    } elsif ($modj == 3) {
		$entry{1} .= 'Order: gm'.$newnumber.' NAME UNIT "'.$racename.'"';
		$entry{1} .= "\n";
		$entry{1} .= 'Order: gm'.$newnumber.' DESCRIBE UNIT "making obscure experiments"';
		$entry{1} .= "\n";
	    } elsif ($modj == 4) {
		$entry{1} .= 'Order: gm'.$newnumber.' NAME UNIT "Master '.$racename.'"';
		$entry{1} .= "\n";
		$entry{1} .= 'Order: gm'.$newnumber.' DESCRIBE UNIT "hunting the females of the settlement"';
		$entry{1} .= "\n";
	    } elsif ($modj == 5) {
		$entry{1} .= 'Order: gm'.$newnumber.' NAME UNIT "'.$racename.' the Great"';
		$entry{1} .= "\n";
		$entry{1} .= 'Order: gm'.$newnumber.' DESCRIBE UNIT "a corrupt slimy individual"';
		$entry{1} .= "\n";
	    } elsif ($modj == 6) {
		$entry{1} .= 'Order: gm'.$newnumber.' NAME UNIT "Invulnerable '.$racename.'"';
		$entry{1} .= "\n";
		$entry{1} .= 'Order: gm'.$newnumber.' DESCRIBE UNIT "a real loudmouth"';
		$entry{1} .= "\n";
	    } elsif ($modj == 7) {
		$entry{1} .= 'Order: gm'.$newnumber.' NAME UNIT "Baron '.$racename.'"';
		$entry{1} .= "\n";
		$entry{1} .= 'Order: gm'.$newnumber.' DESCRIBE UNIT "if he does not like you, you are as good as dead"';
		$entry{1} .= "\n";
	    } elsif ($modj == 8) {
		$entry{1} .= 'Order: gm'.$newnumber.' NAME UNIT "'.$racename.' the Rich"';
		$entry{1} .= "\n";
		$entry{1} .= 'Order: gm'.$newnumber.' DESCRIBE UNIT "better keep your wealth hidden or he will take it"';
		$entry{1} .= "\n";
	    } elsif ($modj == 9) {
		$entry{1} .= 'Order: gm'.$newnumber.' NAME UNIT "Citizen '.$racename.'"';
		$entry{1} .= "\n";
		$entry{1} .= 'Order: gm'.$newnumber.' DESCRIBE UNIT "he is more equal than the rest"';
		$entry{1} .= "\n";
	    }
	    $y = $j + $i;
	    $j = $y + $i;
	    $newnumber++;
	}
     }
	
     if ($option2 eq "new_guards"){
	    if ($option3 =~ /B/){
	        $max_anzahl = $option3;
		chop($max_anzahl);
		$anzahl = $max_anzahl - $oldguards;
	    } else {
		$anzahl = $option3;
	    }
	    print "Adding $anzahl soldiers\n";
	    $geld = $anzahl * 250;
#Einfügen Truppen
	    $entry{1} .= "NewUnit: ".$newnumber."\n";
	    $entry{1} .= "Item: gm".$newnumber." ".$anzahl." ".$Lrace."\n";
	    $entry{1} .= "Item: gm".$newnumber." ".$anzahl." ".$weapons."\n";
	    $entry{1} .= "Item: gm".$newnumber." ".$anzahl." ".$armour."\n";
	    $entry{1} .= "Item: gm".$newnumber." ".$geld." SILV\n";
	    $entry{1} .= 'Order: gm'.$newnumber.' NAME UNIT "City Guard"';
	    $entry{1} .= "\n";
	    $entry{1} .= 'Order: gm'.$newnumber.' DESCRIBE UNIT "reinforcements for '.$racename.'"';
	    $entry{1} .= "\n";
	    $entry{1} .= "Order: gm".$newnumber." HOLD 1\n";
	    $entry{1} .= "Order: gm".$newnumber." REVEAL FACTION\n";
	    $entry{1} .= "Skill: gm".$newnumber." COMB 180\n";

	    $newnumber++;
	    
     }
	
     if ($option2 eq "new_skills"){
	    ($days, $skill) = split(/days/,$option3);
	    print "Boosting $skill to $days days\n";
	    $entry{1} .= "Skill: ".$leader." ".$skill." ".$days."\n";
     }
}
close(ALL);
  
print "villages\n";

#writing new players.in
open(NEW, ">$pin")|| trouble opening players.in file;
foreach $Faction (@factions){
    print NEW $entry{$Faction};
}
close(NEW);

#writing new village-database
open(NEW2, ">$data");
foreach $dat (@coords){
   print NEW2 $entry{$dat};
}
close(NEW2);

