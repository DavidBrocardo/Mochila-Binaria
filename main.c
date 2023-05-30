//  ./main Mochilax.txt
//  ./main         Escrever direto no código qual mochila quer dai

#include <stdio.h>
#include <stdlib.h>

// Declaração de variaveis
int capacidade = 0;
int pesoS = 0;
int tamanhoMochila = 0;
int controle = 0;
int Tabu = 0;

int BeneficioMelhor = 0;
int PesoMelhor = 0;
int MaiorMelhor = 0;

// Aloca o vetor
int *Alocar_vetor_int(int m) {
  int *v;      /* ponteiro para a matriz */
  int i;       /* variavel auxiliar      */
  if (m < 1) { /* verifica parametros recebidos */
    printf("** Erro: Parametro invalido **\n");
    return (NULL);
  }
  /* aloca as linhas da matriz */
  v = (int *)calloc(m, sizeof(int *)); /* Um vetor de m ponteiros para float */
  if (v == NULL) {
    printf("** Erro: Memoria Insuficiente **");
    return (NULL);
  }
  return (v); /* retorna o ponteiro para a matriz */
}
// calcula beneficio e os peso de cada solução
void BeneficioPesodaSolucao(int *Beneficios, int *pesoS, int *vetorS,
                            int tamanhoMochila, int *fs, int *peso) {
  *fs = 0;
  *peso = 0;
  for (int i = 0; i < tamanhoMochila; i++) {
    if (vetorS[i] == 1) {
      *fs += Beneficios[i];
      *peso += pesoS[i];
    }
  }
}
//inverte bit para achar vizinhança
int variaSolucao(int *vetorS, int posi) {
  if (vetorS[posi] == 1) {
    vetorS[posi] = 0;
  } else {
    vetorS[posi] = 1;
  }
}
/////////FUNÇÃO PRINCIPAL///////////////
int main(int argc, char *argv[]) {
  int fs, peso;
  int maiorBene = 0;
  int pesodobene = 0;
  int MelhorSolInte = 0;
  int QuantInteracoes = 0;

  // Exclui o arquivo de beneficios para a próxima iteração
  remove("bene.txt");

  // Tamanho da Mochila
  // Quantos elementos serão digitados(B,P)
  printf("\nDigite a Quantidade de itens da Mochila: ");
  scanf("%d", &tamanhoMochila);
  printf("\n");

  // Quantidade de Iterações desejadas
  printf("Digite a Quantidade de Iterações Desejadas: ");
  scanf("%d", &QuantInteracoes);

  int tamanhoListaTabu = 0;
  tamanhoListaTabu =
      (int)tamanhoMochila /
      5; // Quanto maior for |T| mais longa é a proibição de volta a uma solução
         // já visitada; Quanto menor for |T| menor a restrição de revisita
  // Alocacao de vetores
  // printf("%d", tamanhoListaTabu);
  int *LT = Alocar_vetor_int(tamanhoListaTabu);
  int *vetorS = Alocar_vetor_int(tamanhoMochila);
  int *Beneficios = Alocar_vetor_int(tamanhoMochila);
  int *MelhorSolucao = Alocar_vetor_int(tamanhoMochila);
  int *BeneficiosInteracoes = Alocar_vetor_int(QuantInteracoes);
  int *Custos = Alocar_vetor_int(tamanhoMochila);

  for (int i = 0; i < tamanhoListaTabu; i++) {
    LT[i] = tamanhoMochila; // valor grande so para nao interferir
    // lembra de explica
  }
  //********************************************************************//
  ///////////////////////LEITURA DO ARQUIVO MOCHILA///////////////////////
  //*******************************************************************///
  int valor;
  FILE *fp;
  int lens = (int)argc;
  printf("%d", lens);

  if (lens == 1) {
    fp = fopen("Mochila10.txt", "r");
    if (fp == NULL) {
      perror("\nErro ao abrir o arquivo");
      exit(1);
    }
  } else {
    // Abre o arquivo para leitura
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
      perror("\nErro ao abrir o arquivo");
      exit(1);
    }
  }

  int cont = 0;       // contador geral
  int contabene = 0;  // contador de beneficios
  int contacusto = 0; // contador de custos

  while (fscanf(fp, "%d", &valor) == 1) {

    if (cont == 0) {
      // printf("%d", valor);
      //  capacidade da mochila = valor
      capacidade = valor;
    }
    // ler linha dos beneficios
    if (cont <= tamanhoMochila && cont != 0) {
      // vetor de beneficios[cont] =  valor
      // printf("%d ", valor);
      Beneficios[contabene] = valor;
      contabene++;
    }
    // ler linha dos custos
    if (cont > tamanhoMochila) {

      // printf("%d ", valor);
      // vetor de custos[cont] = valor
      Custos[contacusto] = valor;
      contacusto++;
    }
    cont++;
    // printf("Cont: %d", cont);

    // printf(" %d %d\n", cont, valor);
  }

  // Fecha o arquivo
  fclose(fp);

  printf("\nCapacidade da Mochila: %d\n\n", capacidade);
  printf("Vetor dos Beneficios: ");
  for (int i = 0; i < tamanhoMochila; i++) {
    printf("%d ", Beneficios[i]);
  }
  printf("\n\n");
  printf("Vetor dos Custos: ");
  for (int i = 0; i < tamanhoMochila; i++) {
    printf("%d ", Custos[i]);
  }
  printf("\n\n");

  printf("Tamanho da Lista Tabu: %d\n\n", tamanhoListaTabu);

  //********************************************************************//
  //********************************************************************//

  int valida = 0;
  int pver = 0; // peso verificao
  while (valida == 0) {
    // necessario para o rand nao gerar valores iguais
    srand((unsigned)time(NULL));
    int valor1 = rand() % QuantInteracoes;
    int valor2 = rand() % QuantInteracoes;
    // printf("%d %d", valor1, valor2);

    for (int i = 0; i < tamanhoMochila; i++) {

      vetorS[valor1] = 1;
      vetorS[valor2] = 1;
      if (i != valor1) {
        // printf("AQUI");
        vetorS[i] = 0;
      }
      if (i != valor2) {
        vetorS[i] = 0;
      }
      if (vetorS[i] == 1) {
        pver += Custos[i];
      }
    } //checando se a solução é valida
    if (pver <= capacidade) {
      valida = 1;
    }
  }
  printf("Solução inicial: ");
  for (int i = 0; i < tamanhoMochila; i++) {
    printf("%d", vetorS[i]);
  }
  printf("\n");
  int melhoriteracao;
  int primeiraInter = 0;
  maiorBene = 0;
  int interacoes = 0;
  while (interacoes < QuantInteracoes) {
    for (int vizinhaca = 0; vizinhaca < tamanhoMochila; vizinhaca++) {
      // chama para verificar o vizinho
      Tabu = 0;
      if (primeiraInter != 0) {
        for (int i = 0; i < tamanhoListaTabu; i++) {
          if (vizinhaca == LT[i]) {
            Tabu = 1;
          }
        }
        //Verificando lista tabu, ver se é possivel adicionar a posicao a solucao
        if (Tabu == 0) {

          variaSolucao(vetorS, vizinhaca);
          // calculo de beneficio do vizinho
          BeneficioPesodaSolucao(Beneficios, Custos, vetorS, tamanhoMochila,
                                 &fs, &peso);
          // retornando ao vetor a solucao inicial =
          variaSolucao(vetorS, vizinhaca);
        }

      } else {
        primeiraInter++;
        variaSolucao(vetorS, vizinhaca);
        // calculo de beneficio do vizinho
        BeneficioPesodaSolucao(Beneficios, Custos, vetorS, tamanhoMochila, &fs,
                               &peso);
        // retornando ao vetor a solucao inicial =
        variaSolucao(vetorS, vizinhaca);
      }
      //salvando melhores iteraçoes
      if (fs > maiorBene) {
        if (peso <= capacidade) {
          maiorBene = fs;
          pesodobene = peso;
          MelhorSolInte = vizinhaca;

          /*controle =tamanhoListaTabu - 1;
          LT[controle] = vizinhaca;*/
        }
      }
    }

    printf("\n");
    printf("Iteração %d:  \n", interacoes + 1);
    //********************************************************************//
    ////////////////////////SALVANDO BENEFICIOS NUM ARQUIVO////////////////
    //*******************************************************************//
    FILE *be;
    int num1 = 10, num2 = 20, num3 = 30;

    be = fopen("bene.txt", "a");
    if (fp == NULL) {
      printf("\nErro ao abrir o arquivo!\n");
      return 1;
    }

    fprintf(be, "%d ", maiorBene);

    //////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////
    BeneficiosInteracoes[interacoes] = maiorBene;
    printf("Beneficio: %d  Peso: %d  Bit Alterado: %d \n", maiorBene,
           pesodobene, MelhorSolInte);
    // resgistra as modificaçoes na lista tabu
    int tamLT = tamanhoListaTabu - 1;

    for (int i = 0; i < tamLT; i++) {
      int c = i + 1;
      //lista tabu
      LT[i] = LT[c];
      // printf("%d ", LT[i]);
    }

    LT[tamLT] = MelhorSolInte;
    //  printf(" %d\n", LT[tamLT]);
    // altera o S0 para a proxima interaçao
    printf("Solução %d: ", interacoes + 1);
    variaSolucao(vetorS, MelhorSolInte);

    for (int i = 0; i < tamanhoMochila; i++) {
      printf("%d", vetorS[i]);
    }
    printf("\n");

    //salvando melhores iteraçoes
    if (maiorBene > BeneficioMelhor) {
      melhoriteracao = interacoes;
      BeneficioMelhor = maiorBene;
      PesoMelhor = pesodobene;
      for (int i = 0; i < tamanhoMochila; i++) {
        MelhorSolucao[i] = vetorS[i];
      }
    }

    maiorBene = 0; // Necessario para que na proxima interacao o algoritmo pegue
                   // o melhor vizinho
    interacoes++;
    fclose(be);
  }
  printf("\n=============================================");
  printf("\nMelhor Solução Encontrada na Iteração %d", melhoriteracao + 1);
  printf("\nMelhor Beneficio Encontrado: %d", BeneficioMelhor);
  printf("\nPeso desse Beneficio: %d", PesoMelhor);
  printf("\n=============================================\n");

  return 0;
}