#!/usr/bin/perl
# script to rate rewards based on player orders

#initializing variables
$dir = "/home/tarmellion/code/tarmellion/";
$dir .= "testturn/";
$pin = $dir."players.out";

#parsing the players.in file to get the current stats
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    #find the factions
    if (/^Faction:\s(\d*)/){
    $Faction = $1;
    next if ($Faction < 15);
    push(@factions, $Faction);
    }
}

if ($ARGV[0] ne ""){
	if ($ARGV[0] eq "friendly"){
		@factions = (220, 196, 57, 41, 40, 61, 54, 33, 45, 103, 77, 104, 213, 205, 69, 99, 142, 173);
	} else {
		@factions = ($ARGV[0]);
	}
}


print "Checking ";

foreach $faction (@factions){

    print "$faction ";

	$ofile = $dir."report.".$faction;
	unless (-f $ofile){
		print "quitted ";
		next;
	}
#check taxing in esgaroth

    $tax = `grep "Collects " $dir/report.$faction | grep "(27,57)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(29,57)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(25,59)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(29,59)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(24,60)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(26,60)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(28,60)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(23,61)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(25,61)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(27,61)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(29,61)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(24,62)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(26,62)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(27,63)" 2>/dev/null`;

    if ($tax ne ""){
	print "\nWARNING! Faction $faction is taxing in Esgaroth\n$tax\nChecking ";
    }

#check taxing in ramish empire

    $tax = `grep "Collects " $dir/report.$faction | grep "(102,14)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(102,16)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(103,17)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(103,19)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(104,14)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(104,16)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(104,18)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(104,20)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(105,15)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(105,17)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(105,19)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(106,14)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(106,14)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(106,14)" 2>/dev/null`;

    if ($tax ne ""){
	print "\nWARNING! Faction $faction is taxing in Ramish Empire\n$tax\nChecking ";
    }

#check taxing in surdell

    $tax = `grep "Collects " $dir/report.$faction | grep "(25,17)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(26,16)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(27,11)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(27,13)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(27,15)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(27,17)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(28,14)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(28,16)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(28,18)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(29,13)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(29,15)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(29,17)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(30,14)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(30,16)" 2>/dev/null`;

    if ($tax ne ""){
	print "\nWARNING! Faction $faction is taxing in Surdell\n$tax\nChecking ";
    }

#check taxing in elven woods

    $tax = `grep "Collects " $dir/report.$faction | grep "(85,61)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(85,63)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(86,60)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(86,62)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(86,64)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(87,57)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(87,59)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(87,61)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(87,63)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(88,58)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(88,60)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(88,62)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(88,64)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(89,61)" 2>/dev/null`;

    if ($tax ne ""){
	print "\nWARNING! Faction $faction is taxing in Elven Woods\n$tax\nChecking ";
    }

#check taxing in pelisham

#    $tax = `grep "Collects " $dir/report.$faction | grep "(30,40)" 2>/dev/null`;
    $tax = `grep "Collects " $dir/report.$faction | grep "(31,37)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(31,39)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(31,41)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(31,43)" 2>/dev/null`;
#    $tax .= `grep "Collects " $dir/report.$faction | grep "(32,38)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(32,40)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(33,37)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(33,39)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(33,41)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(34,38)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(34,40)" 2>/dev/null`;

    if ($tax ne ""){
	print "\nWARNING! Faction $faction is taxing in Pelisham\n$tax\nChecking ";
    }

#check taxing in shire

#    $tax = `grep "Collects " $dir/report.$faction | grep "(107,59)" 2>/dev/null`;
#    $tax .= `grep "Collects " $dir/report.$faction | grep "(107,61)" 2>/dev/null`;
    $tax = `grep "Collects " $dir/report.$faction | grep "(108,58)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(108,60)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(109,57)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(109,59)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(109,61)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(110,58)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(110,60)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(111,59)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(111,61)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(112,58)" 2>/dev/null`;

    if ($tax ne ""){
	print "\nWARNING! Faction $faction is taxing in Shire\n$tax\nChecking ";
    }

#check taxing in tortuga

    $tax = `grep "Collects " $dir/report.$faction | grep "(50,64)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(51,63)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(51,65)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(52,66)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(52,68)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(53,67)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(53,69)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(54,62)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(54,64)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(54,66)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(54,68)" 2>/dev/null`;

    if ($tax ne ""){
	print "\nWARNING! Faction $faction is taxing in Tortuga\n$tax\nChecking ";
    }

#check taxing in vyk

    $tax = `grep "Collects " $dir/report.$faction | grep "(61,11)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(62,10)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(62,12)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(62,14)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(63,11)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(63,13)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(63,15)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(64,14)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(64,16)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(65,15)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(65,17)" 2>/dev/null`;

    if ($tax ne ""){
	print "\nWARNING! Faction $faction is taxing in Vyk\n$tax\nChecking ";
    }

#check taxing in the vault

    $tax = `grep "Collects " $dir/report.$faction | grep "(12,24,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(12,26,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(13,23,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(13,25,underdeep)" 2>/dev/null`;
#    $tax .= `grep "Collects " $dir/report.$faction | grep "(13,27,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(14,24,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(14,26,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(15,23,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(15,25,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(15,27,underdeep)" 2>/dev/null`;

    if ($tax ne ""){
	print "\nWARNING! Faction $faction is taxing in The Vault\n$tax\nChecking ";
    }

#check taxing in seindelamere

    $tax = `grep "Collects " $dir/report.$faction | grep "(48,8,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(48,10,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(49,7,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(49,9,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(49,11,underdeep)" 2>/dev/null`;
#    $tax .= `grep "Collects " $dir/report.$faction | grep "(50,6,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(50,8,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(50,10,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(50,12,underdeep)" 2>/dev/null`;
#    $tax .= `grep "Collects " $dir/report.$faction | grep "(51,5,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(51,7,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(51,9,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(51,11,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(52,6,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(52,8,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(52,10,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(53,7,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(53,9,underdeep)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(53,11,underdeep)" 2>/dev/null`;
#    $tax .= `grep "Collects " $dir/report.$faction | grep "(54,8,underdeep)" 2>/dev/null`;
#    $tax .= `grep "Collects " $dir/report.$faction | grep "(54,10,underdeep)" 2>/dev/null`;

    if ($tax ne ""){
	print "\nWARNING! Faction $faction is taxing in Seindelamere\n$tax\nChecking ";
    }

#check taxing in dwarven kingdom

    $tax = `grep "Collects " $dir/report.$faction | grep "(4,8,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(4,10,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(5,7,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(5,9,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(5,11,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(6,8,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(6,10,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(6,12,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(6,14,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(7,9,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(7,11,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(7,13,underworld)" 2>/dev/null`;
#    $tax .= `grep "Collects " $dir/report.$faction | grep "(7,15,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(8,10,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(8,12,underworld)" 2>/dev/null`;
#    $tax .= `grep "Collects " $dir/report.$faction | grep "(8,14,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(9,9,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(9,11,underworld)" 2>/dev/null`;
#    $tax .= `grep "Collects " $dir/report.$faction | grep "(9,13,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(10,8,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(10,10,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(10,12,underworld)" 2>/dev/null`;
#    $tax .= `grep "Collects " $dir/report.$faction | grep "(11,9,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(11,11,underworld)" 2>/dev/null`;

    if ($tax ne ""){
	print "\nWARNING! Faction $faction is taxing in Dwarven Kingdom\n$tax\nChecking ";
    }

#check taxing in veldia

    $tax = `grep "Collects " $dir/report.$faction | grep "(40,28,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(40,30,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(41,27,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(41,29,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(41,31,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(42,26,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(42,28,underworld)" 2>/dev/null`;
#    $tax .= `grep "Collects " $dir/report.$faction | grep "(42,30,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(43,27,underworld)" 2>/dev/null`;
    $tax .= `grep "Collects " $dir/report.$faction | grep "(43,29,underworld)" 2>/dev/null`;

    if ($tax ne ""){
	print "\nWARNING! Faction $faction is taxing in Veldia\n$tax\nChecking ";
    }


}

print "\n";
