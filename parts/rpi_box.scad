//
// Box for Arduino and Raspberry Pi
//

$fn=32;
e=0.1;
mm=25.4;


ardu_x = 0.05*mm;
ardu_y = 0.35*mm;
ardu_dy = 2.25*mm;
ardu_so = 0.125*mm;

flange_w = 0.5*mm;
fh_off = 0.25*mm;
flange_hole_dia = 0.15*mm;
ardu_hole_dia = 0.125*mm;

//cavity_wid = 3.5*mm;
cavity_wid = 3.75*mm;
standoff_wid = 3.5*mm;
cavity_len = 5*mm;
cavity_hgt = 1*mm;
// cavity_hgt = 0.25*mm;

thk = 1.6;

// flange_w = 0.25*mm;
// fh_off = 0.125*mm;
// flange_hole_dia = 0.1*mm;
// ardu_hole_dia = 0.125*mm;
// 
// cavity_wid = 1*mm;
// cavity_len = 1*mm;
// cavity_hgt = 0.5*mm;
// thk = .063*mm;

module hole_at( x, y, d) {
  translate( [x, y, -e])
    cylinder( h=3*mm, d=d);
}

// arduino/shield mounting holes
module ardu_holes( x, y) {
  translate( [x, y, 0]) {
    hole_at( 0.55*mm,  0.1*mm, ardu_hole_dia);
    hole_at( 0.601*mm, 2*mm, ardu_hole_dia);
    hole_at( 2.601*mm, 1.4*mm, ardu_hole_dia);
    hole_at( 2.601*mm, 0.3*mm, ardu_hole_dia);
  }
}

module rpi_holes( x, y) {
  translate( [x, y, 0]) {
    hole_at( 3.5, 3.5, ardu_hole_dia);
    hole_at( 3.5+58, 3.5, ardu_hole_dia);
    hole_at( 3.5, 3.5+49, ardu_hole_dia);
    hole_at( 3.5+58, 3.5+49, ardu_hole_dia);
  }
}

// bottom with mounting holes
module bottom() {
  difference() {
    cube( [cavity_wid, cavity_len, thk]);
    ardu_holes( ardu_x, ardu_y);
    rpi_holes( ardu_x+22.5, ardu_y+ardu_dy);
    //    ardu_holes( ardu_x, ardu_y+2.25*mm);
  }
}

// clearance around cutouts
spc = 0.05*mm;

module ardu_cutouts() {
  // USB connector
  translate( [-6, 32+spc, pcb_thk-spc])
    cube( [16, 11.5+2*spc, 10.5+2*spc]);
  // power barrel jack
  color("black") {
    translate( [-6, 3.5+spc, pcb_thk-spc])
      cube( [10, 8.5+2*spc, 11+2*spc]);
  }
}

module rpi_cutouts() {
  // USB connectors
  translate( [-6, 22+spc, pcb_thk-spc])
    cube( [16, 12+2*spc, 15.5+2*spc]);
  translate( [-6, 40+spc, pcb_thk-spc])
    cube( [16, 12+2*spc, 15.5+2*spc]);
  // Ethernet
  translate( [-6, 1+spc, pcb_thk-spc])
    cube( [10, 16+2*spc, 13+2*spc]);
}

module ardu_cuts() {
  translate( [ardu_x, ardu_y, thk+ardu_so]) {
    ardu_cutouts();
    translate( [0, ardu_dy, 0])
      rpi_cutouts();
  }
}

module side_cuts() {
  translate( [73, cavity_len-2, 5])
    cube( [12, 10, 5]);
}

module back_cuts() {
  translate( [cavity_wid-3, cavity_len-40, 2])
    cube( [10, 17, 5]);
}

// box with cavity
module box() {
  translate( [-thk, -thk, 0]) {
    difference() {
      cube( [cavity_wid+2*thk, cavity_len+2*thk, cavity_hgt+thk]);
      translate( [thk, thk, -e])
	cube( [cavity_wid, cavity_len, cavity_hgt+2*e+thk]);
      ardu_cuts();
      side_cuts();
      back_cuts();
    }
  }
}

// flange
max_wid = cavity_wid+2*flange_w;
max_len = cavity_len+2*flange_w;

module flange() {
  translate( [-flange_w, -flange_w, cavity_hgt]) {
    difference() {
      cube( [max_wid, max_len, thk]);
      translate( [flange_w, flange_w, -e])
	cube( [cavity_wid, cavity_len, thk+2*e]);
      hole_at( fh_off, fh_off, flange_hole_dia);
      hole_at( max_wid-fh_off, fh_off, flange_hole_dia);
      hole_at( fh_off, max_len-fh_off, flange_hole_dia);
      hole_at( max_wid-fh_off, max_len-fh_off, flange_hole_dia);
    }
  }
}

so_dia = 0.25*mm;
so_hol = 0.106*mm;

module so_at(x, y) {
  translate( [x, y, thk-e]) {
    difference() {
      cylinder( d=so_dia, h=cavity_hgt);
      translate( [0, 0, thk])
	cylinder( d=so_hol, h=cavity_hgt);
    }
  }
    
}

module standoffs() {
  so_at( thk, thk);
  so_at( standoff_wid-thk, thk);
  so_at( standoff_wid-thk, cavity_len-thk);
  so_at( thk, cavity_len-thk);
}

pcb_thk = 1.6;

module arduino() {
  color("blue")
    linear_extrude( height=pcb_thk)
      scale( [mm, mm, mm]) import("arduino_outline.dxf");
  // USB connector
  translate( [-6, 32, pcb_thk])
    color("gray") cube( [16, 11.5, 10.5]);
  // power barrel jack
  color("black") {
    translate( [-2, 3.5, pcb_thk])
      cube( [3, 8.5, 11]);
    translate( [0, 7.75, 6])
    rotate( [0, 90, 0])
      cylinder( d=8, h=10);
  }
}

module rpi() {
  translate( [45, 28, 0.8])
  rotate( [90, 0, 180])
	     color("green")
      import("rpi_4b.stl");
}

module boards() {
  translate( [ardu_x, ardu_y, thk+ardu_so]) {
    arduino();
    translate( [0, ardu_dy, 0])
      rpi();
  }
}

module assembly() {
  bottom();
  box();
  standoffs();
}

// projection( cut=true) translate( [0, 0, -10])  
assembly();
//boards();
