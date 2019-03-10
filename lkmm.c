
//////////////////////////////////////////
//////////////////////////////////////////
// lkmm  
//////////////////////////////////////////
//////////////////////////////////////////
#include <stdio.h>
#define PATH_MAX 2500
#if defined(__linux__) //linux
#define MYOS 1
#elif defined(_WIN32)
#define MYOS 2
#elif defined(_WIN64)
#define MYOS 3
#elif defined(__unix__) 
#define MYOS 4  // freebsd
#define PATH_MAX 2500
#else
#define MYOS 0
#endif
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <time.h>

#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>  
#include <time.h>




#define ESC "\033"
#define home() 			printf(ESC "[H") //Move cursor to the indicated row, column (origin at 1,1)
#define clrscr()		printf(ESC "[2J") //clear the screen, move to (1,1)
#define gotoxy(x,y)		printf(ESC "[%d;%dH", y, x);
#define ansigotoyx(y,x)		printf(ESC "[%d;%dH", y, x);



int rows, cols ; 
int pansel = 1;

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

char pathclipboard[PATH_MAX];



///////////////////////////////////////////
void readfile( char *filesource )
{
   FILE *source; 
   int ch ; 
   source = fopen( filesource , "r");
   if ( source == NULL ) { printf( "File not found.\n" ); } else {
   while( ( ch = fgetc(source) ) != EOF )
   {
         printf( "%c", ch );
   }
   fclose(source);
   }
}
   










void nsystem( char *mycmd )
{
   printf( "<SYSTEM>\n" );
   printf( " >> CMD:%s\n", mycmd );
   system( mycmd );
   printf( "</SYSTEM>\n");
}


void nrunwith( char *cmdapp, char *filesource )
{
           char cmdi[PATH_MAX];
           strncpy( cmdi , "  " , PATH_MAX );
           strncat( cmdi , cmdapp , PATH_MAX - strlen( cmdi ) -1 );
           strncat( cmdi , " " , PATH_MAX - strlen( cmdi ) -1 );
           strncat( cmdi , " \"" , PATH_MAX - strlen( cmdi ) -1 );
           strncat( cmdi ,  filesource , PATH_MAX - strlen( cmdi ) -1 );
           strncat( cmdi , "\" " , PATH_MAX - strlen( cmdi ) -1 );
           nsystem( cmdi ); 
}



void clear_screen()
{
    int fooi;
    struct winsize w; // need ioctl and unistd 
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
    clrscr();
    home();
}


int fexist(char *a_option)
{
  char dir1[PATH_MAX]; 
  char *dir2;
  DIR *dip;
  strncpy( dir1 , "",  PATH_MAX  );
  strncpy( dir1 , a_option,  PATH_MAX  );

  struct stat st_buf; 
  int status; 
  int fileordir = 0 ; 

  status = stat ( dir1 , &st_buf);
  if (status != 0) {
    fileordir = 0;
  }

  // this is compatible to check if a file exists
  FILE *fp2check = fopen( dir1  ,"r");
  if( fp2check ) {
  // exists
  fileordir = 1; 
  fclose(fp2check);
  } 

  if (S_ISDIR (st_buf.st_mode)) {
    fileordir = 2; 
  }
return fileordir;
/////////////////////////////
}







static struct termios oldt;

void restore_terminal_settings(void)
{
    tcsetattr(0, TCSANOW, &oldt);  /* Apply saved settings */
}

void enable_waiting_for_enter(void)
{
    tcsetattr(0, TCSANOW, &oldt);  /* Apply saved settings */
}

void disable_waiting_for_enter(void)
{
    struct termios newt;

    /* Make terminal read 1 char at a time */
    tcgetattr(0, &oldt);  /* Save terminal settings */
    newt = oldt;  /* Init new settings */
    newt.c_lflag &= ~(ICANON | ECHO);  /* Change settings */
    tcsetattr(0, TCSANOW, &newt);  /* Apply settings */
    atexit(restore_terminal_settings); /* Make sure settings will be restored when program ends  */
}





