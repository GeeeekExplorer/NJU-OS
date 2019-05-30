#!/usr/bin/perl

open(SIG, $ARGV[0]) || die "open $ARGV[0]: $!";

$n = sysread(SIG, $buf, 100000);

print STDERR "OK: Kernel is $n bytes - Extended to 200 sectors\n";

$buf .= "\0" x (102400-$n);

open(SIG, ">$ARGV[0]") || die "open >$ARGV[0]: $!";
print SIG $buf;
close SIG;
