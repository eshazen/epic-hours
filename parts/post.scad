tip_dia1 = 3.1;			/* mounting hole ID */
tip_dia2 = 2.9;
tip_hgt = 3.5;			/* length of tip through hole */

peg_dia = 0.22*mm;		/* diameter of peg PCB rests on */
peg_hgt = 0.25*mm;

// support post
module post_at( x, y) {
     translate( [x, y, 0]) {
	  cylinder( h=total_hgt, d=peg_dia);
	  translate( [0, 0, total_hgt-e])
	       cylinder( h=tip_hgt, d1=tip_dia1, d2=tip_dia2);
     }
}


