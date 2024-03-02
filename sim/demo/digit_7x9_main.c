#include "digit_7x9.h"

int
main( int    argc,
      char * argv[ ] )
{
    FD_digit *fd_digit;

    fl_initialize( &argc, argv, 0, 0, 0 );
    fd_digit = create_form_digit( );

    /* Fill-in form initialization code */

    /* Show the first form */

    fl_show_form( fd_digit->digit, FL_PLACE_CENTERFREE, FL_FULLBORDER, "digit" );

    for( int row=0; row<NROW; row++)
      for( int col=0; col<NCOL; col++)
	fl_set_button( fd_digit->leds[col][row], 1);

    fl_set_button( fd_digit->leds[0][0], 0);


    fl_do_forms( );

    if ( fl_form_is_visible( fd_digit->digit ) )
        fl_hide_form( fd_digit->digit );
    fl_free( fd_digit );
    fl_finish( );

    return 0;
}
