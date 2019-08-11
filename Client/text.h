#ifndef TEXT_H
#define TEXT_H

/*
  Exempel
  0. Skapa globala variabler
  1. Ladda text
  2. Rita med glDrawArrays

void loadText(void)
{
     //här skrivs text in i buffer, skickas och returnar vertex array
     // 0. skapa global float array, text#VertexArray[500]. Och int text#v
     // 1. skriv in text i char array, text#[] max 20
     // 2. ändra längd i getVertexArray()
     // 3. fyll i antal punkter i DrawArray, text#v
     float* VertexArray;
     
     char text0[]={"NU FUNKAR"};
     VertexArray=getTextVertexArray(text0, 9);
     for (int i=0;i<VertexArray[0];i++) text0VertexArray[i]=VertexArray[i+1];
     text0v=(int)VertexArray[0]-1;
     
     char text1[]={"DET JU"};
     VertexArray=getTextVertexArray(text1, 6);
     for (int i=0;i<VertexArray[0];i++) text1VertexArray[i]=VertexArray[i+1];
     text1v=(int)VertexArray[0]-1;
}

    //draw Text
    glLoadIdentity();
    glTranslatef(-6,4,-18);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, text0VertexArray);
    glDrawArrays(GL_LINES, 0, text0v/3);
    glDisableClientState(GL_VERTEX_ARRAY);

*/

