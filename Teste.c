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
    unsigned char aniversario[31][12]; // [0..30] => dias 1..31, [0..11] => meses 1..12
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

void limpar_aniversario(Funcionario *f) {
    memset(f->aniversario, 0, sizeof(f->aniversario));
}

void define_aniversario(Funcionario *f, int dia, int mes) {
    limpar_aniversario(f);
    if (dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12) {
        f->aniversario[dia - 1][mes - 1] = 1;
    }
}

int verifica_aniversario(const Funcionario *f, int *dia, int *mes) {
    for (int d = 0; d < 31; d++) {
        for (int m = 0; m < 12; m++) {
            if (f->aniversario[d][m]) {
                if (dia) *dia = d + 1;
                if (mes) *mes = m + 1;
                return 1;
            }
        }
    }
    return 0;
}

// ================================
//  Criar
// ================================

void criar_funcionario(FILE *arquivo) {
    Funcionario f;
    f.id = proximo_id(arquivo);
    f.ativo = ATIVO;
    limpar_aniversario(&f);

    printf("\n--- Cadastro Novo Funcionario ---\n");
    limpar_buffer();
    printf("Nome: ");
    fgets(f.nome, sizeof(f.nome), stdin);
    f.nome[strcspn(f.nome, "\n")] = '\0';

    int tmpCargo;
    printf("Cargo (1-Gerente, 2-Funcionario, 3-Estagiario): ");
    if (scanf("%d", &tmpCargo) != 1) tmpCargo = 2;
    f.tipo_cargo = (tmpCargo >= 1 && tmpCargo <= 3) ? (enum Cargo)tmpCargo : FUNCIONARIO;

    int dia = 0, mes = 0;
    do {
        printf("Data de nascimento - Dia (1-31): ");
        if (scanf("%d", &dia) != 1) { limpar_buffer(); dia = 0; }
        if (dia < 1 || dia > 31) printf("Dia invalido. Tente novamente.\n");
    } while (dia < 1 || dia > 31);

    do {
        printf("Data de nascimento - Mes (1-12): ");
        if (scanf("%d", &mes) != 1) { limpar_buffer(); mes = 0; }
        if (mes < 1 || mes > 12) printf("Mes invalido. Tente novamente.\n");
    } while (mes < 1 || mes > 12);

    define_aniversario(&f, dia, mes);

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
            int d, m;
            printf("\nID: %d\n", f.id);
            printf("Nome: %s\n", f.nome);
            if (verifica_aniversario(&f, &d, &m)) {
                printf("Nascimento: %02d/%02d\n", d, m);
            } else {
                printf("Nascimento: N/A\n");
            }
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
    if (scanf("%d", &id) != 1) { limpar_buffer(); printf("ID invalido.\n"); return; }

    Funcionario f;
    fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
    if (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo == ATIVO) {
            int d, m;
            printf("\n--- Funcionario Encontrado ---\n");
            printf("ID: %d\n", f.id);
            printf("Nome: %s\n", f.nome);
            if (verifica_aniversario(&f, &d, &m)) {
                printf("Nascimento: %02d/%02d\n", d, m);
            } else {
                printf("Nascimento: N/A\n");
            }
            switch (f.tipo_cargo) {
                case GERENTE: printf("Cargo: Gerente\n"); break;
                case FUNCIONARIO: printf("Cargo: Funcionario\n"); break;
                case ESTAGIARIO: printf("Cargo: Estagiario\n"); break;
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
    if (scanf("%d", &id) != 1) { limpar_buffer(); printf("ID invalido.\n"); return; }

    Funcionario f;
    fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
    if (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo == ATIVO) {
            limpar_buffer();
            char novo_nome[50];
            printf("Nome (atual: %s)\n", f.nome);
            printf("Novo nome (pressione enter para manter): ");
            fgets(novo_nome, sizeof(novo_nome), stdin);
            if (novo_nome[0] != '\n') {
                novo_nome[strcspn(novo_nome, "\n")] = '\0';
                strncpy(f.nome, novo_nome, sizeof(f.nome));
                f.nome[sizeof(f.nome)-1] = '\0';
            }

            int tmpCargo;
            printf("Novo cargo (1-Gerente, 2-Funcionario, 3-Estagiario) (atual: %d): ", f.tipo_cargo);
            if (scanf("%d", &tmpCargo) == 1) {
                if (tmpCargo >= 1 && tmpCargo <= 3) f.tipo_cargo = (enum Cargo)tmpCargo;
            } else {
                limpar_buffer();
            }

            int dia = 0, mes = 0;
            int cur_d = 0, cur_m = 0;
            verifica_aniversario(&f, &cur_d, &cur_m);

            printf("Nova data de nascimento (Digite 0 0 para manter atual) (atual: %02d/%02d)\n", cur_d, cur_m);
            do {
                printf("Dia (0 para manter, 1-31 para alterar): ");
                if (scanf("%d", &dia) != 1) { limpar_buffer(); dia = -1; }
                if (dia < 0 || (dia > 31)) printf("Dia invalido. Tente novamente.\n");
            } while (dia < 0 || dia > 31);

            do {
                printf("Mes (0 para manter, 1-12 para alterar): ");
                if (scanf("%d", &mes) != 1) { limpar_buffer(); mes = -1; }
                if (mes < 0 || (mes > 12)) printf("Mes invalido. Tente novamente.\n");
            } while (mes < 0 || mes > 12);

            if (dia == 0 && mes == 0) {
                // manter atual
            } else {
                if (dia == 0) dia = cur_d ? cur_d : 1;
                if (mes == 0) mes = cur_m ? cur_m : 1;
                define_aniversario(&f, dia, mes);
            }

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
    if (scanf("%d", &id) != 1) { limpar_buffer(); printf("ID invalido.\n"); return; }

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
        if (scanf("%d", &opcao) != 1) { limpar_buffer(); opcao = -1; }

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
