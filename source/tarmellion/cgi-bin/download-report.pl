#!/usr/bin/perl
#script to identify user before download of report
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

@fields = param();
foreach $field (@fields) {
  $$field = param($field);
  $$field =~ s/\r//g;
}

my $BasePath = "/home/tarmellion/code/tarmellion";
my $LogFile = "/home/tarmellion/download.log";
my $PlayerFile = $BasePath."/players.in";
my $ReportFile2 = "report.".$faction;
my $ReportFile = "report.".$faction.".".$password;


#=============================================================
# Logging
#=============================================================
if ($LogFile) {
  if (!open (LOG,">>$LogFile")) {
    warn "Can't open $LogFile: $!\n";
    $LogFile = 0;
  }
  print LOG "******************************************************************************\n";
  print LOG "Faction   : $faction\n";
  print LOG "Password  : $password\n";
  print LOG "Client IP : " . remote_host() . "\n";
}

#===========================================================
# Displaying download site
#===========================================================

print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE></HEAD>\n";
print "<BODY background=/images/background.jpg>";
print "<h2>Download of report for faction $faction</h2>\n";

if (!open (PIN,"$PlayerFile")) {
    warn "Can't open $PlayerFile: $!\n";
    $PlayerFile = 0;
    print "<p>Sorry, I have trouble getting the game data. Please try again later.";
} else {
    $pinf = $pinp = $pine = "";
    while (<PIN>) {
	chomp;
	if (/^TurnNumber: (\d+)/) {
	    $turn = $1;
	}
	if (/^Faction: (\d+)/) {
	    $pinf = $1 ;
	    if ($pinf eq $faction) {
		<PIN>;
		$_ = <PIN>;
		chomp;
		/^Email: (.+)/;
		$pine = $1;
		$_ = <PIN>;
		chomp;
		/^Password: (.+)/;
		$pinp = $1 ;
		last;
	    }
	}
    }
    close PIN;
    if ($pinp eq $password) {
	if (open (REP,"</home/tarmellion/html/report/$ReportFile")){
	    print "<p><a href=\"http://www.herobane.de/tarmellion/report/$ReportFile\">download ".$ReportFile2." for turn ".$turn."</a>\n";
	    close(REP);
	} else {
	    print "<p>Cannot open $ReportFile2";
	    print ".<br>Please try again later";
	}
    } else {
	print "<p>wrong password given\n";
	print LOG "download attempt failed.";
    }
}

print "</body></html>";
close(LOG);
