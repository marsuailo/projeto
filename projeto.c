#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================================
// Estrutura do Funcionário
// ================================
enum Status {INATIVO=0, ATIVO, PENDENTE};
enum Cargo (gerente, funcionario, estagiario);
typedef struct {
    int id;                     // ID único
    char nome[50];              // Nome do funcionário   
                            // 1 = ativo, 0 = removido
    enum Cargo cargo;
    enum status ativo;
        struct {
            int dia;
            int mes;
            int ano;
        } aniversario;
    int tipo_cargo; // 1 = gerente, 2 = funcionário, 3 = estagiário
} Funcionario;

// ================================
// Funções auxiliares
// ================================

int proximo_id(FILE *arquivo) {
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    return (int)(tamanho / sizeof(Funcionario)) + 1;
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ================================
// CRUD
// ================================

void criar_funcionario(FILE *arquivo) {
    Funcionario f;
    f.id = proximo_id(arquivo);
    f.ativo = 1;

    printf("\n--- Cadastro de Novo Funcionário ---\n");
    printf("Nome: ");
    limpar_buffer();
    fgets(f.nome, sizeof(f.nome), stdin);
    f.nome[strcspn(f.nome, "\n")] = '\0';

    printf("Data de nascimento (dd/mm/aaaa): ");
    fgets(f.data_nascimento, sizeof(f.data_nascimento), stdin);
    f.data_nascimento[strcspn(f.data_nascimento, "\n")] = '\0';

    printf("Tipo de cargo (1-Gerente, 2-Funcionário, 3-Estagiário): ");
    scanf("%d", &f.tipo_cargo);
    limpar_buffer();

    if (f.tipo_cargo == 1) {
        printf("Departamento: ");
        fgets(f.cargo.gerente.departamento, sizeof(f.cargo.gerente.departamento), stdin);
        f.cargo.gerente.departamento[strcspn(f.cargo.gerente.departamento, "\n")] = '\0';
        printf("Salário: ");
        scanf("%f", &f.cargo.gerente.salario);
        limpar_buffer();
        printf("Projetos (máx 5):\n");
        for (int i = 0; i < 5; i++) {
            printf("Projeto %d (vazio para parar): ", i + 1);
            fgets(f.cargo.gerente.projetos[i], 20, stdin);
            f.cargo.gerente.projetos[i][strcspn(f.cargo.gerente.projetos[i], "\n")] = '\0';
            if (strlen(f.cargo.gerente.projetos[i]) == 0) break;
        }
    } else if (f.tipo_cargo == 2) {
        printf("Setor: ");
        fgets(f.cargo.funcionario.setor, sizeof(f.cargo.funcionario.setor), stdin);
        f.cargo.funcionario.setor[strcspn(f.cargo.funcionario.setor, "\n")] = '\0';
        printf("ID do supervisor: ");
        scanf("%d", &f.cargo.funcionario.supervisor_id);
        limpar_buffer();
        printf("Tarefas (máx 5):\n");
        for (int i = 0; i < 5; i++) {
            printf("Tarefa %d (vazio para parar): ", i + 1);
            fgets(f.cargo.funcionario.tarefas[i], 20, stdin);
            f.cargo.funcionario.tarefas[i][strcspn(f.cargo.funcionario.tarefas[i], "\n")] = '\0';
            if (strlen(f.cargo.funcionario.tarefas[i]) == 0) break;
        }
    } else {
        printf("Área: ");
        fgets(f.cargo.estagiario.area, sizeof(f.cargo.estagiario.area), stdin);
        f.cargo.estagiario.area[strcspn(f.cargo.estagiario.area, "\n")] = '\0';
        printf("Horas semanais: ");
        scanf("%d", &f.cargo.estagiario.horas_semanais);
        limpar_buffer();
        printf("Disciplinas (máx 5):\n");
        for (int i = 0; i < 5; i++) {
            printf("Disciplina %d (vazio para parar): ", i + 1);
            fgets(f.cargo.estagiario.disciplinas[i], 20, stdin);
            f.cargo.estagiario.disciplinas[i][strcspn(f.cargo.estagiario.disciplinas[i], "\n")] = '\0';
            if (strlen(f.cargo.estagiario.disciplinas[i]) == 0) break;
        }
    }

    fseek(arquivo, (f.id - 1) * sizeof(Funcionario), SEEK_SET);
    fwrite(&f, sizeof(Funcionario), 1, arquivo);
    fflush(arquivo);

    printf("\nFuncionário cadastrado com sucesso! (ID: %d)\n", f.id);
}

void listar_funcionarios(FILE *arquivo) {
    Funcionario f;
    rewind(arquivo);
    printf("\n--- Lista de Funcionários Ativos ---\n");
    while (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo) {
            printf("\nID: %d\nNome: %s\nNascimento: %s\n", f.id, f.nome, f.data_nascimento);
            if (f.tipo_cargo == 1)
                printf("Cargo: Gerente (%s)\n", f.cargo.gerente.departamento);
            else if (f.tipo_cargo == 2)
                printf("Cargo: Funcionário (%s)\n", f.cargo.funcionario.setor);
            else
                printf("Cargo: Estagiário (%s)\n", f.cargo.estagiario.area);
        }
    }
}

void buscar_funcionario(FILE *arquivo) {
    int id;
    printf("Digite o ID para buscar: ");
    scanf("%d", &id);

    Funcionario f;
    fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
    if (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo) {
            printf("\n--- Funcionário Encontrado ---\n");
            printf("ID: %d\nNome: %s\nNascimento: %s\n", f.id, f.nome, f.data_nascimento);
        } else {
            printf("Este funcionário foi removido.\n");
        }
    } else {
        printf("Funcionário não encontrado.\n");
    }
}

void excluir_funcionario(FILE *arquivo) {
    int id;
    printf("Digite o ID do funcionário a remover: ");
    scanf("%d", &id);

    Funcionario f;
    fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
    if (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo) {
            f.ativo = 0;
            fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
            fwrite(&f, sizeof(Funcionario), 1, arquivo);
            fflush(arquivo);
            printf("Funcionário removido com sucesso.\n");
        } else {
            printf("Esse funcionário já estava removido.\n");
        }
    } else {
        printf("Funcionário não encontrado.\n");
    }
}

// ================================
// Menu Principal
// ================================

void menu(FILE *arquivo) {
    int opcao = -1;

    while (opcao != 0) {
        printf("\n=== SISTEMA DE FUNCIONÁRIOS ===\n");
        printf("1. Cadastrar funcionário\n");
        printf("2. Buscar funcionário\n");
        printf("3. Excluir funcionário\n");
        printf("4. Listar funcionários\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: criar_funcionario(arquivo); break;
            case 2: buscar_funcionario(arquivo); break;
            case 3: excluir_funcionario(arquivo); break;
            case 4: listar_funcionarios(arquivo); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
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
