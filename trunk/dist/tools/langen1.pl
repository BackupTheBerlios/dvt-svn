#!/usr/bin/perl

use strict;

my $outFileName = 'GB_A1-%CHAPTER%';
my $inFileName = $ARGV[0];

my $lastChapter = '1';

sub openNext() {
	my $fileName = $outFileName;
	my $s = sprintf("%02d", $lastChapter);
	$fileName =~ s/%CHAPTER%/$s/g;
	open(OUT, ">$fileName.dtl") || die("$!");

	print(OUT "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n".
		"<divotra:trainingLesson formatVersion=\"1.0\" langCode_o=\"de\" langCode_t=\"en\" version=\"0.1\" xmlns:divotra=\"http://www.delta-my.de/divotra\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:schemaLocation=\"http://www.delta-my.de/divotra/dtl.xsd\">\n".
		"<title><lg c=\"ia\">$fileName</lg></title>\n".
		"<description><lg c=\"en\">Importiert von www.vokabeln.de</lg></description>\n".
		"<entries>\n");
}

sub closeCurrent() {
	print(OUT "</entries>\n</divotra:trainingLesson>\n");
	close(OUT);
}

-e $inFileName || die("$!");

open(IN, "iconv -f ISO-8859-1 -t UTF-8 $inFileName|") || die("$!");
openNext();

my $i = 0;
while (<IN>) {
	$i++;
	if ($i >= 217) {
		chop; chop;
		
		my $line = $_;
		my $t = "";
		my $o = "";
		my $chapter = "";
		
		if (s/^\"(.{2,}?)\",\"(.{2,}?)\",(\d*)$/$1/) {
			$t = $_;
		}
		
		$_ = $line;
		if (s/^\"(.{2,}?)\",\"(.{2,}?)\",(\d*)$/$2/) {
			$o = $_;
		}
		
		$_ = $line;
		if (s/^\"(.{2,}?)\",\"(.{2,}?)\",(\d*)$/$3/) {
			$chapter = $_;
		}
		
#		if ($chapter % 4 == 0) {
#			closeCurrent();
#			$lastChapter = $chapter/4;
#			openNext();
#		}
		
		$t =~ s/°//g;
		$o =~ s/°//g;
		
		if ($t ne "" && $o ne "") {
			my $type = "";
			if ($t =~ /^to /) {
				$type = "v";
			} elsif ($o =~ /^([A-Z]|Ä|Ö|Ü)/) {
				$type = "n";
			}
			
			if ($type ne "") {
				print(OUT "<e t=\"$type\">\n");
			} else {
				print(OUT "<e>\n");
			}
			
			print(OUT "\t<o>$o</o>\n");
			print(OUT "\t<t>$t</t>\n</e>\n");
			
		}
		
		$t = <IN>;
		$t = <IN>;
	}
}

closeCurrent();
close(IN);