/*void drawLetter(char* name)
{
     
}
*/
float* getTextVertexArray(char text[], int length)
{
    float A[18]={0,0,0, 0.5,1,0, 0.5,1,0, 1,0,0, 0.25,0.5,0, 0.75,0.5,0};
    float B[24]={0,0,0, 0,1,0, 0,1,0, 1,0.5,0, 1,0.5,0, 0,0.5,0, 1,0.5,0, 0,0,0};
    float C[18]={1,1,0, 0,1,0, 0,1,0, 0,0,0, 0,0,0, 1,0,0};
    float D[18]={0,0,0, 0,1,0, 0,1,0, 1,0.5,0, 1,0.5,0, 0,0,0};
    float E[24]={0,0,0, 0,1,0, 0,1,0, 1,1,0, 0,0.5,0, 0.5,0.5,0, 0,0,0, 1,0,0};
    float F[18]={0,0,0, 0,1,0, 0,1,0, 1,1,0, 0,0.5,0, 0.5,0.5,0};
    float G[24]={1,1,0, 0,0.5,0, 0,0.5,0, 0.5,0,0, 0.5,0,0, 1,0.5,0, 1,0.5,0, 0.5,0.5,0};
    float H[18]={0,0,0, 0,1,0, 1,0,0, 1,1,0, 0,0.5,0, 1,0.5,0};
    float I[18]={0.5,0,0, 0.5,1,0, 0.3,1,0, 0.7,1,0, 0.3,0,0, 0.7,0,0};
    float J[18]={1,1,0, 1,0.5,0, 1,0.5,0, 0.5,0,0, 0.5,0,0, 0,0.5,0};
    float K[18]={0,0,0, 0,1,0, 1,1,0, 0,0.5,0, 0,0.5,0, 1,0,0};
    float L[12]={0,0,0, 0,1,0, 0,0,0, 1,0,0};
    float M[24]={0,0,0, 0,1,0, 0,1,0, 0.5,0.5,0, 0.5,0.5,0, 1,1,0, 1,1,0, 1,0,0};
    float N[18]={0,0,0, 0,1,0, 0,1,0, 1,0,0, 1,0,0, 1,1,0};
    float O[24]={0,0,0, 0,1,0, 0,1,0, 1,1,0, 1,1,0, 1,0,0, 1,0,0, 0,0,0};
    float P[18]={0,0,0, 0,1,0, 0,1,0, 1,0.75,0, 1,0.75,0, 0,0.5,0};
    float Q[24]={0.5,0.25,0, 1,0.5,0, 1,0.5,0, 0.5,1,0, 0.5,1,0, 0,0.5,0, 0,0.5,0, 1,0,0};
    float R[24]={0,0,0, 0,1,0, 0,1,0, 1,0.75,0, 1,0.75,0, 0,0.5,0, 0,0.5,0, 1,0,0};
    float S[18]={1,1,0, 0,0.5,0, 0,0.5,0, 1,0.5,0, 1,0.5,0, 0,0,0};
    float T[12]={0,1,0, 1,1,0, 0.5,0,0, 0.5,1,0};
    float U[18]={0,1,0, 0,0,0, 0,0,0, 1,0,0, 1,0,0, 1,1,0};
    float V[12]={0,1,0, 0.5,0,0, 0.5,0,0, 1,1,0};
    float W[24]={0,1,0, 0.25,0,0, 0.25,0,0, 0.5,0.5,0, 0.5,0.5,0, 0.75,0,0, 0.75,0,0, 1,1,0};
    float X[12]={0,0,0, 1,1,0, 0,1,0, 1,0,0};
    float Y[18]={0.5,0,0, 0.5,0.5,0, 0,1,0, 0.5,0.5,0, 0.5,0.5,0, 1,1,0};
    float Z[18]={0,1,0, 1,1,0, 1,1,0, 0,0,0, 0,0,0, 1,0,0};
    float Less[12]={1,1,0, 0,0.5,0, 0,0.5,0, 1,0,0};
    float Larger[12]={0,1,0, 1,0.5,0, 1,0.5,0, 0,0,0};
    
    float* pointer;
    float vertexArray[500];
    pointer=vertexArray;    
    int arrayOffset=1;
    float space=1.2;

    for (int i=0;i<length;i++)
    {
        switch (text[i])
        {
            case 'A': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=A[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;}  
            case 'B': 
            {for (int j=0;j<24;j++) {vertexArray[j+arrayOffset]=B[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=24;  break;}   
            case 'C': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=C[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;}              
            case 'D': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=D[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;} 
            case 'E': 
            {for (int j=0;j<24;j++) {vertexArray[j+arrayOffset]=E[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=24;  break;} 
            case 'F': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=F[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;} 
            case 'G': 
            {for (int j=0;j<24;j++) {vertexArray[j+arrayOffset]=G[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=24;  break;}  
            case 'H': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=H[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;}  
            case 'I': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=I[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;}  
            case 'J': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=J[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;}  
            case 'K': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=K[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;}  
            case 'L': 
            {for (int j=0;j<12;j++) {vertexArray[j+arrayOffset]=L[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=12;  break;}  
            case 'M': 
            {for (int j=0;j<24;j++) {vertexArray[j+arrayOffset]=M[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=24;  break;}  
            case 'N': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=N[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;}  
            case 'O': 
            {for (int j=0;j<24;j++) {vertexArray[j+arrayOffset]=O[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=24;  break;}  
            case 'P': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=P[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;}  
            case 'Q': 
            {for (int j=0;j<24;j++) {vertexArray[j+arrayOffset]=Q[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=24;  break;}  
            case 'R': 
            {for (int j=0;j<24;j++) {vertexArray[j+arrayOffset]=R[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=24;  break;}  
            case 'S': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=S[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;}  
            case 'T': 
            {for (int j=0;j<12;j++) {vertexArray[j+arrayOffset]=T[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=12;  break;}  
            case 'U': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=U[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;}  
            case 'V': 
            {for (int j=0;j<12;j++) {vertexArray[j+arrayOffset]=V[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=12;  break;}  
            case 'W': 
            {for (int j=0;j<24;j++) {vertexArray[j+arrayOffset]=W[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=24;  break;}  
            case 'X': 
            {for (int j=0;j<12;j++) {vertexArray[j+arrayOffset]=X[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=12;  break;}  
            case 'Y':
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=Y[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;} 
            case 'Z': 
            {for (int j=0;j<18;j++) {vertexArray[j+arrayOffset]=Z[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=18;  break;}  
            case '<': 
            {for (int j=0;j<12;j++) {vertexArray[j+arrayOffset]=Less[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=12;  break;} 
            case '>': 
            {for (int j=0;j<12;j++) {vertexArray[j+arrayOffset]=Larger[j]; if (j%3==0) vertexArray[j+arrayOffset]+=i*space;} 
             arrayOffset+=12;  break;} 
//            case ' ': {vertexArray[j+arrayOffset]+=i*space;}       
        }
    }
    vertexArray[0]=arrayOffset;
//pointer=A;
    return pointer;
}

//första värde i returnad array=lengd

#endif