/////////////////////////
/////////////////////////
int  nexp_user_sel[5] ; 
int  nexp_user_scrolly[5] ;
char nexp_user_fileselection[PATH_MAX]; 
int  tc_det_dir_type = 0;
/////////////////////////
void printdir( int pyy, int fopxx, char *mydir , int panviewpr )
{
   int pxx = fopxx;
   if ( pxx == 0 )       pxx = 2;
   DIR *dirp; int posy = 0;  int posx, chr ; 
   int fooselection = 0;
   posy = 1; posx = cols/2;
   char cwd[PATH_MAX];
   struct dirent *dp;
   dirp = opendir( mydir  );
   int entrycounter = 0;
   fooselection = 0;
   while  ((dp = readdir( dirp )) != NULL ) 
   if ( posy <= rows-3 )
   {
        //printf("%s\n", KYEL);
        entrycounter++;
        if ( entrycounter <= nexp_user_scrolly[panviewpr] )
              continue;

        if (  dp->d_name[0] !=  '.' ) 
        if (  strcmp( dp->d_name, "." ) != 0 )
        if (  strcmp( dp->d_name, ".." ) != 0 )
        {
            posy++;  fooselection++;
            if ( dp->d_type == DT_DIR ) 
            {
                 //color_set( 3 , NULL );
                 //mvaddch( posy, posx++ , '/' );
                 //printf( "/" );
                 ansigotoyx( posy, pxx );
                 printf( "/" );
                 posx++;
            }
            else if ( dp->d_type == 0 )
            {
               if ( tc_det_dir_type == 1 )
               if ( fexist( dp->d_name ) == 2 )
               {
                 //color_set( 3 , NULL );
                 //mvaddch( posy, posx++ , '/' );
                 //printf( "/" );
                 ansigotoyx( posy, pxx );
                 printf( "/" );
                 posx++;
               }
            }

            ansigotoyx( posy, pxx-1 );
            if ( nexp_user_sel[ panviewpr ] == fooselection ) 
            {
                  if ( panviewpr == pansel )
                  {
                    strncpy( nexp_user_fileselection, dp->d_name , PATH_MAX );
                    //color_set( 4, NULL );
                    printf( ">" );
                  }
            }
            else 
                  printf( " " );

            ansigotoyx( posy, pxx );
            for ( chr = 0 ;  chr <= strlen(dp->d_name) ; chr++) 
            {
              if  ( dp->d_name[chr] == '\n' )
              {    //posx = cols/2;
              }
              else if  ( dp->d_name[chr] == '\0' )
              {    //posx = cols/2;
              }
              else
              {  
                 //mvaddch( posy, posx++ , dp->d_name[chr] );
                 printf( "%c", dp->d_name[chr] );
                 posx++;
              }
            }
        }
   }
   closedir( dirp );
   // color_set( 0, NULL ); attroff( A_REVERSE );
   //mvprintw( rows-1, cols/2, "[FILE: %s]", nexp_user_fileselection );
}





char userstr[PATH_MAX];
/////////////////////////////////////
void strninput( char *mytitle, char *foostr )
{
      strncpy( userstr , "" , PATH_MAX );
      disable_waiting_for_enter();
      char strmsg[PATH_MAX];
      char charo[PATH_MAX];
      strncpy( strmsg, foostr , PATH_MAX );
      int foousergam = 0; int ch ;  int chr;

      strncpy( strmsg, ""  ,  PATH_MAX );
      int j; 
      char ptr[PATH_MAX];
      char str[PATH_MAX];

      while( foousergam == 0 ) 
      {

         home();
         for ( chr = 0 ;  chr <= cols-1 ; chr++) printf( " ");

         home();
         if ( strcmp( mytitle, "" ) != 0 ) 
            printf( "|%s|", mytitle );

         ansigotoyx( rows, 0 );
         for ( chr = 0 ;  chr <= cols-1 ; chr++) printf( " ");
         ansigotoyx( rows, 0 );
         printf( ": %s", strmsg );

         ch = getchar();
         if ( ch == 10 )       foousergam = 1;

	 else if ( ch == 27 ) 
	      strncpy( strmsg, ""  ,  PATH_MAX );

	 else if ( ( ch == 8 )  || ( ch == 127 ) )  
         {
            if ( strlen( strmsg ) >= 2 ) 
            {
              j = 0; strncpy(  ptr , "" ,  PATH_MAX );
              for ( chr = 0 ;  chr <= strlen( strmsg )-2 ; chr++) 
              {
                 ptr[j++] = strmsg[chr];
              }
	      strncpy( strmsg, ptr  ,  PATH_MAX );
            }
            else
	      strncpy( strmsg, ""  ,  PATH_MAX );
         }

	 else if (
			(( ch >= 'a' ) && ( ch <= 'z' ) ) 
		        || (( ch >= 'A' ) && ( ch <= 'Z' ) ) 
		        || (( ch >= '1' ) && ( ch <= '9' ) ) 
		        || (( ch == '0' ) ) 
		        || (( ch == '~' ) ) 
		        || (( ch == '!' ) ) 
		        || (( ch == '&' ) ) 
		        || (( ch == '=' ) ) 
		        || (( ch == ':' ) ) 
		        || (( ch == ';' ) ) 
		        || (( ch == '<' ) ) 
		        || (( ch == '>' ) ) 
		        || (( ch == ' ' ) ) 
		        || (( ch == '|' ) ) 
		        || (( ch == '#' ) ) 
		        || (( ch == '?' ) ) 
		        || (( ch == '+' ) ) 
		        || (( ch == '/' ) ) 
		        || (( ch == '\\' ) ) 
		        || (( ch == '.' ) ) 
		        || (( ch == '$' ) ) 
		        || (( ch == '%' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == ',' ) ) 
		        || (( ch == '{' ) ) 
		        || (( ch == '}' ) ) 
		        || (( ch == '(' ) ) 
		        || (( ch == ')' ) ) 
		        || (( ch == ']' ) ) 
		        || (( ch == '[' ) ) 
		        || (( ch == '*' ) ) 
		        || (( ch == '"' ) ) 
		        || (( ch == '@' ) ) 
		        || (( ch == '-' ) ) 
		        || (( ch == '_' ) ) 
		        || (( ch == '^' ) ) 
		        || (( ch == '\'' ) ) 
	             ) 
		  {
                        snprintf( charo, PATH_MAX , "%s%c",  strmsg, ch );
		        strncpy( strmsg,  charo ,  PATH_MAX );
		  }
     }
     strncpy( userstr, strmsg , PATH_MAX );
}



