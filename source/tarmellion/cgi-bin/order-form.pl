#!/usr/bin/perl
#script to submit orders
use CGI qw(param remote_host);
use CGI::Carp qw(fatalsToBrowser);
print "Content-Type: text/html\n\n";

my $pin = "/home/tarmellion/code/tarmellion/players.in";
my $pfact="";
my $ppasswd="";
@fields = param();

foreach $field (@fields) {
  $$field = param($field);
  $$field =~ s/\r//g;
}

my $ctest = "/home/tarmellion/orders.log";
if(-f $ctest){
open (LOG, ">>$ctest");
print LOG "******************************************************************************\n";
print LOG "Faction   : $faction\n";
print LOG "Password  : $password\n";
print LOG "Turn      : $turn\n";
print LOG "Client IP : " . remote_host() . "\n";
}

if (!$turn) { 
	$file = "/home/tarmellion/code/tarmellion/web/orders.".$faction;
} else {
        $advpath = "/home/tarmellion/code/tarmellion/web-advance/".$turn;
        if (!opendir(ADV, "$advpath")){
	  `mkdir $advpath`;
	  `chmod 775 $advpath`;
	}
	$file = $advpath."/orders.".$faction;
}

open(PIN, "<$pin") || die "Couldn't open $pin: $!\n";
while (<PIN>) {
	chomp;
	if (/^Faction:\s(\d*)/){
		$pfact = $1;
	}

	if (/^Password:\s(.*)/){
		$ppasswd = $1;
	}

	$pass{$pfact} = $ppasswd;
}

if ($password eq $pass{$faction}) {
    open(FILE, ">$file") || die "Couldn't open $file: $!\n";
    print FILE $orders;
    close(FILE);
	`chmod 664 $file`;
	$ok = 1;
    $checkfile = $file.".checkfile";
    system("/home/tarmellion/code/tarmellion/tarmellion check $file $checkfile");
}

print "<HTML>\n";
print "<HEAD><TITLE>Tarmellion</TITLE></HEAD>";
if ($ok) {
	if (open(ORDERS, $file)) {
		print "<BODY background=/images/background.jpg><p>The orders were stored. ";
		$err_num = `grep "*** Error: " $checkfile | wc -l`;
		if ($err_num == 0){
			print "<p>There are no syntax errors in your orders.";
			print "<p>The orders checker was run, but as there are no errors its output won´t be displayed.";
		} else {
			print "<p>There are ".$err_num." syntax errors in your orders.";
			print "<p>Here is the output of the orders checker.<BR><BR>\n";
			open(CHECKER, "<$checkfile");
			while ($line = <CHECKER>) {
				$line =~ s/\n/<BR>\n/g;
				print $line;
			}
			close(CHECKER);
		}
		close(ORDERS);
		unlink($checkfile);
		print LOG "Orders stored. $err_num errors\n";
	} else {
		print "<BODY background=/images/background.jpg><p>There was an error while storing your orders! Try again or contact the game master!<BR>\n";
		print LOG "Orders NOT stored: $!\n";
	}
} else {
	print "<BODY background=/images/background.jpg><p><strong>Wrong password!</strong> <p>Orders were not stored.";
	print LOG "Orders NOT stored!\n";
}
print "</BODY></HTML>";
close(LOG);
