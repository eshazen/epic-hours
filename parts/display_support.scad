//
// support bracket for 8x8 displays
//

$fn = 64;
e = 0.1;
mm = 25.4;

include <post.scad>;

ndigit = 3;

bracket_wid = 32;		/* width of one panel */
bracket_len = 9;		/* bracket width (in Y) */
bracket_hgt = 0.125*mm;

total_hgt = 0.5*mm;		/* overall bracket height (back to pcb) */

hole_dia = 0.15*mm;

// mounting hole
module hole_at( x, y) {
     translate( [x, y, -e])
	  cylinder( d=hole_dia, h=bracket_hgt+2*e);
}

module bracket() {
     difference() {
	  cube( [bracket_wid+e, bracket_wid+e, bracket_hgt]);
	  hole_at( bracket_wid/2, 0.2*mm);
	  hole_at( bracket_wid/2, bracket_wid-0.2*mm);
     }
     post_at( 3, 5.9);
     post_at( 32-3, 5.9);
     post_at( 3, 32-5.9);
     post_at( 32-3, 32-5.9);
}

// set of <ndigit> brackets
module multi_bracket() {
     for( i=[0:ndigit-1]) {
	  translate( [i*bracket_wid, 0, 0])
	       bracket();
     }
}

// single peg for testing
module test_peg() {
     post_at( 5, 5);
}

multi_bracket();


// test_peg();
