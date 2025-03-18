#!/usr/bin/perl
use strict;
use warnings;

# Leggi ogni riga da STDIN (pipe)
while (my $line = <STDIN>) {
    # Puoi fare qualsiasi elaborazione su ogni riga ricevuta
    print "Ricevuto: $line";
}

