#include <stdio.h>
#include <stdlib.h>

int main()
{

    FILE* outputfile;
    int accelsteps=0,vend=0,freq=0,timerafter=0;
    double zeit=0,accel=0;

    printf("Anzahl der Schritte während der Beschleunigung: ");
    scanf("%i",&accelsteps);
    int steps[accelsteps];

    printf("\nGeschwindigkeit nach der Beschleunigung: ");
    scanf("%i",&vend);

    printf("\nTimer Frequenz: ");
    scanf("%i",&freq);

    zeit=2.0*accelsteps/vend;
    accel=(vend*vend)/(2*accelsteps);
    printf("%f\n",accel);
    timerafter=zeit*freq+1;

    int laststeps=0;
    int currentsteps=0;
    int j=0;
    for(int i=1;i<=timerafter;i++)
    {
        currentsteps=accel*(i/(float)freq)*(i/(float)freq)/2;
        if(currentsteps>laststeps)
        {
            steps[j++]=i;
            laststeps=currentsteps;
            printf("%i\n",i);
        }

    }
    outputfile=fopen("output.txt","w+");
    for(int i=0;i<accelsteps/10;i++)
    {
        for(int j = i*10;j<10*i+10;j++)
        {
            fprintf(outputfile,"%i, ",steps[j]);
        }
        fputs("\n",outputfile);
    }
    fclose(outputfile);
    system("pause");
}
