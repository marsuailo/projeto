#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================================
// Estrutura do Funcionário
// ================================

enum Status { INATIVO = 0, ATIVO, PENDENTE };
enum Cargo { GERENTE = 1, FUNCIONARIO, ESTAGIARIO };

typedef struct {
    int id;                   
    char nome[50];                
    enum Cargo tipo_cargo;     
    enum Status ativo;          
    int dia;                    
    int mes;                 
    int ano;                    
} Funcionario;

// ================================
// Funções auxiliares
// ================================

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


int proximo_id(FILE *arquivo) {
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    return (int)(tamanho / sizeof(Funcionario)) + 1;
}

// ================================
//  Criar
// ================================

void criar_funcionario(FILE *arquivo) {
    Funcionario f;
    f.id = proximo_id(arquivo);
    f.ativo = ATIVO;

    printf("\n--- Cadastro Novo Funcionario ---\n");
    limpar_buffer();
    printf("Nome: ");
    fgets(f.nome, sizeof(f.nome), stdin);
    f.nome[strcspn(f.nome, "\n")] = '\0';

    printf("Cargo (1-Gerente, 2-Funcionario, 3-Estagiario): ");
    scanf("%d", (int*)&f.tipo_cargo);

    printf("Data de nascimento (dia/mes/ano): ");
    printf("Dia: ");
    scanf("%d", &f.dia);
    printf("mes: ");
    scanf("%d",&f.mes);
    printf("ano: ");
    scanf("%d", &f.ano);

    fseek(arquivo, (f.id - 1) * sizeof(Funcionario), SEEK_SET);
    fwrite(&f, sizeof(Funcionario), 1, arquivo);
    fflush(arquivo);

    printf("\nFuncionario cadastrado com sucesso! (ID: %d)\n", f.id);
}
// ================================
// MOSTRAR FUNCIONARIOS
// ================================

void listar_funcionarios(FILE *arquivo) {
    Funcionario f;
    rewind(arquivo);

    printf("\n--- Lista de Funcionarios Ativos ---\n");
    while (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo == ATIVO) {
            printf("\nID: %d\n", f.id);
            printf("Nome: %s\n", f.nome);
            printf("Nascimento: %02d/%02d/%04d\n", f.dia, f.mes, f.ano);
            switch (f.tipo_cargo) {
                case GERENTE: printf("Cargo: Gerente\n"); break;
                case FUNCIONARIO: printf("Cargo: Funcionario\n"); break;
                case ESTAGIARIO: printf("Cargo: Estagiario\n"); break;
            }
        }
    }
}
// ================================
// BUSCAR FUNCIONARIO
// ================================

void buscar_funcionario(FILE *arquivo) {
    int id;
    printf("Digite o ID do funcionario para buscar: ");
    scanf("%d", &id);

    Funcionario f;
    fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
    if (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo == ATIVO) {
            printf("\n--- Funcionario Encontrado ---\n");
            printf("ID: %d\n", f.id);
            printf("Nome: %s\n", f.nome);
            printf("Nascimento: %02d/%02d/%04d\n", f.dia, f.mes, f.ano);
            switch (f.tipo_cargo) {
                case GERENTE: printf("Cargo: Gerente\n"); break;
                case FUNCIONARIO: printf("Cargo: Funcionario\n"); break;
                case ESTAGIARIO: printf("Cargo: Estagiáaio\n"); break;
            }
        } else {
            printf("Este funcionario esta inativo.\n");
        }
    } else {
        printf("Funcionario nao encontrado.\n");
    }
}
// ================================
// EDITAR
// ================================

void editar_funcionario(FILE *arquivo) {
    int id;
    printf("Digite o ID do funcionario para editar: ");
    scanf("%d", &id);

    Funcionario f;
    fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
    if (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo == ATIVO) {
            printf("Nome (atual: %s) \n", f.nome);
            fgets(f.nome, sizeof(f.nome), stdin);
            f.nome[strcspn(f.nome, "\n")] = '\0';

            printf("Novo cargo (1-Gerente, 2-Funcionario, 3-Estagiario) (atual: %d): ", f.tipo_cargo);
            scanf("%d", (int*)&f.tipo_cargo);

            printf("Nova data de nascimento (dia mes ano) (atual: %02d/%02d/%04d): ", f.dia, f.mes, f.ano);
            scanf("%d %d %d", &f.dia, &f.mes, &f.ano);

            fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
            fwrite(&f, sizeof(Funcionario), 1, arquivo);
            fflush(arquivo);

            printf("Funcionario atualizado com sucesso!\n");
        } else {
            printf("Este funcionario está inativo e nao pode ser editado.\n");
        }
    } else {
        printf("Funcionario nao encontrado.\n");
    }
}
// ================================
// EXCLUIR
// ================================

void excluir_funcionario(FILE *arquivo) {
    int id;
    printf("Digite o ID do funcionario para remover: ");
    scanf("%d", &id);

    Funcionario f;
    fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
    if (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo == ATIVO) {
            f.ativo = INATIVO;
            fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
            fwrite(&f, sizeof(Funcionario), 1, arquivo);
            fflush(arquivo);
            printf("Funcionario marcado como inativo.\n");
        } else {
            printf("Esse funcionario ja esta inativo.\n");
        }
    } else {
        printf("Funcionario nao encontrado.\n");
    }
}

// ================================
// Menu Principal
// ================================

void menu(FILE *arquivo) {
    int opcao = -1;

    while (opcao != 0) {
        printf("\n=== SISTEMA DE FUNCIONARIOS ===\n");
        printf("1. Cadastrar funcionario\n");
        printf("2. Buscar funcionario\n");
        printf("3. Editar funcionario\n");
        printf("4. Excluir funcionario\n");
        printf("5. Listar funcionarios\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: criar_funcionario(arquivo); break;
            case 2: buscar_funcionario(arquivo); break;
            case 3: editar_funcionario(arquivo); break;
            case 4: excluir_funcionario(arquivo); break;
            case 5: listar_funcionarios(arquivo); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    }
}

// ================================
// Função Principal
// ================================

int main() {
    FILE *arquivo = fopen("funcionarios.dat", "r+b");
    if (!arquivo) {
        arquivo = fopen("funcionarios.dat", "w+b");
        if (!arquivo) {
            printf("Erro ao abrir o arquivo.\n");
            return 1;
        }
    }

    menu(arquivo);
    fclose(arquivo);
    return 0;
}
