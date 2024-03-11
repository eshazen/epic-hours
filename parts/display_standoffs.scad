//
// support bracket for 8x8 displays
//

// TODO:  fix standoff bridge plugging holes

$fn = 64;
e = 0.1;
mm = 25.4;

include <post.scad>;

bracket_wid = 32;		/* width of one panel */
bracket_len = 9;		/* bracket width (in Y) */
bracket_hgt = 1.6;

total_hgt = 0.5*mm;		/* overall bracket height (back to pcb) */

hole_dia = 0.15*mm;

post_hole = 0.100*mm;

post_dx = 3;
posts = [ [post_dx, 5.9], [32-post_dx, 5.9], [post_dx, 32-5.9], [32-post_dx, 32-5.9] ];


// mounting hole
module hole_at( x, y, d) {
     translate( [x, y, -e])
	  cylinder( d=d, h=bracket_hgt+2*e);
}

module post_hole( x, y) {
     translate( [x, y, 0.2*mm])
	  cylinder( h=20, d=post_hole);

}

module bracket() {
     difference() {
	  union() {
	       cube( [bracket_wid+e, bracket_wid+e, bracket_hgt]);
	       for( pt = posts) {
		    post_at( pt.x, pt.y);
	       }
	  }
	  hole_at( bracket_wid/2, 0.2*mm, hole_dia);
	  hole_at( bracket_wid/2, bracket_wid-0.2*mm, hole_dia);
	  for( pt = posts) {
	       post_hole( pt.x, pt.y);
	  }
     }
}

// set of <ndigit> brackets
module multi_bracket(ndigit) {
     for( i=[0:ndigit-1]) {
	  translate( [i*bracket_wid, 0, 0])
	       bracket();
     }
     // bridges between standoffs
     if( ndigit > 1) {
	  p1 = posts[1];
	  p2 = posts[3];
	  for( i=[0:ndigit-2]) {
	       // offset to bracket
	       translate( [i*bracket_wid, 0, 0]) {
		    // draw the bridge, then re-drill the holes
		    difference() {
			 translate( [p1.x, p1.y-peg_dia/2, 0])
			      cube( [peg_dia, peg_dia, total_hgt]);
			 post_hole( p1.x, p1.y);
			 post_hole( p1.x+2*post_dx, p1.y);
		    }
		    difference() {
			 translate( [p2.x, p2.y-peg_dia/2, 0])
			      cube( [peg_dia, peg_dia, total_hgt]);
			 post_hole( p2.x, p2.y);
			 post_hole( p2.x+2*post_dx, p2.y);
		    }
	       }
	  }
     }
}

// single peg for testing
module test_peg() {
     difference() {
	  post_at( 5, 5);
	  post_hole( 5,5);
     }
}

// two multi_bracket() with spacing
module multi_assembly( nb, dx) {
     multi_bracket(nb);
     translate( [dx, 0, 0])
	  multi_bracket(nb);
     translate( [bracket_wid*nb, 0, 0])
	  cube( [dx-nb*bracket_wid, bracket_wid, bracket_hgt]);
}

translate( [0, 10, 0])
 multi_bracket(3);
// multi_assembly( 3, 155);



// test_peg();
