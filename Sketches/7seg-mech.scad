
$fn=64;
e = 0.1;

seg_len = 15.6;
seg_dia = 4.6;

module polly() {
     seg_bod = seg_len-seg_dia;
     polygon( points = [ [0,0], [seg_dia/2, seg_dia/2], [seg_dia/2, seg_dia/2+seg_bod],
			 [0, seg_len], [0,0] ]);
			 
}

n = 5;
dn = 360/(n*2);

module segment_at( x, y, rot_t) {
     rt = -dn+2*(rot_t/2)*(dn*2);     
     translate( [x-seg_len/2, y, 0]) {
	  rotate( [0, 90, 0]) {
	       rotate( [0, 0, rt]) {
		    color("#202020") rotate_extrude( $fn=n) polly();
		    translate( [0, 0, seg_len/2])
			 rotate( [0, 90, 0])
			 cylinder( d=1, h=5);
		    color("yellow")
			 rotate( [0, 90, dn])
			 translate( [-seg_len+seg_dia/2, -1.4, -1.88])
			 cube( [seg_len-seg_dia, seg_dia*.6, 0.01]);
	       }
	  }
     }
}

// lookup table for digits 0-9
codez = [ 192, 249, 164, 176, 153, 146, 130, 248, 128, 144 ];
// a=1, b=2, c=4, d=8, e=16, f=32, g=64

function bmask(n,m) = floor(n / m) % 2;

module digit( d) {
     seg_spc = 1.6;
     seg_pit = seg_len + seg_spc;

     cod = codez[d];

     segment_at( 0, 0, bmask(cod,8));		/* segment d */
     segment_at( 0, seg_pit, bmask(cod,64));	/* segment g */
     segment_at( 0, 2*seg_pit, bmask(cod,1));	/* segment a */
     rotate( [0, 0, 90]) {
	  segment_at( seg_pit/2, seg_pit/2, bmask(cod,16)); /* segment e */
	  segment_at( seg_pit/2, -seg_pit/2, bmask(cod,4)); /* segment c */
	  segment_at( 3*seg_pit/2, seg_pit/2, bmask(cod,32)); /* segment f */
	  segment_at( 3*seg_pit/2, -seg_pit/2, bmask(cod,2)); /* segment b */
     }
}

dspc = 28;

translate( [-25, -5, -10]) color("black") cube( [dspc*11, 50, 0.1]);

for( i=[0:3]) {
     translate( [i*dspc, 0, 0])
	  digit(i);
}
