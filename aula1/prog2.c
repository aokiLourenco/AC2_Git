int main(void) 
   { 
    char c; 
 
    do 
    { 
     c = getChar(); 
     putChar( c+1 ); 
    } while( c != '\n' ); 
    return 0; 
   }