#!/usr/bin/perl
# Simple form to set EPIC sign params
use strict;
use CGI qw/:standard/;
use CGI::Carp qw(fatalsToBrowser);
use IO::Socket::INET;

my $debug = 0;

sub printParams {
    
    my $query = shift;
    my @names = $query->param();
    
    my ($name, $value, @rows);
    
    foreach $name ( @names ) {
	push @rows, Tr( th($name), td( $query->param( "$name")));
    }

    print p,b('Query Parameters:');
    print table({-border=>1, -width=>"50%"}, @rows );
}    



sub send_to_sign {
    my $ip = shift @_;
    my $msg = shift @_;
    my $req;
    my $resp;
    my $socket;
    $socket = new IO::Socket::INET(
	PeerHost => "$ip",
	PeerPort => 80,
	Proto => 'tcp',
	);

    if( !$socket) {
	print p,"Not connected to $ip!", br;
    } else {

	$req = "GET /CMD=$msg HTTP/1.1\n\n";
	$socket->send($req);
	print "Send: $req", br if( $debug);
	$resp = $socket->recv($resp, 1024);
	print "Received: $resp", br if( $debug);
	shutdown($socket, 1);
    }
    #    sleep( 1);
    select( undef, undef, undef, 0.25);
}


my @daze = (
    { name => "Monday", open_f => "d0_open", close_f => "d0_close", defo => "9:00", defc => "5:00"},
    { name => "Tuesday", open_f => "d1_open", close_f => "d1_close", defo => "9:00", defc => "5:00"},
    { name => "Wednesday", open_f => "d2_open", close_f => "d2_close", defo => "9:00", defc => "5:00"},
    { name => "Thursday", open_f => "d3_open", close_f => "d3_close", defo => "9:00", defc => "5:00"},
    { name => "Friday", open_f => "d4_open", close_f => "d4_close", defo => "9:00", defc => "5:00"},
    { name => "Saturday", open_f => "d5_open", close_f => "d5_close", defo => "x", defc => "x"},
    { name => "Sunday", open_f => "d6_open", close_f => "d6_close", defo => "x", defc => "x"}
    );
    

{
    my $q = new CGI;
    
    print header,

    start_html('EPIC Sign'),
	h1('EPIC Electronic Sign');

    print "Enter data below.  Banner scrolls across top.",
	br, "IP address is number displayed when sign powers up", br
	br, "Open and close times must be in the format ", b("8:30"),
	br, "Enter \"<b>x</b>\" for blank time (not open)", br,

	p, start_form,
	b("IP Address:"), textfield('ipaddr'), p,
	b("Banner:"),textfield( -name=>'message', -value=>'Welcome to epic!'), p,
	"<table>",
	"<tr><th>Day <th>Open <th>close";

    foreach my $dayh ( @daze ) {
	print "<tr><td>", b($dayh->{name}), 
	    "<td>", textfield( -name=>$dayh->{open_f}, -value=>$dayh->{defo}),
	    "<td>", textfield( -name=>$dayh->{close_f}, -value=>$dayh->{defc})
    }
    print "</table>",
	p,
	submit, "Values above (be sure IP address is set!)",
	br, defaults, "Reset page to defaults",
	end_form;
}

if (param()) {
    my $ip = param('ipaddr');

    if( param('ipaddr')) {
	print p,"IP=$ip", br;
    } else {
	die "no IP specified", br;
    }

    if( param('message')) {
	send_to_sign( $ip, "M " . param('message'));
    }

    my $day = 0;
    foreach my $dayh ( @daze ) {

	if( param( $dayh->{open_f})) {
	    send_to_sign( $ip, "T " . $day . " " . param( $dayh->{open_f}) . " " .
			  param( $dayh->{close_f}));
	}
	$day++;
    }

    print "sign updated", br;
}
