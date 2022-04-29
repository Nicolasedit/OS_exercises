#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <strings.h>
#include <string.h>

int main(){

   /*F1 ed F2 sono figli di P
   F3 è figlio di F1
    
   1)P chiede all'utente di inserire il testo di un signal da inviare al processo F3
   2)P invia la stringa inserita all'utente ad F1
   3)F1 la inoltra a F2
   4)F2 traduce la stringa nel corrispondente signal decimale e lo rimanda ad F1
   5)F1 genera il comando di sistema per inviare il signal ad F3
   */
   
   /*
   F1 and F2 are child processes of P
   F3 is child of F1
   
   1) P asks to insert a string that will be the signal to send to F3
   2) P sends the string to F1
   3) F1 sends the string to F2
   4) F2 translate the string to the int signal correspondant and sends it to F3
   */

    int fd_p_f1[2];
    int fd_f1_f2[2];
    int fd_f1_f3[2];
    char cod[7];
    int *child_status1, *child_status2;
    pipe(fd_p_f1);
    pid_t pid_f1 = fork();

    if(pid_f1 == 0){
        pipe(fd_f1_f3);
        pid_t pid_f3 = fork();

        if(pid_f3 == 0){
            //F3 code
            int ppid = (int)getpid();
            close(fd_f1_f3[0]);
            write(fd_f1_f3[1], &ppid, sizeof(ppid));

        }else{
            //F1 code
            char c[7];
            int sig, f3;
            close(fd_p_f1[1]);
            read(fd_p_f1[0], &c, strlen(c));
            printf("F1: La stringa letta da P e': %s\n", c);
            
            write(fd_f1_f2[1], &c, sizeof(c));
            printf("F1: Ho inviato la stringa ad F2\n");
            
            read(fd_f1_f2[0], &sig, sizeof(sig));
            printf("F1: Ho ricevuto l'intero %d corrispondente alla stringa %s\n", sig, c);
            
            read(fd_f1_f3[0], &f3, sizeof(f3));
            printf("F1: PID F3: %d", (int)f3);
            
           kill(f3, sig);
            printf("kill(%d,%d)", f3, sig);


        }
    
    }else{
        pipe(fd_f1_f2);
        pid_t pid_f2 = fork();
        if(pid_f2 == 0){
            //F2 code
            int sgg;
            char sig_to_convert[7];
            read(fd_f1_f2[0], &sig_to_convert, sizeof(sig_to_convert));
            if(strcmp(sig_to_convert, "SIGTERM")== 0){
                sgg = 15;
            }
            else if(strcmp(sig_to_convert, "SIGSTOP") == 0){
                sgg = 17;
            }
            else if(strcmp(sig_to_convert, "SIGKILL") == 0){
                sgg = 9;
            }
            else if(strcmp(sig_to_convert, "SIGINT") == 0){
                sgg = 2;
            }
            else{
                printf("Errore nel signal.\n");
            }
            write(fd_f1_f2[1], &sgg, sizeof(sgg));
            printf("F2: Ho mandato l'intero %d ad F1 corrispondente al signal %s", sgg, sig_to_convert);

        }else{
            //P code
            printf("Inserisci una stringa corrispondente al Signal da inviare: \n");
            scanf("%s", cod);
            close(fd_p_f1[0]);
            write(fd_p_f1[1], &cod, sizeof(cod));
            printf("P: Ho inviato il signal %s ad F1\n", cod);
            
        }
            
            
    }





}