///////////////////////////////////////////
void printhline( )
{
   int ch; 
   for ( ch = 0 ;  ch <= cols-1 ; ch++) printf( "%c", '-');
}



///////////////////////////////////////////
void readfilesp( char *filesource, int linestart , int lineend )
{
  FILE *source; 
  int ch ;  int linecount = 1;
  source = fopen( filesource , "r");
  if ( source == NULL ) { printf( "File not found.\n" ); } else  
  {
   clear_screen();
   for ( ch = 0 ;  ch <= cols-1 ; ch++) printf( "%c", '-');
   printf( "FILE: %s\n", filesource );
   for ( ch = 0 ;  ch <= cols-1 ; ch++) printf( "%c", '-');
   printf( "%d: ", linecount );
   while( ( ch = fgetc(source) ) != EOF )
   {
       if ( linecount <= lineend ) 
       {
         if ( ch == '\n' ) 
         {
            linecount++; 
            printf( "\n%d: ", linecount );
         }
         else
            printf( "%c", ch );
       }
   }
   fclose(source);
   }

   printf( "\33[2K" ); 
   printf( "\r" );
   for ( ch = 0 ;  ch <= cols-1 ; ch++) printf( "%c", '-');
}
   




int main( int argc, char *argv[])
{
     ////////////////////////////////////////////////////////
     // file
     ////////////////////////////////////////////////////////
     if ( argc == 3)
     if ( strcmp( argv[1] , "-f" ) ==  0 ) 
     {
       printf("%syellow\n", KYEL);
       readfile( argv[ 2 ] );
       return 0;
     }

    ////////////////////////////////////////////////////////
    char cwd[PATH_MAX];
    char pathpan[5][PATH_MAX];
    ////////////////////////////////////////////////////////
    if ( argc == 2)
    if ( strcmp( argv[1] , "" ) !=  0 )
    if ( fexist( argv[1] ) ==  2 )
    {
          chdir( argv[ 1 ] );
          strncpy( pathpan[ 1 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
          strncpy( pathpan[ 2 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
    }



    ////////////////////////////////////////////////////////
    int viewpan[5];
    nexp_user_sel[1] = 1;
    nexp_user_sel[2] = 1;
    nexp_user_scrolly[1] = 0;
    nexp_user_scrolly[2] = 0;
    strncpy( pathpan[ 1 ] ,  getcwd( cwd, PATH_MAX ), PATH_MAX );
    strncpy( pathpan[ 2 ] ,  getcwd( cwd, PATH_MAX ), PATH_MAX );
    strncpy( pathclipboard , getcwd( cwd, PATH_MAX ), PATH_MAX );
    viewpan[ 1 ] = 1;
    viewpan[ 2 ] = 1;

    struct winsize w; // need ioctl and unistd 
    ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
    char string[PATH_MAX];
    ////////////////////////////////////////////////////////
    if ( argc == 2)
    if ( strcmp( argv[1] , "-s" ) ==  0 ) 
    {
       printf("Screen\n" );
       printf("Env HOME:  %s\n", getenv( "HOME" ));
       printf("Env PATH:  %s\n", getcwd( string, PATH_MAX ) );
       printf("Env TERM ROW:  %d\n", w.ws_row );
       printf("Env TERM COL:  %d\n", w.ws_col );
       return 0;
    }
    rows = w.ws_row ; 
    cols = w.ws_col ; 


    int ch ; 
    int gameover = 0;
    

    while ( gameover == 0 ) 
    {
       strncpy( nexp_user_fileselection, "" , PATH_MAX );
       disable_waiting_for_enter();
       clear_screen();

       ansigotoyx( 0, 0 );
       if ( pansel == 1 )
         printf( "|*1 |[%s]", pathpan[ 1 ] );
       else 
         printf( "| 1 |[%s]", pathpan[ 1 ] );

       ansigotoyx( 0, cols/2 );
       if ( pansel == 2 )
         printf( "|*2 |[%s]", pathpan[ 2 ] );
       else 
         printf( "| 2 |[%s]", pathpan[ 2 ] );

       chdir( pathpan[ 1 ] );
       if ( viewpan[ 1 ] == 1 ) 
          printdir( 0, 0,       "." , 1 );
       chdir( pathpan[ 2 ] );
       if ( viewpan[ 2 ] == 1 ) 
          printdir( 0, cols/2,  "." , 2 );

       ansigotoyx( rows-1, 0 );
       printf( "|%s|F|%d|[%s]", "LKMM" , nexp_user_sel[pansel] ,  nexp_user_fileselection );

       ansigotoyx( rows, 0 );
       printf( "| j:Down | k:Up | h:CdParent | l:CdSel | n:ScrollDown | u:ScrollUp | r:Multi |" );

       ch = getchar();

       chdir( pathpan[ pansel ] );
       if (ch == 'q')            gameover = 1; 
       else if (ch ==  'Q')      gameover = 1;
       else if ( ch == 'w')      
       {
            chdir( pathpan[ pansel ] );
            chdir( getenv( "HOME" ) );
            chdir( "workspace" );
            nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
            strncpy( pathpan[ pansel ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }
       else if ( ch == '~')      
       {
            chdir( pathpan[ pansel ] );
            chdir( getenv( "HOME" ) );
            nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
            strncpy( pathpan[ pansel ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }
       else if ( ch == 'h')      
       {
            chdir( pathpan[ pansel ] );
            chdir( ".." );
            nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
            strncpy( pathpan[ pansel ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }
       else if ( ch == 'l')      
       {
            chdir( pathpan[ pansel ] );
            chdir( nexp_user_fileselection );
            nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
            strncpy( pathpan[ pansel ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }

       else if ( ch == 'r' ) 
       {
           clear_screen();
           strncpy( string , "  " , PATH_MAX );
           strncat( string , " less " , PATH_MAX - strlen( string ) -1 );
           strncat( string , " " , PATH_MAX - strlen( string ) -1 );
           strncat( string , " \"" , PATH_MAX - strlen( string ) -1 );
           strncat( string ,  nexp_user_fileselection , PATH_MAX - strlen( string ) -1 );
           strncat( string , "\" " , PATH_MAX - strlen( string ) -1 );
           nsystem( string ); 
       }

         else if ( ch == '"')      
         {
            chdir( pathpan[ pansel ] );
            strncpy( pathclipboard , getcwd( string, PATH_MAX ), PATH_MAX );
         }
         else if ( ( ch == 'b') || ( ch == '\'')   )
         {
            home();printf( "|BACK|");
            chdir( pathpan[ pansel ] );
            chdir( pathclipboard );
            strncpy( pathpan[ pansel ] , getcwd( string, PATH_MAX ), PATH_MAX );
            nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
         }


       else if ( ch == 10 ) 
       {
           strncpy( string , "  " , PATH_MAX );
           strncat( string , " rox " , PATH_MAX - strlen( string ) -1 );
           strncat( string , " " , PATH_MAX - strlen( string ) -1 );
           strncat( string , " \"" , PATH_MAX - strlen( string ) -1 );
           strncat( string ,  nexp_user_fileselection , PATH_MAX - strlen( string ) -1 );
           strncat( string , "\" " , PATH_MAX - strlen( string ) -1 );
           nsystem( string ); 
       }

       else if ( ch == 'p') 
       {
          if ( fexist(   nexp_user_fileselection ) == 1 )
          {
            readfilesp( nexp_user_fileselection , 0 , rows-4 );
            getchar();
          }
       }

       else if ( ( ch == 'o') && ( pansel == 1 )   )
       {
            chdir( pathpan[ 1 ] );
            chdir( nexp_user_fileselection );
            nexp_user_sel[ 2 ] = 1; 
            nexp_user_scrolly[ 2 ] = 0; 
            strncpy( pathpan[ 2 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }

       else if ( ( ch == 'o') && ( pansel == 2 )   )
       {
            chdir( pathpan[ 2 ] );
            chdir( nexp_user_fileselection );
            nexp_user_sel[ 1 ] = 1; 
            nexp_user_scrolly[ 1 ] = 0; 
            strncpy( pathpan[ 1 ] , getcwd( cwd, PATH_MAX ), PATH_MAX );
       }
       else if ( ch == 'k')      nexp_user_sel[pansel]--;
       else if ( ch == 'j')      nexp_user_sel[pansel]++;
       else if ( ch == 'g')      { nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; }
       else if ( ch == 'u')      nexp_user_scrolly[pansel]-=4;
       else if ( ch == 'd')      nexp_user_scrolly[pansel]+=4;
       else if ( ch == 'n')      nexp_user_scrolly[pansel]+=4;

       else if ( ch == 't' )     printf("%syellow\n", KYEL);
       else if ( ch == 'T' )     printf("%sgreen\n",  KGRN);


       else if ( ch == 'r' )   {  enable_waiting_for_enter();  nrunwith(  " tcview ",  nexp_user_fileselection    );   }
       else if ( ch == 'v' )   {  enable_waiting_for_enter();  nrunwith(  " vim  ",  nexp_user_fileselection    );   }


        else if ( ch == '!') 
        {
            strninput( " Run Cmd on File (!) ", "" );
            strncpy( string, userstr , PATH_MAX );
            printf("\n" );
            printf("\n" );
            printf("got: \"%s\"\n", string );
            if ( strcmp( string , "" ) != 0 ) 
               nrunwith( string , nexp_user_fileselection  ) ; 
        }
        else if ( ch == '&' )     nsystem( " sh  ");
        else if ( ch == '$' ) 
        {
            strninput( " Run SH Command ", "" );
            strncpy( string, userstr , PATH_MAX );
            printf("\n" );
            printf("\n" );
            printf("got: \"%s\"\n", string );
            if ( strcmp( string , "" ) != 0 ) 
            {
               printf("run: \"%s\"\n", string );
               nsystem( string );
               getchar();
            }
        }



       else if ( ch == ';' ) 
       {
            strninput( " Run CHDIR Command ", "" );
            strncpy( string, userstr , PATH_MAX );
            printf("\n" );
            printf("\n" );
            printf("got: \"%s\"\n", string );
            chdir( pathpan[ pansel ] );
            chdir( string );
            strncpy( pathpan[ pansel ] , getcwd( string, PATH_MAX ), PATH_MAX );
            nexp_user_sel[pansel]=1; nexp_user_scrolly[pansel] = 0; 
       }


       else if ( ch == ':' ) 
       {
            clear_screen( );
            ansigotoyx( rows/2-1 , 0 );
            printhline( );
            printf("\n" );
            strninput( "", "" );
            strncpy( string, userstr , PATH_MAX );
            printf("got: \"%s\"\n", string );
       }


       else if ( ch == '1' )
       {  if ( viewpan[ 1 ] == 1 )   viewpan[ 1 ] = 0; else viewpan[ 1 ] = 1; }
       else if ( ch == '2' )
       { if ( viewpan[ 2 ] == 1 )    viewpan[ 2 ] = 0; else viewpan[ 2 ] = 1; }

       else if ( ch == 9 )
       {  if ( pansel == 1 )   pansel = 2 ; else pansel = 1; }
       else if ( ch == 'i' )
       {  if ( pansel == 1 )   pansel = 2 ; else pansel = 1; }


    }

    enable_waiting_for_enter();
    printf( "\n" );
    printf( "Bye!\n" );
    return 0;
}


