#!/usr/bin/perl
# script to create a new faction and to choose the starting location
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

#
# Neues CGI-Element
#
$q = new CGI;

@fields = $q->param();

foreach $field (@fields) {
  $$field = $q->param($field);
}

#specify the relative path to the newplayers.in file
$npin = "/home/tarmellion/code/tarmellion/newplayers.in";
$pin = "/home/tarmellion/code/tarmellion/players.in";

#initializing variables
@primechoices = "";
@choices = "";

#preparing the html page
print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE></HEAD>\n";
print "<body background=/images/background.jpg>\n";
print "<h2>Signup for the game of Tarmellion - step 3</h2>\n";

#parsing the newplayers.in file to check double signups
$newnumber = 0;
$emails = "";
$names = "";
open(NPIN, "<$npin");
while (<NPIN>) {
    chomp;
    #find all signed up players
    if (/^Email:\s(\.*)/){
	($dummy,$email) = split(/\s/,$_);
	$emails .= $email;
    }
    if (/^Name:\s(\.*)/){
	($dummy,$name) = split(/:/,$_);
	$names .= $name;
    }
}
close(NPIN);
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    #find all signed up players
    if (/^Email:\s(\.*)/){
	($dummy,$email) = split(/\s/,$_);
	$emails .= $email;
    }
    if (/^Name:\s(\.*)/){
	($dummy,$name) = split(/:/,$_);
	$names .= $name;
    }
}
close(PIN);

