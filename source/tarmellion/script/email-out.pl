#!/usr/bin/perl

use SendMail;

$turn = $ARGV[0];

$dir = "/home/tarmellion/code/tarmellion/";
$dir2 = $dir."script/";
$pin = $dir."players.in";
$sender = "Tarmellion <tarmellion\@her0bane.de>";
$replyto = "Tarmellion <tarmellion-orders\@herobane.de>"; 
$host = "localhost";

print "Sending reports for ";
open(PIN, "<$pin");
while (<PIN>) {
    chomp;
    $body = "";
    if (/^Faction:\s(\d*)/){
	$pfact = $1;
    }
    if (/^ZippedReport:\s(.*)/){
	$zipped = $1;
	if ($zipped eq "yes"){
	    $file = $dir."report.".$pfact.".zip";
	} elsif ($zipped eq "none"){
	    next;
	} else {
	    $file = "";
	    $report = $dir."report.".$pfact;
	    open(REP, "< $report");
	    while(<REP>){
		$body .= $_;
	    }
	    close(REP);
	}
    }
    if (/^Email:\s(.*)/){
	$pemail = $1;
	next if ($pemail eq "NoAddress");
	$subject = "report.$pfact for turn $turn";
	$recipient = $pemail;
	$obj = new SendMail($host);
	$obj->From($sender);
	$obj->Subject($subject);
	$obj->To($recipient);
	$obj->Attach($file);
	$obj->setMailBody($body);
	if ($obj->sendMail() != 0) {
	    print $obj->{'error'}."\n";
	}
	$obj->reset();		
	print "$pfact ";
    }
}
print "\n";


