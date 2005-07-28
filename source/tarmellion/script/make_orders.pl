#!/usr/bin/perl
#script to create default orders for the next turn

$dir = "/home/tarmellion/code/tarmellion/";
$dir2 = $dir."script/";

# gewuenschtes File
$tit = "report";

opendir(DIR, "$dir") || die "$dir nicht gefunden. \n";
@filelist = grep(/$tit\./, readdir(DIR));
closedir(DIR);

# ---------------------------------------------
# Dateien einlesen und abspeichern
# ---------------------------------------------

foreach $file (reverse sort @filelist) {
    next if ($file =~ /zip/);
    $altdat="";
    open(FILE, "<$file") || skip;
    $keep=0;
    while(<FILE>) {
        $line = $_;
        if (/\#atlantis/) { $keep=1 };
        if ($keep) { $altdat .= $line; }
    }
    close(FILE);
    ($filename, $ext) = split(/\./,$file);
    $filenew = $dir."orders.".$ext;
    open(NEWFILE, ">$filenew") || die "$filenew kann nicht angelegt werden. \n";
    print NEWFILE $altdat;
    close(NEWFILE);
}