#checking if everything is correct
if ($factionname eq ""){
    print "<p>You have to fill in a name for your faction!\n";
    $okay = "no";
} elsif ($youremail eq "") {
    print "<p>You have to fill in an email address for your faction!\n";
    $okay = "no";
} elsif ($yourpassword eq "") {
    print "<p>You have to choose a password for your faction!\n";
    $okay = "no";
} elsif ($emails =~ /$youremail/) {
    print "<p>You have already signed up for this game.<br>It is not possible to play more than one faction.<br>If you just want to change your signup, please contact the gamemaster personally at tarmellion\@herobane.de\n";
} elsif ($names =~ /$factionname/) {
    print "<p>You have already signed up for this game.<br>It is not possible to play more than one faction.<br>If you just want to change your signup, please contact the gamemaster personally at tarmellion\@herobane.de\n";
} elsif ($startingrace eq "") {
    print "<p>You have to choose a race for your faction!\n";
    $okay = "no";
} elsif ($profession eq "") {
    print "<p>You have to choose a profession for your starting character!\n";
    $okay = "no";
} else {

    print "<p>Now you have to decide in which starting location your hero shall enter the world of Tarmellion. Each combination of race and profession should have at least one 'prime choice' starting location that is most fitting for your choice and up to 5 other possible locations that are also possible. You can choose freely between those possibilities, but choosing one of the 'prime' locations ensures optimal starting conditions for your game and role playing.\n";

if ($startingrace eq "human") {
    push(@choices,"Ramish Empire");
    push(@choices,"Halfing Democracy");
    push(@choices,"Viking Confederacy");
    push(@choices,"Darkman Kingdom");
    if ($profession eq "warrior"){
	push(@primechoices,"Ramish Empire");
	push(@primechoices,"Darkman Kingdom");
    } elsif ($profession eq "noble"){
	push(@primechoices,"Ramish Empire");
	push(@primechoices,"Darkman Kingdom");
    } elsif ($profession eq "officer"){
	push(@primechoices,"Ramish Empire");
	push(@primechoices,"Darkman Kingdom");
    } elsif ($profession eq "seaman"){
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "mage"){
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "barbarian") {
    if ($profession eq "warrior"){
	push(@choices,"Ramish Empire");
	push(@choices,"Halfing Democracy");
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "noble"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "officer"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "seaman"){
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "mage"){
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "tribesman") {
    if ($profession eq "warrior"){
	push(@choices,"Ramish Empire");
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
	push(@choices,"Viking Confederacy");
    } elsif ($profession eq "noble"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "officer"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "seaman"){
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Silsalon");
	push(@choices,"Viking Confederacy");
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Silsalon");
	push(@choices,"Viking Confederacy");
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "mage"){
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "viking") {
    push(@choices,"Viking Confederacy");
    push(@primechoices,"Viking Confederacy");
    if ($profession eq "warrior"){
	push(@choices,"Ramish Empire");
    } elsif ($profession eq "noble"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "officer"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "seaman"){
	push(@choices,"Ramish Empire");
    } elsif ($profession eq "merchant"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "mage"){
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "clansman") {
    if ($profession eq "warrior"){
	push(@choices,"Ramish Empire");
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "noble"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "officer"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "seaman"){
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Viking Confederacy");
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Viking Confederacy");
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "mage"){
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "highlander") {
    if ($profession eq "warrior"){
	push(@choices,"Ramish Empire");
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "noble"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "officer"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "seaman"){
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Viking Confederacy");
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Viking Confederacy");
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "mage"){
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "ffolk") {
    push(@choices,"Silsalon");
    if ($profession eq "warrior"){
	push(@choices,"Ramish Empire");
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "noble"){
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "officer"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "seaman"){
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "mage"){
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "dervish") {
    if ($profession eq "warrior"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
	push(@choices,"Halfing Democracy");
    } elsif ($profession eq "noble"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "officer"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "seaman"){
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "mage"){
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "nomad") {
    if ($profession eq "warrior"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
	push(@choices,"Halfing Democracy");
    } elsif ($profession eq "noble"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "officer"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "seaman"){
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "mage"){
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "cajun folk") {
    push(@choices,"Viking Confederacy");
    if ($profession eq "warrior"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "noble"){
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "officer"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "seaman"){
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "mage"){
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "lizardman") {
    push(@choices,"Viking Confederacy");
    if ($profession eq "warrior"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "noble"){
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "officer"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    } elsif ($profession eq "seaman"){
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "mage"){
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "halfing") {
    push(@choices,"Halfing Democracy");
    push(@primechoices,"Halfing Democracy");
    if ($profession eq "warrior"){
	push(@choices,"Ramish Empire");
    } elsif ($profession eq "noble"){
	push(@choices,"Ramish Empire");
    } elsif ($profession eq "officer"){
	push(@choices,"Ramish Empire");
    } elsif ($profession eq "seaman"){
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Silsalon");
	push(@choices,"Ramish Empire");
    } elsif ($profession eq "artificer"){
	push(@choices,"Silsalon");
	push(@choices,"Ramish Empire");
    } elsif ($profession eq "mage"){
	push(@choices,"Ramish Empire");
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "lowlander") {
    push(@choices,"Halfing Democracy");
    push(@primechoices,"Halfing Democracy");
    if ($profession eq "warrior"){
	push(@choices,"Ramish Empire");
    } elsif ($profession eq "noble"){
	push(@choices,"Ramish Empire");
    } elsif ($profession eq "officer"){
	push(@choices,"Ramish Empire");
    } elsif ($profession eq "seaman"){
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Silsalon");
	push(@choices,"Ramish Empire");
    } elsif ($profession eq "artificer"){
	push(@choices,"Silsalon");
	push(@choices,"Ramish Empire");
    } elsif ($profession eq "mage"){
	push(@choices,"Ramish Empire");
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "imperial") {
    push(@primechoices,"Ramish Empire");
    push(@choices,"Ramish Empire");
    if ($profession eq "warrior"){
	push(@choices,"Halfing Democracy");
    } elsif ($profession eq "officer"){
	push(@choices,"Halfing Democracy");
    } elsif ($profession eq "seaman"){
	push(@choices,"Halfing Democracy");
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "mage"){
	push(@choices,"Halfing Democracy");
	push(@choices,"Silsalon");
	push(@primechoices,"Silsalon");
    }
} elsif ($startingrace eq "elf") {
    push(@choices,"Silsalon");
    push(@primechoices,"Silsalon");
    if ($profession eq "warrior"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "officer"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "seaman"){
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "halfelf") {
    push(@choices,"Silsalon");
    push(@primechoices,"Silsalon");
    if ($profession eq "warrior"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "officer"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "seaman"){
	push(@choices,"Viking Confederacy");
	push(@primechoices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Halfing Democracy");
	push(@primechoices,"Halfing Democracy");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
	push(@primechoices,"Ramish Empire");
    }
} elsif ($startingrace eq "darkman") {
    push(@choices,"Darkman Kingdom");
    push(@primechoices,"Darkman Kingdom");
    if ($profession eq "warrior"){
	push(@choices,"Dwarven Kingdom");
    } elsif ($profession eq "officer"){
	push(@choices,"Dwarven Kingdom");
    } elsif ($profession eq "seaman"){
	push(@choices,"Dwarven Kingdom");
	push(@primechoices,"Dwarven Kingdom");
	push(@choices,"Viking Confederacy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Dwarven Kingdom");
    } elsif ($profession eq "artificer"){
	push(@choices,"Dwarven Kingdom");
    } elsif ($profession eq "mage"){
	push(@choices,"Dwarven Kingdom");
	push(@choices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Dwarven Kingdom");
	push(@choices,"Ramish Empire");
    }
} elsif ($startingrace eq "gnome") {
    push(@choices,"Darkman Kingdom");
    push(@choices,"Dwarven Kingdom");
    if ($profession eq "warrior"){
	push(@primechoices,"Darkman Kingdom");
    } elsif ($profession eq "noble"){
	push(@primechoices,"Darkman Kingdom");
    } elsif ($profession eq "officer"){
	push(@primechoices,"Darkman Kingdom");
    } elsif ($profession eq "seaman"){
	push(@primechoices,"Dwarven Kingdom");
    } elsif ($profession eq "merchant"){
	push(@primechoices,"Dwarven Kingdom");
    } elsif ($profession eq "artificer"){
	push(@primechoices,"Dwarven Kingdom");
    } elsif ($profession eq "mage"){
	push(@primechoices,"Darkman Kingdom");
	push(@primechoices,"Dwarven Kingdom");
	push(@choices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@primechoices,"Dwarven Kingdom");
	push(@choices,"Ramish Empire");
    }
} elsif ($startingrace eq "dwarf") {
    push(@choices,"Dwarven Kingdom");
    push(@primechoices,"Dwarven Kingdom");
    if ($profession eq "warrior"){
	push(@choices,"Darkman Kingdom");
    } elsif ($profession eq "merchant"){
	push(@choices,"Darkman Kingdom");
    } elsif ($profession eq "artificer"){
	push(@choices,"Darkman Kingdom");
    } elsif ($profession eq "mage"){
	push(@choices,"Darkman Kingdom");
	push(@choices,"Silsalon");
    } elsif ($profession eq "assassin"){
	push(@choices,"Ramish Empire");
    }
} elsif ($startingrace eq "buccaneer") {
    push(@choices,"Buccaneer Fellowship");
    push(@primechoices,"Buccaneer Fellowship");
    if ($profession eq "warrior"){
	push(@choices,"Black Keep");
    } elsif ($profession eq "noble"){
	push(@choices,"Black Keep");
    } elsif ($profession eq "officer"){
	push(@choices,"Black Keep");
    } elsif ($profession eq "seaman"){
	push(@choices,"Black Keep");
    } elsif ($profession eq "merchant"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "artificer"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "republican") {
    push(@choices,"Republic of Pelisham");
    push(@primechoices,"Republic of Pelisham");
    if ($profession eq "warrior"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "noble"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "officer"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "seaman"){
	push(@choices,"Buccaneer Fellowship");
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "merchant"){
	push(@choices,"Black Keep");
    } elsif ($profession eq "artificer"){
	push(@choices,"Black Keep");
    } elsif ($profession eq "mage"){
	push(@choices,"Black Keep");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "black keep leader") {
    push(@choices,"Black Keep");
    push(@primechoices,"Black Keep");
    if  ($profession eq "seaman"){
	push(@choices,"Buccaneer Fellowship");
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "merchant"){
	push(@choices,"Buccaneer Fellowship");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "artificer"){
	push(@choices,"Buccaneer Fellowship");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    }
} elsif ($startingrace eq "amazon") {
    push(@choices,"Amazon Nation");
    push(@primechoices,"Amazon Nation");
    if  ($profession eq "seaman"){
	push(@choices,"Buccaneer Fellowship");
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "merchant"){
	push(@choices,"Republic of Pelisham");
    } elsif ($profession eq "artificer"){
	push(@choices,"Republic of Pelisham");
    } elsif ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "ogre male") {
    push(@choices,"Amazon Nation");
    if ($profession eq "warrior"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "noble"){
	push(@choices,"Black Keep");
	push(@primechoices,"Amazon Nation");
    } elsif ($profession eq "officer"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "seaman"){
	push(@choices,"Buccaneer Fellowship");
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "merchant"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Amazon Nation");
    } elsif ($profession eq "artificer"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Amazon Nation");
    } elsif ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "goblin") {
    push(@choices,"Black Keep");
    push(@choices,"Duergar Kingdom");
    push(@choices,"Republic of Pelisham");
    if ($profession eq "warrior"){
	push(@choices,"Drow Matriarchy");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "noble"){
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "officer"){
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "seaman"){
	push(@choices,"Drow Matriarchy");
	push(@choices,"Buccaneer Fellowship");
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "merchant"){
	push(@choices,"Drow Matriarchy");
	push(@choices,"Amazon Nation");
	push(@primechoices,"Amazon Nation");
	push(@primechoices,"Duergar Kingdom");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "artificer"){
	push(@choices,"Drow Matriarchy");
	push(@choices,"Amazon Nation");
	push(@primechoices,"Amazon Nation");
	push(@primechoices,"Drow Matriarchy");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "mage"){
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Drow Matriarchy");
	push(@primechoices,"Drow Matriarchy");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "hobgoblin") {
    push(@choices,"Black Keep");
    push(@choices,"Duergar Kingdom");
    push(@choices,"Republic of Pelisham");
    if ($profession eq "warrior"){
	push(@choices,"Drow Matriarchy");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "noble"){
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "officer"){
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "seaman"){
	push(@choices,"Drow Matriarchy");
	push(@choices,"Buccaneer Fellowship");
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "merchant"){
	push(@choices,"Drow Matriarchy");
	push(@choices,"Amazon Nation");
	push(@primechoices,"Amazon Nation");
	push(@primechoices,"Duergar Kingdom");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "artificer"){
	push(@choices,"Drow Matriarchy");
	push(@choices,"Amazon Nation");
	push(@primechoices,"Amazon Nation");
	push(@primechoices,"Drow Matriarchy");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "mage"){
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Drow Matriarchy");
	push(@primechoices,"Drow Matriarchy");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "headhunter") {
    if ($profession eq "warrior"){
	push(@choices,"Amazon Nation");
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "noble"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "officer"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "seaman"){
	push(@choices,"Buccaneer Fellowship");
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "merchant"){
	push(@choices,"Amazon Nation");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "artificer"){
	push(@choices,"Amazon Nation");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "sandling") {
    push(@choices,"Republic of Pelisham");
    push(@primechoices,"Republic of Pelisham");
    if ($profession eq "warrior"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "noble"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "officer"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "seaman"){
	push(@choices,"Buccaneer Fellowship");
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "serpent people") {
    push(@choices,"Buccaneer Fellowship");
    if ($profession eq "warrior"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "noble"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "officer"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "seaman"){
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "merchant"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "artificer"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "troll") {
    if ($profession eq "warrior"){
	push(@choices,"Black Keep");
	push(@choices,"Buccaneer Fellowship");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "noble"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "officer"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "seaman"){
	push(@choices,"Buccaneer Fellowship");
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "merchant"){
	push(@choices,"Amazon Nation");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "artificer"){
	push(@choices,"Amazon Nation");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "gnoll") {
    if ($profession eq "warrior"){
	push(@choices,"Black Keep");
	push(@choices,"Buccaneer Fellowship");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "noble"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "officer"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "seaman"){
	push(@choices,"Buccaneer Fellowship");
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "merchant"){
	push(@choices,"Amazon Nation");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "artificer"){
	push(@choices,"Amazon Nation");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "orc") {
    if ($profession eq "warrior"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "noble"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "officer"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "seaman"){
	push(@choices,"Buccaneer Fellowship");
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "merchant"){
	push(@choices,"Amazon Nation");
	push(@choices,"Buccaneer Fellowship");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "artificer"){
	push(@choices,"Amazon Nation");
	push(@choices,"Buccaneer Fellowship");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "uruk") {
    if ($profession eq "warrior"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "noble"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "officer"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    } elsif ($profession eq "seaman"){
	push(@choices,"Buccaneer Fellowship");
	push(@primechoices,"Buccaneer Fellowship");
    } elsif ($profession eq "merchant"){
	push(@choices,"Amazon Nation");
	push(@choices,"Buccaneer Fellowship");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "artificer"){
	push(@choices,"Amazon Nation");
	push(@choices,"Buccaneer Fellowship");
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
	push(@primechoices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
	push(@primechoices,"Black Keep");
    }
} elsif ($startingrace eq "minotaur") {
    push(@choices,"Duergar Kingdom");
    push(@primechoices,"Duergar Kingdom");
    if ($profession eq "warrior"){
	push(@choices,"Drow Matriarchy");
    } elsif ($profession eq "seaman"){
	push(@choices,"Drow Matriarchy");
    } elsif ($profession eq "merchant"){
	push(@choices,"Drow Matriarchy");
    } elsif ($profession eq "artificer"){
	push(@choices,"Drow Matriarchy");
    } elsif ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
    }
} elsif ($startingrace eq "drow female") {
    push(@choices,"Drow Matriarchy");
    push(@primechoices,"Drow Matriarchy");
    if ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
    }
} elsif ($startingrace eq "duergar") {
    push(@choices,"Duergar Kingdom");
    push(@primechoices,"Duergar Kingdom");
    unless ($profession eq "noble"){
	push(@choices,"Drow Matriarchy");
    }
    if ($profession eq "mage"){
	push(@choices,"Republic of Pelisham");
    } elsif ($profession eq "assassin"){
	push(@choices,"Black Keep");
    }
}

    foreach $prime (@primechoices){
    	next if ($prime eq "");
	$primechoices .= $prime." : ".$primechoices;
    }
    chop($primechoices);

#writing the form to choose the starting city
    print "<table border=2>\n";
    print "<form action=/cgi-bin/tarmellion/signup2.pl method=POST>";
    print "<input name=factionname type=hidden value='$factionname'>\n";
    print "<input name=youremail type=hidden value='$youremail'>\n";
    print "<input name=yourpassword type=hidden value='$yourpassword'>\n";
    print "<input name=profession type=hidden value='$profession'>\n";
    print "<input name=startingrace type=hidden value='$startingrace'>\n";
    print "<input name=primechoices type=hidden value='$primechoices'>\n";
    print "<tr><td>possible starting cities</td><td>prime choice</td><td>selection</td></tr>\n";
    foreach $choice (@choices){
	next if ($choice eq "");
	$linkname = $choice;
	$linkname =~ s/ /_/g;
	print "<tr><td><a href=/tarmellion/$linkname.html target=blank>$choice</a></td><td>";
	if ($primechoices =~ /$choice/){
	    print "yes";
	} else {
	    print "no";
	}
	print "</td><td><input type=radio name=chosen value='$choice'></td></tr>\n";
    }
    print "<tr><td colspan=3 align=center><input type=submit value=submit></td></tr>\n";
    print "</form>\n</table>\n";

}

#completing everything
print "</body></html>\n";



