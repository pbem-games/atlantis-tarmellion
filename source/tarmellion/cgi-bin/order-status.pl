#!/usr/bin/perl -w
#script to show which orders have been submited already

use CGI qw(header end_html);
use CGI::Carp qw(fatalsToBrowser);

$dir = "/home/tarmellion/code/tarmellion/";
$pin = $dir."players.in";
$activelast = `grep "SendOrder: 0" $pin | wc -l`;
chomp($activelast);
$activethis = 0;

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
}

print "<tr><td>Active factions last turn:</td><td align=right>".$activelast."</td></tr>\n";
print "<tr><td>Active factions this turn:</td><td align=right>".$activethis."</td></tr>\n";

print qq~</table>

<p>

<TABLE BORDER="1">
<TR>
	<TH>File</TH><TH>Type</TH><TH>Access date</TH>
</TR>
~;

foreach (sort by_faction keys %in) {
print qq~<TR>
	<TD>$_</TD><TD>~;

print $in{$_}->{"type"};

print qq~</TD><TD align=right>~;

$date = localtime($in{$_}->{"date"});
print $date;

print qq~</TD>
</TR>
~;
}

print qq~</TABLE>
~;

print end_html;

sub by_faction {
	$c = (split(/\./, $a))[1] if ($a =~ /^orders/);
	$d = (split(/\./, $b))[1] if ($b =~ /^orders/);

	return $c <=> $d;
}


















