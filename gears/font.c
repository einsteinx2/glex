// #include <stdlib.h>

// #include <glad/gl.h>
// #define GLFW_INCLUDE_NONE
// #include <GLFW/glfw3.h>


// void displayString(char* text, int length, GLfloat l, int active, int align) {
//     int i, n;
//     glPushMatrix();
//     glTranslatef(0.0, -l, 0.0);
//     if (align == 1) glTranslatef(-15.0, 0.0, 0.0); //left alignment
//     if (align == 3) glTranslatef(15.0, 0, 0); //right aligment
//     for (int i = 0; i < length; i++) {
//         glTranslatef(1,0,0);
//         char c = text[i];
//         if (c == 32) continue; // space
//         if (c >= 48 && c <= 58) c = c - 47; // translate ascii code to the
//         if (c >= 65 && c <= 90) c = c - 53; // corresponding position in the bitmap
        
//     }
// }

// procedure DisplayString(str: string; l: integer; active: integer; align: integer);
// var i,n: integer;
// s: string;
// begin
//   glPushMatrix;
//   glTranslatef(0,-l,0);
//   if align=1 then glTranslatef(-15,0,0); //left alignment
//   if align=3 then glTranslatef(15,0,0);//right aligment
//   s:=uppercase(str); // the font is uppercase only
//   for i:=1 to length(str)-4 do begin
//    n:=ord(s[i]); // get the ASCII code of letter
//    glTranslatef(1,0,0);
//    if n=32 then continue; //space
//    if (n>=48) and (n<=58) then n:=n-47; // translate ascii code to the
//    if (n>=65) and (n<=90) then n:=n-53; // corresponding position in the bitmap
//       if active=0 then glBindTexture(GL_TEXTURE_2D, font2) // two different font files used
//     else glBindTexture(GL_TEXTURE_2D, font);
//     glBegin(GL_QUADS); // 1850 is the total width of the bitmap image, 50 is one character
//      glTexCoord2f(((n-1)*50/1850)-1, 0.0); glVertex3f(0.0, 0.0,  1.0);
//      glTexCoord2f((n*50/1850)-1, 0.0); glVertex3f(1.0, 0.0,  1.0);
//      glTexCoord2f((n*50/1850)-1, 1.0); glVertex3f(1.0, 1.0,  1.0);
//      glTexCoord2f(((n-1)*50/1850)-1, 1.0); glVertex3f(0.0, 1.0,  1.0);
//     glEnd();
//     end;
//    glPopMatrix;
// end;