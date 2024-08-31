//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//%%%%%%%%%%%%%%%%%%%%%%%%% Ising 2D %%%%%%%%%%%%%%%%%%%%%%%//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//
//%%%%%%%%%%%% Elaborado por: Igor Beder Burti R. %%%%%%%%%%//
//%%%%%%%%%%%%%% Última modificação: 31/07/2024 %%%%%%%%%%%%//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(){

    double J, H, Temp, TempMax, dt, Cv, E, SomaE2, SomaE, MediaE, MediaE2, M, SomaM, SomaM2, SomaMAbs, MediaM, MediaM2, MediaMAbs;
    double DeltaE, P, x3, X, Y; 
    int L, N, i, k, tmax, t, x1, x2, Dir, Esq, Cima, Baixo, Passos, NumPassos, NumPassosMedia;
    FILE *energia, *magnetizacao, *energiaTemp, *magnetizacaoTemp, *suscep, *calor, *divisao;

    //energia = fopen("DoubleLoopEnergia_T_2.5.dat", "w");
    //magnetizacao = fopen("DoubleLoopMagnetizacao_T_2.5.dat", "w");
    //energiaTemp = fopen("EnergiaTemp.dat", "w");
    magnetizacaoTemp = fopen("MagnetizacaoTemp_L100.dat", "w"); 
    suscep = fopen("Susceptibilidade_L100.dat", "w");
    calor = fopen("CalorEspecifico_L100.dat", "w");
    //divisao = fopen("U_L16.dat", "w");

    srand(time(NULL));
    
    J = 1.0;
    //Temp = 2.5;
    H = 0.0;
    L = 100;
    tmax = L*L; // numero de iterações

    double S[L+1][L+1];

    for (i = 1; i <= L; i++){ // Condição inicial: todos os spins alinhados

        for(k = 1; k <= L; k++){
            
                S[i][k] = 1.0;

        }

    }

    E = -2*J*L*L - H*L;
    M = L*L;
    
    TempMax = 5.0;

for (Temp = 0.1; Temp <= TempMax; Temp = Temp + dt){ // Loop da temperatura

    SomaE = 0.0;
    SomaE2 = 0.0;
    SomaM = 0.0;
    SomaM2 = 0.0;
    SomaMAbs = 0.0;

    if (Temp >= 1.8 & Temp <= 3.00){

        dt = 0.030;
        NumPassos = 600000;
        NumPassosMedia = NumPassos - 10000; 

    }

    else{

        dt = 0.1;
        NumPassos = 100000;
        NumPassosMedia = NumPassos - 10000; 

    }

    for (Passos = 1; Passos <= NumPassos; Passos++){ // Loop dos passos (1 a 100000)

        for (t = 1; t <= tmax; t++){ // Loop dos passos 2 (1 a L*L)

            x1 = rand() % 100 + 1;
            x2 = rand() % 100 + 1;

            Dir = x1 + 1;
            if (Dir > L){
                Dir = 1;
            }

            Esq = x1 - 1;
            if (Esq < 1){
                Esq = L;
            }
            
            Cima = x2 - 1;
            if (Cima < 1){
                Cima = L;
            }

            Baixo = x2 + 1;
            if (Baixo > L){
                Baixo = 1;
            }
            
            DeltaE = 2*J*S[x1][x2]*(S[Dir][x2] + S[Esq][x2] + S[x1][Cima] + S[x1][Baixo]) + 2*H*S[x1][x2];

            if (DeltaE <= 0){

                S[x1][x2] = -S[x1][x2];
                E = E + DeltaE;
                M = M + 2*S[x1][x2];

            }

            else{
                
                P = exp(-DeltaE/Temp);
                x3 = (double)rand()/(double)RAND_MAX;

                if (P > x3){

                    S[x1][x2] = -S[x1][x2];
                    E = E + DeltaE;
                    M = M + 2*S[x1][x2];

                }

            }


        } // Loop do tempo

    //fprintf(energia, "%i, %.8g\n", Passos, E); //Energia em função dos passos
    //fprintf(magnetizacao,"%i, %.8g\n", Passos, fabs(M)); //Modulo da magnetização em função dos passos

    if(Passos > 10000){

    SomaE = SomaE + E;
    SomaE2 = SomaE2 + E*E;
    SomaMAbs = SomaMAbs + fabs(M);
    SomaM2 = SomaM2 + M*M;
    SomaM = SomaM + M;

    }

    } // Loop dos passos

    MediaE = SomaE/((double)NumPassosMedia);
    MediaE2 = SomaE2/((double)NumPassosMedia);
    MediaM = SomaM/((double)NumPassosMedia);
    MediaM2 = SomaM2/((double)NumPassosMedia);
    MediaMAbs = SomaMAbs/((double)NumPassosMedia);
    Cv = (1.0/(Temp*Temp))*(MediaE2 - MediaE*MediaE);
    X = (1.0/(Temp*Temp))*(MediaM2 - MediaM*MediaM);
    Y = MediaM2/(MediaMAbs*MediaMAbs);
    //fprintf(energiaTemp, "%lf, %lf\n", Temp, MediaE);
    //fprintf(divisao, "%lf, %lf\n", Temp, Y);
    fprintf(calor, "%lf, %lf\n", Temp, Cv);
    fprintf(magnetizacaoTemp, "%lf, %lf\n", Temp, MediaMAbs);
    fprintf(suscep, "%lf, %lf\n", Temp, X);
    printf("%lf\n", Temp);
    

} // Loop da temperatura

} // Término do programa