/* Form definition file generated by fdesign on Fri Mar  1 16:12:43 2024 */

#include <stdlib.h>
#include "digit_7x9.h"



/***************************************
 ***************************************/

FD_digit *
create_form_digit( void )
{
    FL_OBJECT *obj;
    FD_digit *fdui = ( FD_digit * ) fl_malloc( sizeof *fdui );

    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->digit = fl_bgn_form( FL_NO_BOX, 89, 148 );

    obj = fl_add_box( FL_FLAT_BOX, 0, 0, 89, 148, "" );

    for( int row=0; row<NROW; row++) {
      for( int col=0; col<NCOL; col++) {
	fdui->leds[col][row] = obj = 
	  fl_add_roundbutton( FL_PUSH_BUTTON, col*PITCH, row*PITCH, PITCH, PITCH, "" );
	fl_set_object_color( obj, FL_RED, FL_BLACK );
      }
    }
    fl_end_form( );

    fdui->digit->fdui = fdui;

    return fdui;
}
