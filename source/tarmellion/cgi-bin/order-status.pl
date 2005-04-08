#!/usr/bin/perl -w
#script to show which orders have been submited already

use CGI qw(header end_html);
use CGI::Carp qw(fatalsToBrowser);

$dir = "/home/tarmellion/code/tarmellion/";
$pin = $dir."players.in";
$activelast = `grep "SendOrder: 0" $pin | wc -l`;
chomp($activelast);
$activethis = 0;
$limit = 0;
$turn = `grep "TurnNumber: " $pin`;
($dummy,$turnnumber) = split(/: /,$turn);
$timelimit = $turnnumber * 604800 + 1102586400;

opendir(EMAIL, $dir . "/email") || die "Couldn't read $dir/email: $!\n";
chdir($dir . "/email") || die "Couldn't chdir to $dir/email: $!\n";

while ($_ = readdir(EMAIL)) {
	next unless (/orders\.\d+/);
	$in{$_}->{"date"} = (stat($_))[9];
	$in{$_}->{"type"} = "EMail";
}
closedir(EMAIL);

opendir(WEB, $dir . "/web") || die "Couldn't read $dir/web: $!\n";
chdir($dir . "/web") || die "Couldn't chdir to $dir/web: $!\n";

while ($_ = readdir(WEB)) {
	next unless (/orders\.\d+/);
	$in{$_}->{"date"} = (stat($_))[9];
	$in{$_}->{"type"} = "Web";
}
closedir(WEB);

print header;
print qq~<HEAD>
	<TITLE>Tarmellion</TITLE>
</HEAD>
<BODY background=/images/background.jpg>
<CENTER><H2>Order status</H2>

<p>Here are listed all email and web orders for the current turn. Advance orders are not listed.
<table border=0>
    ~;

foreach (sort by_faction keys %in) {
    $activethis++;
    $ordersin{$_} = $in{$_}->{"date"};
    $dates{$in{$_}->{"date"}} = $_;
}


print "<tr><td>Active factions last turn:</td><td align=right>".$activelast."</td></tr>\n";
print "<tr><td>Active factions this turn:</td><td align=right>".$activethis."</td></tr>\n";

print qq~</table>

<p>

<table border=0 size=100%>
<tr><td valign=top size=50%>

<TABLE BORDER="1">
<TR>
	<TH>File</TH><TH>Type</TH><TH>Access date</TH>
</TR>
~;

foreach (sort by_faction keys %in) {
    if ($in{$_}->{"date"} > $timelimit){
	$font1 = "<font color=red>";
	$font2 = "</font>";
    }
print qq~<TR>
	<TD>$font1 $_ $font2</TD><TD>~;

print $font1.$in{$_}->{"type"}.$font2;

print qq~</TD><TD align=right>~;

$date = localtime($in{$_}->{"date"});
print $font1.$date.$font2;

print qq~</TD>
</TR>
~;
    $font1 = "";
    $font2 = "";

}

print qq~</TABLE>

</td><td valign=top size=50%>

<TABLE BORDER="1">
<TR>
	<TH>File</TH><TH>Type</TH><TH>Access date</TH>
</TR>
~;

foreach (sort %ordersin) {
    next if ($_ < 1);
    if ($_ > $timelimit){
	$font1 = "<font color=red>";
	$font2 = "</font>";
	if ($limit == 0){
	    $limit = 1;
	    print "<tr><td colspan=3 align=center><strong>$font1 ORDER SUBMISSION DEADLINE$font2</strong></td></tr>\n";
	}
    }

print qq~<TR>
	<TD>$font1 $dates{$_} $font2</TD><TD>~;

print $font1.$in{$dates{$_}}->{"type"}.$font2;

print qq~</TD><TD align=right>~;


$date = localtime($_);
#    $date = $_;
print $font1.$date.$font2;

print qq~</TD>
</TR>
~;
    $font1 = "";
    $font2 = "";

}

print "</td></tr></table>\n";


print end_html;

sub by_faction {
	$c = (split(/\./, $a))[1] if ($a =~ /^orders/);
	$d = (split(/\./, $b))[1] if ($b =~ /^orders/);

	return $c <=> $d;
}


















