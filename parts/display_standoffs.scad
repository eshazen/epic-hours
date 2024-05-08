//
// support bracket for 8x8 displays
//

// ToDo:
//   Stiffener ridge down middle
//   Mount tabs at ends
//

$fn = 64;
e = 0.1;
mm = 25.4;

include <post.scad>;

bracket_wid = 32;		/* width of one panel */
bracket_len = 9;		/* bracket width (in Y) */
bracket_hgt = 1.6;		/* thickness of base */

total_hgt = 0.5*mm;		/* overall bracket height (base to pcb) */

// mounting hole diameter
hole_dia = 0.15*mm;

// hole in center of post (for #4 sheet metal screw)
// post_hole = 0.150*mm;
post_hole_dia = 0.11*mm;

// post locations
post_dx = 3;
posts1 = [ [post_dx, 5.9], [32-post_dx, 32-5.9] ];
posts2 = [ [32-post_dx, 5.9], [post_dx, 32-5.9] ];

post_spc_x = 32-2*post_dx;
post_spc_y = 32-2*5.9;

// stiffener
stiff_thk = 5;
stiff_hgt = 2;
stiff_gap = 5;

// end mounting tabs
tab_wid = 0.4*mm;

// mounting hole
module hole_at( x, y, d) {
     translate( [x, y, -e])
	  cylinder( d=d, h=bracket_hgt+2*e);
}

module post_hole( x, y) {
     translate( [x, y, -10])
	  cylinder( h=30, d=post_hole_dia);

}

module tab_at( x, y) {
  translate( [x, y, 0]) {
    difference() {
	 cube( [tab_wid+e, tab_wid, bracket_hgt]);
	 hole_at( tab_wid/2, tab_wid/2, hole_dia);
       }
  }
}


module bracket() {
     difference() {
	  union() {
	       cube( [bracket_wid+e, bracket_wid+e, bracket_hgt]);
	       for( pt = posts1) {
		 post_at( pt.x, pt.y, 1);
	       }
	       for( pt = posts2) {
		 post_at( pt.x, pt.y, 0);
	       }
	  }
	  hole_at( bracket_wid/2, 0.2*mm, hole_dia);
	  hole_at( bracket_wid/2, bracket_wid-0.2*mm, hole_dia);
	  for( pt = posts2) {
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
	  p1 = posts2[0];
	  p2 = posts1[1];
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

     // stiffener
     translate( [stiff_gap, bracket_wid/2-stiff_thk/2, bracket_hgt-e])
       cube( [ndigit*bracket_wid-2*stiff_gap, stiff_thk, stiff_hgt]);

     // end mounting tabs
     tab_at( -tab_wid, bracket_wid/2-tab_wid/2);
     tab_at( ndigit*bracket_wid-e, bracket_wid/2-tab_wid/2);
       
}

// single peg for testing
module test_peg() {
     difference() {
       post_at( 5, 5, 0);
	  post_hole( 5,5);
     }
     post_at( 5+post_spc_x, 5, 1);
     difference() {
       cube( [bracket_wid+5, 10, bracket_hgt]);
       post_hole( 5, 5);
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

// projection()
// rotate( [90, 0, 0])
// rotate( [0, 0, 45])
multi_bracket(8);
// multi_assembly( 3, 155);



// test_peg();
