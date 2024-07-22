#!/usr/bin/perl

use IO::Socket::INET;

my $socket = new IO::Socket::INET(
    PeerHost => '10.239.25.67',
    PeerPort => 80,
    Proto => 'tcp',
    );
die "cannot connect to server $!n" unless $socket;
print "connected\n";

my $msg = $ARGV[0];
my $req = "GET /CMD=$msg HTTP/1.1\n\n";
print "Sending: \"$req\"\n";
my $size = $socket->send($req);
print "sent $size bytes\n";

my $resp = $socket->recv($resp, 1024);
print "Received: $resp\n";

shutdown($socket, 1);
