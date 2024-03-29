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

    fdui->led1 = obj = fl_add_roundbutton( FL_PUSH_BUTTON, 0, 0, 10, 10, "" );
    fl_set_object_color( obj, FL_RED, FL_BLACK );

    fdui->led2 = obj = fl_add_roundbutton( FL_PUSH_BUTTON, 10, 0, 10, 10, "" );
    fl_set_object_color( obj, FL_RED, FL_BLACK );

    fdui->led3 = obj = fl_add_roundbutton( FL_PUSH_BUTTON, 20, 0, 10, 10, "" );
    fl_set_object_color( obj, FL_RED, FL_BLACK );

    fdui->led4 = obj = fl_add_roundbutton( FL_PUSH_BUTTON, 30, 0, 10, 10, "" );
    fl_set_object_color( obj, FL_RED, FL_BLACK );

    fdui->led5 = obj = fl_add_roundbutton( FL_PUSH_BUTTON, 40, 0, 10, 10, "" );
    fl_set_object_color( obj, FL_RED, FL_BLACK );

    fdui->led6 = obj = fl_add_roundbutton( FL_PUSH_BUTTON, 50, 0, 10, 10, "" );
    fl_set_object_color( obj, FL_RED, FL_BLACK );

    fdui->led7 = obj = fl_add_roundbutton( FL_PUSH_BUTTON, 60, 0, 10, 10, "" );
    fl_set_object_color( obj, FL_RED, FL_BLACK );

    fl_end_form( );

    fdui->digit->fdui = fdui;

    return fdui;
}
