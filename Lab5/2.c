#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <fcntl.h>

#define SIZE 100 //dowolnie dobrana wielkosc

int main (int argc, char* argv[])
{

    //wieksza czesc zadania jest taka sama jak w zad 1, zmiany zachodza w dziecku
    int potok[2]; //2 konce potoku
    int id, k, file; //id - file descriptor
    char buffer[SIZE];
    char *filename = argv[1];

    if (pipe(potok) == -1) //utworzenie potoku funkcja pipe
	{		
		printf("Blad tworzenia potoku. \n");
	}
    else
	{
		printf("Potok zostal utworzony. \n");
	}

    pid_t pid;
	pid = fork(); //utworzenie podprocesu

    if (pid < 0)
 	{
		printf("Blad tworzenia procesu potomnego. \n");
	}

    //rodzic - proces nadrzedny
	else if (pid > 0)
	{
        printf("Uruchomiono proces nadrzedny. \n");
        close(potok[0]); //zamyka koniec potoku do odczytu

        if (argc != 2)
		{
			printf("Wymaganie jest podanie jednego argumentu. \n");
		}
        else
        {
            if((id = open(filename, O_RDONLY)) < 0)
            {
                printf("Nie udalo sie otworzyc pliku. \n");
            }
            
            while ((k = read(id, &buffer, SIZE)) > 0 )
            {
                if (write(potok[1], &buffer, k) < 0) //wpisywanie do konca potoku
                {
				    printf("Nie udalo sie zapisac do potoku. \n");
			    }
 
            }

            close(id);
            close(potok[1]); //zamyka koniec potoku do wpisywania
        }
        
    }

    //dziecko - proces podrzedny
	else
	{
		close(potok[1]); //zamyka koniec potoku do wpisywania
        printf("Uruchomiono podproces. \n");

        close(0);
        dup(potok[0]); //przekierowuje nr zduplikowanego deskryptora pliku
        close(potok[0]); //zamyka koniec potoku do odczytu

        execlp("display", "display", "-", NULL); //wyswietla obrazek programem display
    }

    return 0;
}



